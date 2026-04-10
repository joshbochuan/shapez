//
// Created by joshb on 2026/4/1.
//
#include "../../include/Opti/OptiRenderer.hpp"
#include <queue>
#include <iostream>
#include <execution>
#include <chrono>
#include <thread>

#include "../../include/Opti/OptiObject.hpp"
#include "ThreadPool.hpp"
#include "WaitGroup.hpp"

OptiRenderer::OptiRenderer(const std::vector<std::shared_ptr<OptiObject>> &children)
    : m_Children(children) {
}

void OptiRenderer::AddChild(const std::shared_ptr<OptiObject> &child) {
    m_Children.push_back(child);
}

void OptiRenderer::RemoveChild(std::shared_ptr<OptiObject> child) {
    m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child),
                     m_Children.end());
}

void OptiRenderer::AddChildren(
    const std::vector<std::shared_ptr<OptiObject>> &children) {
    m_Children.reserve(m_Children.size() + children.size());
    m_Children.insert(m_Children.end(), children.begin(), children.end());
}

void OptiRenderer::Update(ThreadPool& pool) {
    auto start = std::chrono::steady_clock::now();
    unsigned int numThreads = pool.size();
    if (numThreads > 12) {numThreads = 12;}

    // =========================
    // 1. Filter visible objects
    // =========================
    size_t total = m_Children.size();
    size_t chunkSize = (total + numThreads - 1) / numThreads;

    std::vector<std::vector<OptiObject*>> localCopy(numThreads);

    WaitGroup wg;

    auto t1 = std::chrono::steady_clock::now();
    for (unsigned int t = 0; t < numThreads; ++t) {
        size_t start = t * chunkSize;
        size_t end = std::min(start + chunkSize, total);
        if (start >= end) break;

        wg.Add(1);

        pool.Enqueue([this, start, end, t, &localCopy, &wg]() {
            try {
                for (size_t i = start; i < end; ++i) {
                    if (m_Children[i]->m_Visible) {
                        localCopy[t].push_back(m_Children[i].get());
                    }
                }
            } catch (...) {}

            wg.Done();
        });
    }

    wg.Wait();

    current.clear();
    for (auto& vec : localCopy) {
        current.insert(current.end(), vec.begin(), vec.end());
    }

    auto t2 = std::chrono::steady_clock::now();
    // =========================
    // 2. BFS traversal
    // =========================
    numThreads = pool.size();
    if (numThreads > 6) {numThreads = 6;}
    std::vector<std::array<std::vector<OptiObject*>, 101>> localBuckets(numThreads);
    std::array<std::vector<OptiObject*>, 101> buckets;
    OptiObject* obj;

    while (!current.empty()) {
        // trying to do this serially instead
        /*
        obj = current.back();
        current.pop_back();
        if (!obj->m_Visible) {continue;}
        obj->CalData();
        buckets[static_cast<int>(obj->GetZIndex())].push_back(obj);
        for (auto& child : obj->m_Children) {
            current.push_back(child.get());
        }
        */

        std::vector<OptiObject*> currentSnapshot = current; // 🔥 FIXED
        size_t total = currentSnapshot.size();
        size_t chunkSize = (total + numThreads - 1) / numThreads;

        std::vector<std::vector<OptiObject*>> localNext(numThreads);
        WaitGroup wg;

        for (unsigned int t = 0; t < numThreads; ++t) {
            size_t start = t * chunkSize;
            size_t end = std::min(start + chunkSize, total);
            if (start >= end) break;

            wg.Add(1);

            pool.Enqueue([start, end, t,
                          &currentSnapshot,
                          &localNext,
                          &localBuckets,
                          &wg]() {
                try {
                    for (size_t i = start; i < end; ++i) {
                        auto* curr = currentSnapshot[i];
                        if (!curr->m_Visible) continue;

                        curr->CalData();

                        int z = (int)curr->GetZIndex();
                        localBuckets[t][z].push_back(curr);

                        for (const auto& child : curr->GetChildren()) {
                            localNext[t].push_back(child.get());
                        }
                    }
                } catch (...) {}

                wg.Done();
            });
        }

        wg.Wait();

        current.clear();
        for (auto& vec : localNext) {
            current.insert(current.end(), vec.begin(), vec.end());
        }
    }

    auto t3 = std::chrono::steady_clock::now();
    // =========================
    // 3. Merge buckets
    // =========================
    for (int z = 0; z < 101; ++z) {
        buckets[z].clear();
        for (unsigned int t = 0; t < numThreads; ++t) {
            auto& local = localBuckets[t][z];
            buckets[z].insert(buckets[z].end(), local.begin(), local.end());
        }
    }

    auto t4 = std::chrono::steady_clock::now();
    // =========================
    // 4. Rendering (unchanged)
    // =========================
    for (int i = 0; i < 101; i++) {
        if (buckets[i].empty()) continue;

        commands.clear();

        for (const auto& obj : buckets[i]) {
            obj->Draw();
        }

        std::sort(commands.begin(), commands.end(),
        [](const DrawCommand& a, const DrawCommand& b) {
            return a.image->m_Texture < b.image->m_Texture;
        });

        Core::Texture* currentTexture = nullptr;

        for (auto& cmd : commands) {
            auto* img = cmd.image;
            img->m_UniformBuffer->SetData(0, cmd.data);

            if (img->m_Texture.get() != currentTexture) {
                currentTexture = img->m_Texture.get();
                currentTexture->Bind(0);
            }

            OptiImage::s_VertexArray->DrawTriangles();
        }
    }

    for (int i = 0; i < 101; i++) {
        buckets[i].clear();
    }
    auto end = std::chrono::steady_clock::now();
    auto prepDuration = std::chrono::duration_cast<std::chrono::microseconds>(t1-start);
    auto filterDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1);
    auto BFSDuration = std::chrono::duration_cast<std::chrono::microseconds>(t3-t2);
    auto MergeDuration = std::chrono::duration_cast<std::chrono::microseconds>(t4-t3);
    auto DrawDuration = std::chrono::duration_cast<std::chrono::microseconds>(end-t4);
    std::cout << "( " << prepDuration.count();
    std::cout << " " << filterDuration.count();
    std::cout << " " << MergeDuration.count();
    std::cout << " " << BFSDuration.count();
    std::cout << " " << DrawDuration.count();
    std::cout << " )" << std::endl;
}


void OptiRenderer::Submit(OptiImage* img, const Core::Matrices& data) {
    commands.push_back({img, data});
}
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

void OptiRenderer::Update() {
    const unsigned int numThreads = std::thread::hardware_concurrency();

    // copy from m_Children to current, removing invisible objects on the way
    size_t chunkSize = (m_Children.size() + numThreads - 1) / numThreads;
    std::vector<std::thread> threads;
    std::vector<std::vector<OptiObject*>> localCopy(numThreads);
    for (unsigned int t = 0; t < numThreads; ++t) {
        size_t start = t * chunkSize;
        size_t end = std::min(start + chunkSize, m_Children.size());
        threads.emplace_back([&, start, end, t]() {
            for (size_t i = start; i < end; ++i) {
                if (m_Children[i]->m_Visible) {localCopy[t].push_back(m_Children[i].get());}
            }
        });
    }
    for (auto& th : threads) th.join();
    for (unsigned int t = 0; t < numThreads; ++t) {
        current.insert(current.end(), localCopy[t].begin(), localCopy[t].end());
    }

    // BFS through current with multithreading
    std::vector<std::array<std::vector<OptiObject*>, 101>> localBuckets(numThreads);
    while (!current.empty()) {
        size_t total = current.size();
        size_t chunkSize = (total + numThreads - 1) / numThreads;
        std::vector<std::thread> threads;
        // Each thread gets its own "next" list
        std::vector<std::vector<OptiObject*>> localNext(numThreads);
        for (unsigned int t = 0; t < numThreads; ++t) {
            size_t start = t * chunkSize;
            size_t end = std::min(start + chunkSize, total);
            if (start >= end) break;
            threads.emplace_back([&, t, start, end]() {
                for (size_t i = start; i < end; ++i) {
                    auto& curr = current[i];
                    if (!curr->m_Visible) continue;
                    int z = static_cast<int>(curr->GetZIndex());
                    localBuckets[t][z].push_back(curr);
                    for (const auto& child : curr->GetChildren()) {
                        localNext[t].push_back(child.get());
                    }
                }
            });
        }
        for (auto& th : threads) th.join();
        // Merge next level
        current.resize(0);
        for (auto& vec : localNext) {
            current.insert(current.end(), vec.begin(), vec.end());
        }
    }
    for (int z = 0; z < 101; ++z) {
        buckets[z].clear();
        for (unsigned int t = 0; t < numThreads; ++t) {
            auto& local = localBuckets[t][z];
            buckets[z].insert(buckets[z].end(), local.begin(), local.end());
        }
    }

    // drawing the OptiObjects
    for (int i=0; i<101; i++) {
        if (buckets[i].empty()) {continue;}

        commands.clear();
        for (const auto& obj : buckets[i]) {obj->Draw();}

        std::sort(commands.begin(), commands.end(),
        [](const DrawCommand& a, const DrawCommand& b) {
            return a.image->m_Texture < b.image->m_Texture;
        });

        Core::Texture* currentTexture = nullptr;

        // OptiImage::s_Program->Bind();          // ideally move out later
        // OptiImage::s_VertexArray->Bind();      // ideally move out later

        if (commands.empty()) {continue;}
        // commands[0].image->m_Texture->Bind(0);

        for (auto& cmd : commands) {
            auto* img = cmd.image;
            img->m_UniformBuffer->SetData(0, cmd.data);
            if (img->m_Texture.get() != currentTexture) {
                currentTexture = img->m_Texture.get();
                currentTexture->Bind(0);
            }
            // OptiImage::s_Program->Bind();          // ideally move out later
            // OptiImage::s_VertexArray->Bind();      // ideally move out later

            OptiImage::s_VertexArray->DrawTriangles();
        }
    }
    for (int i=0; i<101; i++) {buckets[i].clear();}
}

void OptiRenderer::Submit(OptiImage* img, const Core::Matrices& data) {
    commands.push_back({img, data});
}
//
// Created by joshb on 2026/4/1.
//
#include "ShapezRenderer.hpp"
#include <queue>
#include <iostream>
#include <execution>
#include <chrono>
#include "ShapezObject.hpp"

ShapezRenderer::ShapezRenderer(const std::vector<std::shared_ptr<ShapezObject>> &children)
    : m_Children(children) {}

void ShapezRenderer::AddChild(const std::shared_ptr<ShapezObject> &child) {
    m_Children.push_back(child);
}

void ShapezRenderer::RemoveChild(std::shared_ptr<ShapezObject> child) {
    m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child),
                     m_Children.end());
}

void ShapezRenderer::AddChildren(
    const std::vector<std::shared_ptr<ShapezObject>> &children) {
    m_Children.reserve(m_Children.size() + children.size());
    m_Children.insert(m_Children.end(), children.begin(), children.end());
}

void ShapezRenderer::Update() {
    auto start = std::chrono::steady_clock::now();
    std::cout << "shapez renderer measures ";

    for (int i=0; i<101; i++) {buckets[i].clear();}
    stack = m_Children;
    std::shared_ptr<ShapezObject> curr;
    while (!stack.empty()) {
        curr = stack.back();
        stack.pop_back();
        buckets[static_cast<int>(curr->GetZIndex())].push_back(curr);
        for (const auto &child : curr->GetChildren()) {
            stack.push_back(child);
        }
    }

    for (int i=0; i<101; i++) {
        if (buckets[i].empty()) {continue;}
        std::for_each(std::execution::par, buckets[i].begin(), buckets[i].end(),
        [](const auto& gameObject) {
            gameObject->Draw();
        });
    }

    for (int i=0; i<101; i++) {buckets[i].clear();}
    stack.clear();
}
//
// Created by joshb on 2026/4/1.
//
#include "../../include/Opti/OptiRenderer.hpp"
#include <queue>
#include <iostream>
#include <execution>
#include <chrono>
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
    for (int i=0; i<101; i++) {buckets[i].clear();}
    stack = m_Children;
    std::shared_ptr<OptiObject> curr;
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

        commands.clear();
        for (const auto& obj : buckets[i]) {obj->Draw();}

        std::sort(commands.begin(), commands.end(),
        [](const DrawCommand& a, const DrawCommand& b) {
            return a.image->m_Texture < b.image->m_Texture;
        });

        Core::Texture* currentTexture = nullptr;

        OptiImage::s_Program->Bind();          // ideally move out later
        OptiImage::s_VertexArray->Bind();      // ideally move out later

        for (auto& cmd : commands) {
            auto* img = cmd.image;

            if (img->m_Texture.get() != currentTexture) {
                currentTexture = img->m_Texture.get();
                currentTexture->Bind(0);
            }
            img->m_UniformBuffer->SetData(0, cmd.data);

            OptiImage::s_VertexArray->DrawTriangles();
        }
    }


    for (int i=0; i<101; i++) {buckets[i].clear();}
    stack.clear();
}

void OptiRenderer::Submit(OptiImage* img, const Core::Matrices& data) {
    commands.push_back({img, data});
}
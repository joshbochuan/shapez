//
// Created by joshb on 2026/4/2.
//

#include "../../include/Opti/OptiObject.hpp"
#include "Util/Transform.hpp"
#include "Util/TransformUtils.hpp"
#include "config.hpp"
#include "../../include/Opti/OptiRenderer.hpp"
#include <iostream>

class OptiRenderer;

constexpr glm::mat4 eye(1.F);
constexpr float nearClip = -100;
constexpr float farClip = 100;

auto projection =
    glm::ortho<float>(0.0F, 1.0F, 0.0F, 1.0F, nearClip, farClip);
auto view =
    glm::scale(eye, {1.F / WINDOW_WIDTH, 1.F / WINDOW_HEIGHT, 1.F}) *
    glm::translate(eye, {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0});
auto m_Projection = projection * view;

void OptiObject::CalData() {
    if (m_Drawable == nullptr) {return;}
    glm::mat4 model = glm::mat4(1.0f);  // identity
    model = glm::translate(model, glm::vec3(m_Transform.translation, m_ZIndex));
    model = glm::rotate(model, m_Transform.rotation, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(m_Transform.scale * m_Drawable->GetSize(), 1.0f));
    model = glm::translate(model, glm::vec3(-m_Pivot / m_Drawable->GetSize(), 0.0f));
    data = { model, m_Projection };
}

void OptiObject::Draw() {
    if (m_Drawable == nullptr) {return;}
    m_Drawable->Draw(data);
}

//
// Created by User on 2026/3/27.
//
#include "../../include/UIelement/Button.hpp"

#include "../../include/UIelement/Text.hpp"
#include "../../include/items/Color.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include <cmath>

Button::Button(glm::vec2 dimension, std::string path, std::string text, float size, Util::Color color): OptiObject() {
    this->text = std::make_shared<Text>(text, size, color);
    this->image = std::make_shared<Util::Image>(path);
    this->imagePressed = nullptr;
    this->dimension = dimension;
    SetDrawable(this->image);
    AddChild(this->text);
}

void Button::Update() {
    isClicked = (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)
                && std::abs(Util::Input::GetCursorPosition().x-m_Transform.translation.x)*2.0f < dimension.x
                && std::abs(Util::Input::GetCursorPosition().y-m_Transform.translation.y)*2.0f < dimension.y);
    if (isClicked) {
        isHeld = true;
        if (imagePressed != nullptr) {SetDrawable(imagePressed);}
    }

    isReleased = (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) && isHeld);
    if (isReleased) {
        isHeld = false;
        SetDrawable(this->image);
    }
    text->m_Transform.translation = m_Transform.translation;
}
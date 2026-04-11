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

#include "World.hpp"
using namespace World;

Button::Button(std::shared_ptr<Util::Image> idleImage): OptiObject() {
    this->dimension = idleImage->GetSize();
    this->idleImage = idleImage;
    SetDrawable(this->idleImage);
    this->image = std::make_shared<OptiObject>();
    this->lockedImage = std::make_shared<OptiObject>();
    AddChild(this->image);
    AddChild(this->lockedImage);
}

void Button::Update() {
    hovered = (std::abs(Util::Input::GetCursorPosition().x-m_Transform.translation.x)*2.0f < dimension.x * windowPercent
                && std::abs(Util::Input::GetCursorPosition().y-m_Transform.translation.y)*2.0f < dimension.y * windowPercent);

    if (locked) {
        clicked = false;
        released = false;
        selected = false;
        SetDrawable(idleImage);
        lockedImage->SetVisible(true);
        image->SetVisible(false);
    }
    else {
        lockedImage->SetVisible(false);
        image->SetVisible(true);

        clicked = (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) && hovered);
        for (auto& key : keys) {if (Util::Input::IsKeyDown(key)) {clicked = true;}}
        if (clicked) {held = true;}

        released = (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) && held);
        for (auto& key : keys) {if (Util::Input::IsKeyUp(key)) {released = true;}}
        if (released) {held = false;}

        if (released) {selected = !selected;}

        if (held) {scaleState += (heldScale-scaleState)/heldSpeed;}
        else if (selected) {scaleState += (selectScale-scaleState)/selectSpeed;}
        else if (hovered) {scaleState += (hoverScale-scaleState)/hoverSpeed;}
        else {scaleState += (hoverScale-scaleState)/hoverSpeed;}

        if (selected && (selectedImage!=nullptr)) {SetDrawable(selectedImage);}
        else if (held && (heldImage != nullptr)) {SetDrawable(heldImage);}
        else if (hovered && (hoveredImage != nullptr)) {SetDrawable(hoveredImage);}
        else {SetDrawable(idleImage);}
    }

    m_Transform.scale = {windowPercent * scaleState, windowPercent * scaleState};
    if (text != nullptr) {text->m_Transform = m_Transform;}
    if (this->image != nullptr) {
        this->image->m_Transform.translation = m_Transform.translation;
        this->image->m_Transform.scale = m_Transform.scale * imageScale;
    }
    if (this->lockedImage != nullptr) {
        this->lockedImage->m_Transform.translation = m_Transform.translation;
        this->lockedImage->m_Transform.scale = m_Transform.scale * lockedImageScale;
    }
}

void Button::SetImage(std::shared_ptr<Util::Image> image) {
    this->image->SetDrawable(image);
}
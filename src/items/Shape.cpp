//
// Created by User on 2026/3/20.
//

#include "../../include/items/Shape.hpp"
#include <stdexcept>
#include <string>

#include "Util/Image.hpp"
#include <iostream>

#include "config.hpp"

Shape::Shape(std::string code)
    : Item(ItemType::SHAPE) {
    this->code = code;
    int layer = 0, i = 0, r=0;
    while (i < code.length()) {
        if (code[i] == ':') {
            i++;
            layer++;
            r = 0;
            continue;
        }

        if (layer >= 4) {throw std::invalid_argument("Invalid layer count: " + code);}
        if (r >= 4) {throw std::invalid_argument("Invalid quad on one layer:" + code);}

        if (code[i] != '-') {
            this->quads.push_back(std::make_shared<Quad>(code.substr(i, 2), r, layer));
        }
        r++;
        i += 2;
    }

    for (int i=0; i<this->quads.size(); i++) {
        this->AddChild(this->quads[i]);
    }

    this->SetDrawable(shapeTexture);
    Item::SetZIndex(20);
}

void Shape::Update() {
    m_Visible = ((std::abs(m_Transform.translation.x) < WINDOW_WIDTH) && (std::abs(m_Transform.translation.y) < WINDOW_HEIGHT));
    for (int i=0; i<this->quads.size(); i++) {
        this->quads[i]->m_Transform.translation = this->m_Transform.translation;
        this->quads[i]->m_Transform.scale = this->m_Transform.scale;
        this->quads[i]->SetVisible(this->m_Visible);
    }
}

std::string Shape::getCode() {
    return this->code;
}

std::shared_ptr<Item> Shape::copy() {
    return std::make_shared<Shape>(this->code);
}

bool Shape::operator==(std::shared_ptr<Item> rhs) {
    if (rhs->getType() != ItemType::SHAPE) {return false;}
    std::shared_ptr<Shape> other = std::dynamic_pointer_cast<Shape>(rhs);
    return (this->getCode() == other->getCode());
}

void Shape::MachineItemZIndex(float index) {
    SetZIndex(index);
    for (int i=0; i<this->quads.size(); i++) {
        this->quads[i]->SetZIndex(index+i+1);
    }
}

void Shape::SetItemSize(glm::vec2 s) {
    m_Transform.scale.x = s.x * 0.25f;
    m_Transform.scale.y = s.y * 0.25f;
}

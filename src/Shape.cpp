//
// Created by User on 2026/3/20.
//

#include "Shape.hpp"
#include <stdexcept>
#include <string>

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

    // todo: renders the dark circle
    // this->SetDrawable(
    //     std::make_shared<Util::Image>("../Resources/shapes/circle.png"));
    this->SetZIndex(5);
}

void Shape::Update() {
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
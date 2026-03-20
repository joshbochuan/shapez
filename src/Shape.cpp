//
// Created by User on 2026/3/20.
//

#include "Shape.hpp"
#include <stdexcept>
#include <string>

Shape::Shape(std::string code, glm::vec2 pos)
    : Item(pos, ItemType::SHAPE) {
    this->pos = pos;

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
    //     std::make_shared<Util::Image>("../Resources/shapes/" + code + (string)layer + ".png"));
    this->SetZIndex(6);
    // this->Start();
}

void Shape::Update() {
    for (int i=0; i<this->quads.size(); i++) {
        this->quads[i]->m_Transform.translation.x = this->m_Transform.translation.x;
        this->quads[i]->m_Transform.translation.y = this->m_Transform.translation.y;
        this->quads[i]->Update();
    }
}
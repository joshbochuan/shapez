//
// Created by User on 2026/3/20.
//

#include "Quad.hpp"
#include <string>
#include <cmath>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <iostream>

Quad::Quad(std::string code, int r, int layer): Util::GameObject() {
    this->code = code;
    this->r = r;
    this->layer = layer;

    this->SetDrawable(quadTextures[code + std::to_string(layer)]);
    this->m_Transform.translation = glm::vec2({0, 0});
    this->m_Transform.rotation = M_PI*(-0.5)*(double)r;
    this->SetZIndex(21+ (layer*4) + r);
}
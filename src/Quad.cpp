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

    std::string path = "../Resources/shapes/" + code + std::to_string(layer) + ".png";

    this->SetDrawable(std::make_shared<Util::Image>(path));
    this->m_Transform.translation = glm::vec2({0, 0});
    this->m_Transform.rotation = M_PI*(-0.5)*(double)r;
    this->SetZIndex(5+layer);
    std::cout << "quad created with code " << this->code << " and layer " << this->layer << std::endl;
    std::cout << path << std::endl;
}

void Quad::Update() {

}
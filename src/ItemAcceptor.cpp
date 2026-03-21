//
// Created by joshb on 2026/3/20.
//

#include "ItemAcceptor.hpp"
#include "Global.hpp"

ItemAcceptor::ItemAcceptor(int x, int y, int r, float rate)
    : Machine(x, y, r, rate) {
    this->item = nullptr;
    this->progress = 0;
    this->takesColor = true;
    this->takesShape = true;
}

void ItemAcceptor::Update() {
    if (item == nullptr) {return;}
    if (progress < 1) {progress += rate;}

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale = cam.scale;
    int dx, dy;
    glm::vec2 p1, p2;
    switch (r) {
        case 0: dx = 0; dy = 1; break;
        case 1: dx = -1; dy = 0; break;
        case 2: dx = 0; dy = -1; break;
        case 3: dx = 1; dy = 0; break;
        default: throw std::invalid_argument("illegal miner rotation " + std::to_string(r));
    }

    if (item != nullptr) {
        item->m_Transform.translation.x = this->m_Transform.translation.x;
        p2 = m_Transform.translation;
        p1.x = p2.x - cam.scale.x * 96.0 * dx;
        p1.y = p2.y - cam.scale.y * 96.0 * dy;
        item->m_Transform.translation.x = std::round(p1.x * (1-progress) + p2.x * progress);
        item->m_Transform.translation.y = std::round(p1.y * (1-progress) + p2.y * progress);
        item->m_Transform.scale.x = 0.25 * cam.scale.x;
        item->m_Transform.scale.y = 0.25 * cam.scale.y;
        item->Update();
    }
}
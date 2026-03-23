//
// Created by joshb on 2026/3/20.
//
#include "ItemEjector.hpp"
#include "Global.hpp"
#include <iostream>

ItemEjector::ItemEjector(int x, int y, int r)
    : Machine(x, y, r, BELT_RATE) {
    this->item = nullptr;
    this->progress = 0;
    this->next = nullptr;
}

void ItemEjector::Init() {
    initialized = true;
    MapEjectors[{x, y, r}] = std::dynamic_pointer_cast<ItemEjector>(shared_from_this());
    std::tuple<int, int, int> key;
    switch (r) {
        case 0: key = {x, y+1, r}; break;
        case 1: key = {x-1, y, r}; break;
        case 2: key = {x, y-1, r}; break;
        case 3: key = {x+1, y, r}; break;
        default: throw std::invalid_argument("invalid ejector rotation " + std::to_string(r));
    }

    next = MapAcceptors[key];
    if (next == nullptr) {return;}
    if (next->item != nullptr) {next->RemoveChild(next->item);}
    next->item = nullptr;
    next->progress = 0;
    next->prev = std::dynamic_pointer_cast<ItemEjector>(shared_from_this());
}

void ItemEjector::Delete() {
    MapAcceptors.erase({x, y, r});
    if (next == nullptr) {return;}
    if (next->item != nullptr) {next->RemoveChild(next->item);}
    next->item = nullptr;
    next->progress = 0;
    next->prev = nullptr;
}

void ItemEjector::Update() {
    if (!initialized) {throw std::invalid_argument("ejector not initialized");}
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
        default: throw std::invalid_argument("illegal ejector rotation " + std::to_string(r));
    }

    if (item != nullptr) {
        item->m_Transform.translation.x = this->m_Transform.translation.x;
        p1 = m_Transform.translation;
        p2.x = p1.x + cam.scale.x * 96.0 * dx;
        p2.y = p1.y + cam.scale.y * 96.0 * dy;
        item->m_Transform.translation.x = std::round(p1.x * (1-progress) + p2.x * progress);
        item->m_Transform.translation.y = std::round(p1.y * (1-progress) + p2.y * progress);
        item->m_Transform.scale.x = 0.25 * cam.scale.x;
        item->m_Transform.scale.y = 0.25 * cam.scale.y;
        item->Update();
    }
}
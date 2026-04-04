//
// Created by joshb on 2026/3/20.
//
#include "buildings/ItemEjector.hpp"
#include "World.hpp"
#include <iostream>
using namespace World;

ItemEjector::ItemEjector(int x, int y, int r): rate(BELT_RATE){
    this->x = x;
    this->y = y;
    this->r = r;
    this->item = nullptr;
    this->progress = 0;
    this->next = nullptr;
    SetVisible(false);
    SetZIndex(10+(x+y)%2);
    m_Transform.rotation = 0.5f * M_PI * r;
}

void ItemEjector::Init() {
    initialized = true;
    MapEjectors[{x, y, r}] = std::dynamic_pointer_cast<ItemEjector>(shared_from_this());
    LstEjectors.push_back({shared_from_this()});
    std::tuple<int, int, int> key;
    switch (r) {
        case 0: key = {x, y+1, r}; break;
        case 1: key = {x-1, y, r}; break;
        case 2: key = {x, y-1, r}; break;
        case 3: key = {x+1, y, r}; break;
        default: throw std::invalid_argument("invalid ejector rotation " + std::to_string(r));
    }

    next = MapAcceptors[key];
    if (next == nullptr) {
        MapAcceptors.erase(key);
        return;
    }
    if (next->item != nullptr) {next->RemoveChild(next->item);}
    next->item = nullptr;
    next->progress = 0;
    next->prev = std::dynamic_pointer_cast<ItemEjector>(shared_from_this());
    SetVisible(true);
    next->SetVisible(true);
}

void ItemEjector::Delete() {
    MapEjectors.erase({x, y, r});
    LstEjectors.erase(std::remove(LstEjectors.begin(), LstEjectors.end(), shared_from_this()), LstEjectors.end());
    if (next == nullptr) {return;}
    if (next->item != nullptr) {next->RemoveChild(next->item);}
    next->item = nullptr;
    next->progress = 0;
    next->prev = nullptr;
    next->SetVisible(false);
}

void ItemEjector::Update() {
    if (!initialized) {throw std::invalid_argument("ejector not initialized");}

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale = cam.scale * 1.02f;

    if (item == nullptr) {return;}
    if (progress < 1) {progress += rate;}

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
        item->SetItemSize(cam.scale);
        item->Update();
    }

    // logic for clogged belt
    if (next == nullptr) {return;}
    if (next->item == nullptr) {return;}
    if (item == nullptr) {return;}
    if (progress > next->progress) {progress = next->progress;}
}

void ItemEjector::Transfer() {
    if (item == nullptr) {return;}
    if (progress < 1) {return;}

    if (next == nullptr) {return;};
    if (next->item != nullptr) {return;};
    if ((item->getType() == ItemType::COLOR) && (!next->takesColor)) {return;};
    if ((item->getType() == ItemType::SHAPE) && (!next->takesShape)) {return;};

    next->item = item;
    next->progress = progress - 1;
    next->AddChild(item);

    RemoveChild(item);
    item = nullptr;
    progress = 0;
}

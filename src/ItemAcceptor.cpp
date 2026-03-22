//
// Created by joshb on 2026/3/20.
//

#include "ItemAcceptor.hpp"
#include "Global.hpp"
#include <iostream>

ItemAcceptor::ItemAcceptor(int x, int y, int r)
    : Machine(x, y, r, BELT_RATE) {
    this->item = nullptr;
    this->progress = 0;
    this->takesColor = true;
    this->takesShape = true;

    /*
    acceptors[std::make_tuple(x, y, r)] = static_cast<std::shared_ptr<ItemAcceptor>>(this);
    std::tuple<int, int, int> key;
    switch (r) {
        case 0: key = std::make_tuple(x, y+1, r); break;
        case 1: key = std::make_tuple(x-1, y, r); break;
        case 2: key = std::make_tuple(x, y-1, r); break;
        case 3: key = std::make_tuple(x+1, y, r); break;
        default: throw std::invalid_argument("invalid acceptor rotation " + std::to_string(r));
    }


    if (ejectors[key] != nullptr) {
        next = ejectors[key];
        std::shared_ptr<ItemEjector> ejector = next.lock();
        if (ejector != nullptr) {
            ejector->RemoveChild(ejector->item);
            ejector->item = nullptr;
            ejector->progress = 0;
            ejector->prev = static_cast<std::shared_ptr<ItemAcceptor>>(this);
        }

    }
    */
}

/*
ItemAcceptor::~ItemAcceptor() {
    std::shared_ptr<ItemEjector> ejector = next.lock();
    if (ejector == nullptr) {return;}
    if (ejector->item != nullptr) {ejector->RemoveChild(ejector->item);}
    ejector->item = nullptr;
    ejector->progress = 1;
}
*/

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
        default: throw std::invalid_argument("illegal acceptor rotation " + std::to_string(r));
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
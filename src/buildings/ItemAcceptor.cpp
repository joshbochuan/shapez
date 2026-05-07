//
// Created by joshb on 2026/3/20.
//

#include "buildings/ItemAcceptor.hpp"
#include "World.hpp"
#include <iostream>
using namespace World;

ItemAcceptor::ItemAcceptor(int x, int y, int r) : rate(BELT_RATE) {
    this->x = x;
    this->y = y;
    this->r = r;
    this->item = nullptr;
    this->progress = 0;
    this->takesColor = true;
    this->takesShape = true;
    this->prev = nullptr;
    SetZIndex(10+(x+y)%2);
    m_Transform.rotation = 0.5f * M_PI * r;
}

std::string ItemAcceptor::getSaveString() {
    std::string res = "ACCEPTOR ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    if (item == nullptr) {res += "NULL ";}
    else {res += item->getCode() + " ";}
    res += std::to_string(progress);
    return res;
}

void ItemAcceptor::Init() {
    initialized = true;
    MapAcceptors[{x, y, r}] = std::dynamic_pointer_cast<ItemAcceptor>(shared_from_this());
    std::tuple<int, int, int> key;
    switch (r) {
        case 0: key = {x, y-1, r}; break;
        case 1: key = {x+1, y, r}; break;
        case 2: key = {x, y+1, r}; break;
        case 3: key = {x-1, y, r}; break;
        default: throw std::invalid_argument("invalid acceptor rotation " + std::to_string(r));
    }

    prev = MapEjectors[key];
    if (prev == nullptr) {
        MapEjectors.erase(key);
        return;
    }
    prev->RemoveItem();
    prev->progress = 0;
    prev->next = std::dynamic_pointer_cast<ItemAcceptor>(shared_from_this());
    // SetVisible(true);
    // prev->SetVisible(true);
}

void ItemAcceptor::Delete() {
    MapAcceptors.erase({x, y, r});
    if (prev == nullptr) {return;}
    m_Children.clear();
    prev->RemoveItem();
    prev->progress = 0;
    prev->next = nullptr;
}

void ItemAcceptor::Update() {
    if (!initialized) {throw std::invalid_argument("acceptor not initialized");}
    restored = false;

    this->m_Transform.translation.x = (((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = (((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale = cam.scale * 1.01f;

    if (item == nullptr) {return;}
    if (progress <= 1) {progress += rate * MULTIPLIER_BELT;}
}

bool ItemAcceptor::CheckConflict() {
    return ((item != nullptr) && (prep != nullptr));
}

void ItemAcceptor::StartRestore() {
    Restore(1);
}

void ItemAcceptor::Restore(int arg) {
    std::cout << "called acceptor restore of arg " << arg << std::endl;
    if ((item == nullptr) && (prep == nullptr)) {return;}
    if (restored) {return;}
    restored = true;
    if (prev == nullptr) {return;}
    prev->Restore(arg);
    if (arg && (prep != nullptr)) {
        prev->SetItem(prep);
        prep = nullptr;
    }
}

void ItemAcceptor::Promote() {
    if ((item != nullptr) && (prep != nullptr)) {throw std::invalid_argument("ItemAcceptor cannot promote because its not properly restored");}
    if (prep != nullptr) {
        SetItem(prep);
        prep = nullptr;
        progress = prepProgress-1;
    }

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
        p2 = m_Transform.translation;
        p1.x = p2.x - cam.scale.x * 96.0 * dx;
        p1.y = p2.y - cam.scale.y * 96.0 * dy;
        item->m_Transform.translation.x = (p1.x * (1-progress) + p2.x * progress);
        item->m_Transform.translation.y = (p1.y * (1-progress) + p2.y * progress);
        item->SetItemSize(cam.scale);
        item->Update();
    }
}

void ItemAcceptor::SetItem(std::shared_ptr<Item> item) {
    if (item == nullptr) {throw std::invalid_argument("item cannot be set to a nullptr");}
    this->item = item;
    AddChild(item);
}

void ItemAcceptor::RemoveItem() {
    this->item = nullptr;
    m_Children.clear();
}

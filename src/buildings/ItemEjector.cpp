//
// Created by joshb on 2026/3/20.
//
#include "buildings/ItemEjector.hpp"
#include "World.hpp"
#include <iostream>
using namespace World;

ItemEjector::ItemEjector(int x, int y, int r, std::shared_ptr<Machine> master): rate(BELT_RATE){
    this->x = x;
    this->y = y;
    this->r = r;
    this->item = nullptr;
    this->progress = 0;
    this->next = nullptr;
    this->master = master;
    SetZIndex(10+(x+y)%2);
    m_Transform.rotation = 0.5f * M_PI * r;
}

std::string ItemEjector::getSaveString() {
    std::string res = "EJECTOR ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    if (item == nullptr) {res += "NULL ";}
    else {res += item->getCode() + " ";}
    res += std::to_string(progress);
    return res;
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
    next->RemoveItem();
    next->progress = 0;
    next->prev = std::dynamic_pointer_cast<ItemEjector>(shared_from_this());
    // SetVisible(true);
    // next->SetVisible(true);
}

void ItemEjector::Delete() {
    MapEjectors.erase({x, y, r});
    LstEjectors.erase(std::remove(LstEjectors.begin(), LstEjectors.end(), shared_from_this()), LstEjectors.end());
    master = nullptr;
    if (next == nullptr) {return;}
    m_Children.clear();
    next->RemoveItem();
    next->progress = 0;
    next->prev = nullptr;
}

void ItemEjector::Update() {
    if (!initialized) {throw std::invalid_argument("ejector not initialized");}
    restored = false;

    this->m_Transform.translation.x = (((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = (((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale = cam.scale * 1.01f;

    if (item == nullptr) {return;}
    if (progress <= 1) {progress += rate * MULTIPLIER_BELT;}

    Transfer();

    if (progress > 1) {progress = 1;}
}

bool ItemEjector::CheckConflict() {
    if (item == nullptr) {return false;}
    if (prep != nullptr) {return true;}
    if ((progress >= 1) && (next == nullptr)) {return true;}
    return false;
}

void ItemEjector::StartRestore() {
    // std::cout << "start ejector restore at " << x << " " << y << std::endl;
    Restore(1);
}

void ItemEjector::Restore(int arg) {
    if (restored) {return;}
    restored = true;

    float targetProgress = progress;

    if ((next != nullptr) && (next->item != nullptr)) {targetProgress = next->progress;}
    if (progress > targetProgress) {progress = targetProgress;}
    master->Restore(arg, shared_from_this());
}

void ItemEjector::Promote() {
    /*
    if ((item != nullptr) && (prep != nullptr)) {
        throw std::invalid_argument(
        "ItemEjector at "
            + std::to_string(x)
            + " " + std::to_string(y)
            + " cannot promote: "
            + prep->getCode()
            + ", " + item->getCode()
            + ", " + std::to_string(prepProgress)
            + ", " + std::to_string(progress));
    }
    */
    if (prep != nullptr) {
        if (item != nullptr) {RemoveItem();}
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
        default: throw std::invalid_argument("illegal ejector rotation " + std::to_string(r));
    }

    if (item != nullptr) {
        p1 = m_Transform.translation;
        p2.x = p1.x + cam.scale.x * 96.0 * dx;
        p2.y = p1.y + cam.scale.y * 96.0 * dy;
        item->m_Transform.translation.x = (p1.x * (1-progress) + p2.x * progress);
        item->m_Transform.translation.y = (p1.y * (1-progress) + p2.y * progress);
        item->SetItemSize(cam.scale);
        item->Update();
    }
}

void ItemEjector::Transfer() {
    if (item == nullptr) {return;}
    if (progress <= 1) {return;}

    if (next == nullptr) {
        progress = 1;
        return;
    };
    if ((item->getType() == ItemType::COLOR) && (!next->takesColor)) {
        progress = 1;
        return;
    };
    if ((item->getType() == ItemType::SHAPE) && (!next->takesShape)) {
        progress = 1;
        return;
    };

    next->prep = item;
    next->prepProgress = progress;
    RemoveItem();
}

void ItemEjector::SetItem(std::shared_ptr<Item> item) {
    if (item == nullptr) {throw std::invalid_argument("item cannot be set to a nullptr");}
    if (this->item != nullptr) {throw std::invalid_argument("ItemEjector::SetItem called when there's already an item");}
    this->item = item;
    AddChild(item);
}

void ItemEjector::RemoveItem() {
    this->item = nullptr;
    m_Children.clear();
}
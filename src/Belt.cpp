//
// Created by User on 2026/3/20.
//

#include <cmath>
#include <iostream>
#include "Belt.hpp"
#include "Global.hpp"

// In Belt.cpp, before the constructor
static std::vector<std::string> getBeltSprites(BeltType type) {
    std::string dir;
    switch (type) {
        case BeltType::LEFT:  dir = "left";    break;
        case BeltType::RIGHT: dir = "right";   break;
        default:              dir = "forward";  break;
    }

    std::vector<std::string> sprites;
    for (int i = 0; i <= 13; i++) {
        sprites.push_back("../Resources/sprites/belt/built/" + dir + "_" + std::to_string(i) + ".png");
    }
    return sprites;
}

Belt::Belt(int x, int y, int r, BeltType type)
    : Machine(x, y, r, BELT_RATE), m_Animation(std::make_shared<Util::Animation>(
        getBeltSprites(type),
        true, 15, true, 0)) {
    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetZIndex(10 + (x+y)%2);
    SetDrawable(m_Animation);
    this->type = type;
    this->acceptor = std::make_shared<ItemAcceptor>(x, y, r);
    acceptors[std::make_tuple(x, y, r)] = this->acceptor;
    this->AddChild(this->acceptor);
    if (type == BeltType::LEFT)  { r = (r + 1) % 4; }
    if (type == BeltType::RIGHT) { r = (r - 1) % 4; }
    this->ejector = std::make_shared<ItemEjector>(x, y, r);
    ejectors[std::make_tuple(x, y, r)] = this->ejector;
    this->AddChild(this->ejector);
    if (type != BeltType::FORWARD) {
        this->acceptor->rate = BELTTURN_RATE;
        this->ejector->rate = BELTTURN_RATE;
    }
}

void Belt::Update() {
    // add progress to both accept progress and eject progress
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.02; // adding tiny size to avoid gap
    this->m_Transform.scale.y = cam.scale.y * 1.02;

    // if item can be transferred from input slot to output slot
    if ((acceptor->item != nullptr) && (acceptor->progress >= 1) && (ejector->item == nullptr)) {
        ejector->AddChild(acceptor->item);
        ejector->item = acceptor->item;
        ejector->progress = acceptor->progress-1;
        acceptor->RemoveChild(acceptor->item);
        acceptor->item = nullptr;
        acceptor->progress = 0;
    }

    acceptor->Update();
    ejector->Update();


    if (type == BeltType::LEFT) {
        float cx, cy, radian;
        switch (r) {
            case 0: cx = -1; cy = -1; break;
            case 1: cx = 1; cy = -1; break;
            case 2: cx = 1; cy = 1; break;
            case 3: cx = -1; cy = 1; break;
            default: throw std::invalid_argument("illegal belt rotation " + std::to_string(r));
        }
        cx = m_Transform.translation.x + cam.scale.x * 96.0 * cx;
        cy = m_Transform.translation.y + cam.scale.y * 96.0 * cy;
        if (acceptor->item != nullptr) {
            // radian_t = 90deg*r + 45deg * progress
            radian = M_PI * (0.5) * (r + 0.5 * acceptor->progress);
            acceptor->item->m_Transform.translation.x = std::round(cx + cam.scale.x * 96.0 * std::cos(radian));
            acceptor->item->m_Transform.translation.y = std::round(cy + cam.scale.y * 96.0 * std::sin(radian));
        }
        if (ejector->item != nullptr) {
            // radian = 45deg + 90deg*r + 45deg * progress
            radian = M_PI * (0.5) * (r+0.5 + 0.5 * ejector->progress);
            ejector->item->m_Transform.translation.x = std::round(cx + cam.scale.x * 96.0 * std::cos(radian));
            ejector->item->m_Transform.translation.y = std::round(cy + cam.scale.y * 96.0 * std::sin(radian));
        }
    }

    if (type == BeltType::RIGHT) {
        float cx, cy, radian;
        switch (r) {
            case 0: cx = 1; cy = -1; break;
            case 1: cx = 1; cy = 1; break;
            case 2: cx = -1; cy = 1; break;
            case 3: cx = -1; cy = -1; break;
            default: throw std::invalid_argument("illegal belt rotation " + std::to_string(r));
        }
        cx = m_Transform.translation.x + cam.scale.x * 96.0 * cx;
        cy = m_Transform.translation.y + cam.scale.y * 96.0 * cy;
        if (acceptor->item != nullptr) {
            // radian_t = 180deg + 90deg*r - 45deg * progress
            radian = M_PI * (0.5) * (r+2 - 0.5 * acceptor->progress);
            acceptor->item->m_Transform.translation.x = std::round(cx + cam.scale.x * 96.0 * std::cos(radian));
            acceptor->item->m_Transform.translation.y = std::round(cy + cam.scale.y * 96.0 * std::sin(radian));
        }
        if (ejector->item != nullptr) {
            // radian = 135deg + 90deg*r + 45deg * progress
            radian = M_PI * (0.5) * (r+1.5 - 0.5 * ejector->progress);
            ejector->item->m_Transform.translation.x = std::round(cx + cam.scale.x * 96.0 * std::cos(radian));
            ejector->item->m_Transform.translation.y = std::round(cy + cam.scale.y * 96.0 * std::sin(radian));
        }
    }

    if (acceptor->item != nullptr) {
        acceptor->item->Update();
    }
    if (ejector->item != nullptr) {
        ejector->item->Update();
    }
}
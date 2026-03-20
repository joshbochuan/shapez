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

Belt::Belt(int x, int y, int r, float rate, BeltType type)
    : Machine(x, y, r, rate), m_Animation(std::make_shared<Util::Animation>(
        getBeltSprites(type),
        true, 30, true, 0)) {
    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    SetDrawable(m_Animation);
    this->type = type;
    this->acceptor = std::make_shared<ItemAcceptor>(x, y, r, rate);
    if (type == BeltType::LEFT)  { r = (r - 1) % 4; }
    if (type == BeltType::RIGHT) { r = (r + 1) % 4; }
    this->ejector = std::make_shared<ItemEjector>(x, y, r, rate);
}

void Belt::Update() {
    // add progress to both accept progress and eject progress
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale = cam.scale;

    // if item can be transferred from input slot to output slot
    if ((acceptor->item != nullptr) && (acceptor->progress >= 1) && (ejector->item == nullptr)) {
        ejector->item = acceptor->item;
        acceptor->item = nullptr;
        ejector->progress = acceptor->progress-1;
        acceptor->progress = 0;
    }

    acceptor->Update();
    ejector->Update();

    // getting items on belts in place
    if (type == BeltType::FORWARD) {
        int dx, dy;
        glm::vec2 p1, p2;
        switch (r) {
            case 0: dx = 0; dy = 1; break;
            case 1: dx = -1; dy = 0; break;
            case 2: dx = 0; dy = -1; break;
            case 3: dx = 1; dy = 0; break;
            default: throw std::invalid_argument("illegal belt rotation " + std::to_string(r));
        }

        if (acceptor->item != nullptr) {
            acceptor->item->m_Transform.translation.x = this->m_Transform.translation.x;
            p2 = m_Transform.translation;
            p1.x = p2.x - cam.scale.x * 96.0 * dx;
            p1.y = p2.y - cam.scale.y * 96.0 * dy;
            acceptor->item->m_Transform.translation.x = std::round(p1.x * (1-acceptor->progress) + p2.x * acceptor->progress);
            acceptor->item->m_Transform.translation.y = std::round(p1.y * (1-acceptor->progress) + p2.y * acceptor->progress);
            acceptor->item->Update();
        }

        if (ejector->item != nullptr) {
            ejector->item->m_Transform.translation.x = this->m_Transform.translation.x;
            p1 = m_Transform.translation;
            p2.x = p1.x + cam.scale.x * 96.0 * dx;
            p2.y = p1.y + cam.scale.y * 96.0 * dy;
            ejector->item->m_Transform.translation.x = std::round(p1.x * (1-ejector->progress) + p2.x * ejector->progress);
            ejector->item->m_Transform.translation.y = std::round(p1.y * (1-ejector->progress) + p2.y * ejector->progress);
            ejector->item->Update();
        }
    }

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
            acceptor->item->m_Transform.translation.x = cx + cam.scale.x * 96.0 * std::cos(radian);
            acceptor->item->m_Transform.translation.y = cy + cam.scale.y * 96.0 * std::sin(radian);
        }
        if (ejector->item != nullptr) {
            // radian = 45deg + 90deg*r + 45deg * progress
            radian = M_PI * (0.5) * (r+0.5 + 0.5 * ejector->progress);
            ejector->item->m_Transform.translation.x = cx + cam.scale.x * 96.0 * std::cos(radian);
            ejector->item->m_Transform.translation.y = cy + cam.scale.y * 96.0 * std::sin(radian);
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
            acceptor->item->m_Transform.translation.x = cx + cam.scale.x * 96.0 * std::cos(radian);
            acceptor->item->m_Transform.translation.y = cy + cam.scale.y * 96.0 * std::sin(radian);
        }
        if (ejector->item != nullptr) {
            // radian = 135deg + 90deg*r + 45deg * progress
            radian = M_PI * (0.5) * (r+1.5 - 0.5 * ejector->progress);
            ejector->item->m_Transform.translation.x = cx + cam.scale.x * 96.0 * std::cos(radian);
            ejector->item->m_Transform.translation.y = cy + cam.scale.y * 96.0 * std::sin(radian);
        }
    }

    if (acceptor->item != nullptr) {
        acceptor->item->m_Transform.scale.x = 0.3 * cam.scale.x;
        acceptor->item->m_Transform.scale.y = 0.3 * cam.scale.y;
        acceptor->item->Update();
    }
    if (ejector->item != nullptr) {
        ejector->item->m_Transform.scale.x = 0.3 * cam.scale.x;
        ejector->item->m_Transform.scale.y = 0.3 * cam.scale.y;
        ejector->item->Update();
    }
}
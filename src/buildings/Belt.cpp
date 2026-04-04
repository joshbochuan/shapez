//
// Created by User on 2026/3/20.
//

#include <cmath>
#include <iostream>
#include "buildings/Belt.hpp"
#include "World.hpp"
#include "Util/Time.hpp"
using namespace World;

Belt::Belt(int x, int y, int r, BeltType type)
    : Machine(x, y, r, BELT_RATE, MachineName::BELT) {
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }
    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetZIndex(10 + (x+y)%2);
    this->type = type;
    this->acceptor = std::make_shared<ItemAcceptor>(x, y, r);
    if (type == BeltType::LEFT)  { r = ((r + 1) % 4); }
    if (type == BeltType::RIGHT) { r = ((r - 1 + 4) % 4); }
    this->ejector = std::make_shared<ItemEjector>(x, y, r);
}

void Belt::Init() {
    MapMachines[{x, y}] = shared_from_this();
    acceptor->Init();;
    ejector->Init();;
    this->AddChild(this->acceptor);
    this->AddChild(this->ejector);
}

void Belt::Delete() {
    MapMachines.erase({x, y});
    acceptor->Delete();
    ejector->Delete();
    RemoveChild(acceptor);
    RemoveChild(ejector);
}

void Belt::Update() {
    int frame = static_cast<int>(std::fmod(Util::Time::GetElapsedTimeMs()*0.042f, 14.0f));
    switch (type) {
        case BeltType::FORWARD: SetDrawable(beltForwardTexture[frame]); break;
        case BeltType::LEFT: SetDrawable(beltLeftTexture[frame]); break;
        case BeltType::RIGHT: SetDrawable(beltRightTexture[frame]); break;
        default: throw std::invalid_argument("illegal belt type");
    }

    // add progress to both accept progress and eject progress
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.01f; // adding tiny size to avoid gap
    this->m_Transform.scale.y = cam.scale.y * 1.01f;
    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    // if item can be transferred from input slot to output slot
    if ((type == BeltType::FORWARD)
        && (acceptor->item != nullptr)
        && (acceptor->progress >= 1)
        && (ejector->item == nullptr)) {
        ejector->AddChild(acceptor->item);
        ejector->item = acceptor->item;
        ejector->progress = acceptor->progress-1;
        acceptor->RemoveChild(acceptor->item);
        acceptor->item = nullptr;
        acceptor->progress = 0;
    }

    // instead of acceptor going from 0 to 0.25pi
    // it goes from 0 to 0.75 to avoid irrational number shenanigans
    if ((type != BeltType::FORWARD)
        && (acceptor->item != nullptr)
        && (acceptor->progress >= 0.75)
        && (ejector->item == nullptr)
        && ((ejector->next == nullptr)
            || (ejector->next->item == nullptr)
            || (ejector->next->progress >= 0.25))) {
        ejector->AddChild(acceptor->item);
        ejector->item = acceptor->item;
        ejector->progress = acceptor->progress - 0.5;
        acceptor->RemoveChild(acceptor->item);
        acceptor->item = nullptr;
        acceptor->progress = 0;
    }

    acceptor->Update();
    ejector->Update();

    if ((type != BeltType::FORWARD)
        && (ejector->next != nullptr)
        && (ejector->next->item != nullptr)
        && (acceptor->progress > ejector->next->progress + 0.5)) {
        acceptor->progress = ejector->next->progress + 0.5;
    }

    if ((type == BeltType::FORWARD)
        && (ejector->item != nullptr)
        && (acceptor->progress > ejector->progress)) {
        acceptor->progress = ejector->progress;
    }

    // p + 0.5*M_PI - 1
    if ((type != BeltType::FORWARD)
        && (ejector->item != nullptr)
        && (acceptor->progress > ejector->progress + 1.5 - 2)) {
        acceptor->progress = ejector->progress + 1.5 - 2;
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
            radian = M_PI * (0.5) * (r + 0) + (acceptor->progress * M_PI/3.0);
            acceptor->item->m_Transform.translation.x = std::round(cx + cam.scale.x * 96.0 * std::cos(radian));
            acceptor->item->m_Transform.translation.y = std::round(cy + cam.scale.y * 96.0 * std::sin(radian));
        }
        if (ejector->item != nullptr) {
            // radian = 45deg + 90deg*r + 45deg * progress
            radian = M_PI * (0.5) * (r + 1) + ((M_PI/3.0)*(ejector->progress-1)+1) - 1;
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
            radian = M_PI * (0.5) * (r+2) - (acceptor->progress * M_PI/3.0);
            acceptor->item->m_Transform.translation.x = std::round(cx + cam.scale.x * 96.0 * std::cos(radian));
            acceptor->item->m_Transform.translation.y = std::round(cy + cam.scale.y * 96.0 * std::sin(radian));
        }
        if (ejector->item != nullptr) {
            // radian = 135deg + 90deg*r + 45deg * progress
            radian = M_PI * (0.5) * (r+1) - ((M_PI/3.0)*(ejector->progress-1)+1) + 1;
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


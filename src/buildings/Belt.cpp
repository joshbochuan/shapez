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
}

std::string Belt::getSaveString() {
    std::string res = "BELT ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    switch (type) {
        case BeltType::FORWARD: res += "FORWARD"; break;
        case BeltType::LEFT: res += "LEFT"; break;
        case BeltType::RIGHT: res += "RIGHT"; break;
    }
    return res;
}

std::shared_ptr<Machine> Belt::fromSaveString(std::vector<std::string> prop) {
    int x = std::stoi(prop[1]);
    int y = std::stoi(prop[2]);
    int r = std::stoi(prop[3]);
    BeltType type = BeltType::FORWARD;
    if (prop[4] == "LEFT") {type = BeltType::LEFT;}
    else if (prop[4] == "RIGHT") {type = BeltType::RIGHT;}
    return std::make_shared<Belt>(x, y, r, type);
}

void Belt::Init() {
    int rotated = r;
    this->acceptor = std::make_shared<ItemAcceptor>(x, y, r);
    if (type == BeltType::LEFT)  { rotated = ((r + 1) % 4); }
    if (type == BeltType::RIGHT) { rotated = ((r + 3) % 4); }
    this->ejector = std::make_shared<ItemEjector>(x, y, rotated, shared_from_this());

    MapMachines[{x, y}] = shared_from_this();
    acceptor->Init();;
    ejector->Init();;
    this->AddChild(this->acceptor);
    this->AddChild(this->ejector);
    BELT_COUNT++;
}

void Belt::Delete() {
    MapMachines.erase({x, y});
    RemoveChild(acceptor);
    RemoveChild(ejector);
    acceptor->Delete();
    ejector->Delete();
    acceptor = nullptr;
    ejector = nullptr;
    BELT_COUNT--;
}

void Belt::Update() {
    restored = false;

    int frame = static_cast<int>(std::fmod(Util::Time::GetElapsedTimeMs()*0.042f*MULTIPLIER_BELT, 14.0f));
    switch (type) {
        case BeltType::FORWARD: SetDrawable(beltForwardTexture[frame]); break;
        case BeltType::LEFT: SetDrawable(beltLeftTexture[frame]); break;
        case BeltType::RIGHT: SetDrawable(beltRightTexture[frame]); break;
        default: throw std::invalid_argument("illegal belt type");
    }

    // add progress to both accept progress and eject progress
    this->m_Transform.translation.x = (((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = (((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.01f; // adding tiny size to avoid gap
    this->m_Transform.scale.y = cam.scale.y * 1.01f;
    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    acceptor->Update();
    ejector->Update();

    // if item can be transferred from input slot to output slot
    if ((type == BeltType::FORWARD)
        && (acceptor->item != nullptr)
        && (acceptor->progress > 1)) {
        ejector->prep = acceptor->item;
        ejector->prepProgress = acceptor->progress;
        acceptor->RemoveItem();
    }

    // instead of acceptor going from 0 to 0.25pi
    // it goes from 0 to 0.75 to avoid irrational number shenanigans
    if ((type != BeltType::FORWARD)
        && (acceptor->item != nullptr)
        && (acceptor->progress > 0.75)) {
        ejector->prep = acceptor->item;
        ejector->prepProgress = acceptor->progress + 0.5f;
        acceptor->RemoveItem();
    }
}

void Belt::Restore(int arg, std::shared_ptr<ItemEjector> from) {
    if (restored) {return;}
    restored = true;

    // removed ejector->item == nullptr
    if ((type == BeltType::FORWARD)
        && (ejector->prep == nullptr)
        && ((acceptor->item == nullptr) || (ejector->item == nullptr))) {
        return;
    }

    if ((arg == 0) && (type != BeltType::FORWARD) && (ejector->item == nullptr)) {return;}
    if ((arg == 1) && (type == BeltType::FORWARD) && (ejector->prep == nullptr)) {return;}

    float targetProgress = ejector->progress;
    if (type != BeltType::FORWARD) {targetProgress = ejector->progress-0.5f;}

    bool progressCorrected = false;
    if ((acceptor->item != nullptr) && (acceptor->progress >= targetProgress)) {
        acceptor->progress = targetProgress;
        progressCorrected = true;
    }

    if ((arg == 1) && (type == BeltType::FORWARD)) {
        acceptor->progress = 1;
        acceptor->Restore(1);
        if (ejector->prep != nullptr) {acceptor->SetItem(ejector->prep);}
        ejector->prep = nullptr;
    }
    else if ((arg == 0) && (type != BeltType::FORWARD) && (ejector->progress <= 0.5f)) {
        acceptor->progress = 1;
        acceptor->Restore(1);
    }
    else if (progressCorrected) {
        acceptor->Restore(0);
    }
}

void Belt::Promote() {
    acceptor->Promote();
    ejector->Promote();

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

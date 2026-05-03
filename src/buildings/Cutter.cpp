//
// Created by joshb on 2026/3/21.
//
#include "Util/Image.hpp"
#include "buildings/Cutter.hpp"
#include "World.hpp"
#include "../../include/items/Shape.hpp"
#include <iostream>
using namespace World;

Cutter::Cutter(int x, int y, int r)
    : Machine(x, y, r, CUT_RATE, MachineName::CUTTER) {
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }
    std::shared_ptr<Machine> tmp;
    switch (r) {
        case 0: tmp = MapMachines[{x+1, y}]; break;
        case 1: tmp = MapMachines[{x, y+1}]; break;
        case 2: tmp = MapMachines[{x-1, y}]; break;
        case 3: tmp = MapMachines[{x, y-1}]; break;
        default: throw std::invalid_argument("invalid balancer rotation " + std::to_string(r));
    }
    if (tmp != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }

    this->cooldown = 0;
    this->acceptor = std::make_shared<ItemAcceptor>(this->x, this->y, this->r);
    this->acceptor->takesColor = false;
    this->ejectorA = std::make_shared<ItemEjector>(this->x, this->y, this->r);
    switch (r) {
        case 0: this->ejectorB = std::make_shared<ItemEjector>(this->x+1, this->y, this->r); break;
        case 1: this->ejectorB = std::make_shared<ItemEjector>(this->x, this->y+1, this->r); break;
        case 2: this->ejectorB = std::make_shared<ItemEjector>(this->x-1, this->y, this->r); break;
        case 3: this->ejectorB = std::make_shared<ItemEjector>(this->x, this->y-1, this->r); break;
    }

    this->AddChild(acceptor);
    this->AddChild(ejectorA);
    this->AddChild(ejectorB);

    SetPivot({-84, 0});
    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetDrawable(cutterTexture);
    this->SetZIndex(40 + (x+y)%2);
}

std::string Cutter::getSaveString() {
    std::string res = "CUTTER ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    res += std::to_string(cooldown);
    return res;
}

std::shared_ptr<Machine> Cutter::fromSaveString(std::vector<std::string> prop) {
    int x = std::stoi(prop[1]);
    int y = std::stoi(prop[2]);
    int r = std::stoi(prop[3]);
    float cd = std::stof(prop[4]);
    auto res = std::make_shared<Cutter>(x, y, r);
    res->cooldown = cd;
    return res;
}

std::pair<std::shared_ptr<Shape>, std::shared_ptr<Shape>> Cut(const std::shared_ptr<Shape>& shape) {
    std::shared_ptr<Shape> left, right;
    std::string val, codeLeft, codeRight, orig = shape->getCode();
    int layer = std::count(orig.begin(), orig.end(), ':')+1;
    for (int i=0; i<layer; i++) {
        val = orig.substr(9*i+4, 4);
        if (val != "----") {
            if (!codeLeft.empty()) {codeLeft += ":";}
            codeLeft += "----" + val;
        }
        val = orig.substr(9*i, 4);
        if (val != "----") {
            if (!codeRight.empty()) {codeRight += ":";}
            codeRight += val + "----";
        }
    }
    left = std::make_shared<Shape>(codeLeft);
    right = std::make_shared<Shape>(codeRight);
    if (left->quads.empty()) {left = nullptr;}
    if (right->quads.empty()) {right = nullptr;}
    return std::make_pair(left, right);
}

void Cutter::Init() {
    MapMachines[{x, y}] = shared_from_this();
    MapMachines[{ejectorB->x, ejectorB->y}] = shared_from_this();
    acceptor->Init();
    ejectorA->Init();
    ejectorB->Init();
    MACHINE_COUNT++;
}

void Cutter::Delete() {
    MapMachines.erase({x, y});
    MapMachines.erase({ejectorB->x, ejectorB->y});
    acceptor->Delete();
    ejectorA->Delete();
    ejectorB->Delete();
    MACHINE_COUNT--;
}

void Cutter::Update() {
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;
    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*384 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*384 < WINDOW_HEIGHT>>1));

    cooldown += rate * MULTIPLIER_PROCESS;
    if ((cooldown >= 1)
        && (acceptor->item != nullptr)
        && (acceptor->progress >= 1)
        && (ejectorA->item == nullptr)
        && (ejectorB->item == nullptr)) {
        auto res = Cut(std::dynamic_pointer_cast<Shape>(acceptor->item));
        cooldown -= 1;
        if (res.first != nullptr) {
            ejectorA->item = res.first;
            ejectorA->AddChild(ejectorA->item);
            ejectorA->progress = acceptor->progress-1;
        }
        if (res.second != nullptr) {
            ejectorB->item = res.second;
            ejectorB->AddChild(ejectorB->item);
            ejectorB->progress = acceptor->progress-1;
        }
        acceptor->RemoveChild(acceptor->item);
        acceptor->item = nullptr;
        acceptor->progress = 0;
    }
    if (cooldown > 1) {cooldown = 1;}

    acceptor->Update();
    ejectorA->Update();
    ejectorB->Update();
}



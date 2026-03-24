//
// Created by joshb on 2026/3/21.
//
#include "Util/Image.hpp"
#include "Cutter.hpp"
#include "Global.hpp"
#include "Shape.hpp"
#include <iostream>

Cutter::Cutter(int x, int y, int r)
    : Machine(x, y, r, CUT_RATE, MachineName::CUTTER) {
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
    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetDrawable(std::make_shared<Util::Image>("../Resources/Sprites/buildings/cutter.png"));
    this->SetZIndex(40 + (x+y)%2);
}

std::pair<std::shared_ptr<Shape>, std::shared_ptr<Shape>> Cut(const std::shared_ptr<Shape>& shape) {
    std::shared_ptr<Shape> left, right;
    std::string val="", codeLeft="", codeRight="", orig = shape->getCode();
    int layer = std::count(orig.begin(), orig.end(), ':')+1;
    for (int i=0; i<layer; i++) {
        val = orig.substr(9*i+4, 4);
        if (val != "----") {
            if (codeLeft != "") {codeLeft += ":";}
            codeLeft += "----" + val;
        }
        val = orig.substr(9*i, 4);
        if (val != "----") {
            if (codeRight != "") {codeRight += ":";}
            codeRight += val + "----";
        }
    }
    left = std::make_shared<Shape>(codeLeft);
    right = std::make_shared<Shape>(codeRight);
    return std::make_pair(left, right);
}

void Cutter::Init() {
    MapMachines[{x, y}] = shared_from_this();
    MapMachines[{ejectorB->x, ejectorB->y}] = shared_from_this();
    acceptor->Init();
    ejectorA->Init();
    ejectorB->Init();
}

void Cutter::Delete() {
    MapMachines.erase({x, y});
    MapMachines.erase({ejectorB->x, ejectorB->y});
    acceptor->Delete();
    ejectorA->Delete();
    ejectorB->Delete();
}

void Cutter::Update() {
    switch (r) {
        case 0:
            this->m_Transform.translation.x = std::round(((192.0*(1+x)) - cam.translation.x) * cam.scale.x);
            this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
            break;
        case 1:
            this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
            this->m_Transform.translation.y = std::round(((192.0*(1+y)) - cam.translation.y) * cam.scale.y);
            break;
        case 2:
            this->m_Transform.translation.x = std::round(((192.0*(x)) - cam.translation.x) * cam.scale.x);
            this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
            break;
        case 3:
            this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
            this->m_Transform.translation.y = std::round(((192.0*(y)) - cam.translation.y) * cam.scale.y);
            break;
        default:
            throw std::invalid_argument("illegal cutter rotation" + std::to_string(r));
    }
    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;

    cooldown += rate;
    if ((cooldown >= 1)
        && (acceptor->item != nullptr)
        && (acceptor->progress >= 1)
        && (ejectorA->item == nullptr)
        && (ejectorB->item == nullptr)) {
        auto res = Cut(std::dynamic_pointer_cast<Shape>(acceptor->item));
        cooldown -= 1;
        ejectorA->item = res.first;
        ejectorB->item = res.second;
        ejectorA->AddChild(ejectorA->item);
        ejectorB->AddChild(ejectorB->item);
        ejectorA->progress = acceptor->progress-1;
        ejectorB->progress = acceptor->progress-1;
        acceptor->RemoveChild(acceptor->item);
        acceptor->item = nullptr;
        acceptor->progress = 0;
    }
    if (cooldown > 1) {cooldown = 1;}

    acceptor->Update();
    ejectorA->Update();
    ejectorB->Update();
}



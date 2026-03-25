//
// Created by joshb on 2026/3/25.
//
#include "Stacker.hpp"
#include "Global.hpp"
#include "Shape.hpp"
#include <iostream>

Stacker::Stacker(int x, int y, int r)
    : Machine(x, y, r, STACK_RATE, MachineName::STACKER){
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

    SetPivot({-84, 0});
    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetDrawable(stackerTexture);

    // cooking up a better X-index for 2-wide objects
    this->SetZIndex(60 + fmod((4.0f*x+y), 16.0f)/16.0f);

    acceptorA = std::make_shared<ItemAcceptor>(x, y, r);
    ejector = std::make_shared<ItemEjector>(x, y, r);
    switch (r) {
        case 0: acceptorB = std::make_shared<ItemAcceptor>(x+1, y, r); break;
        case 1: acceptorB = std::make_shared<ItemAcceptor>(x, y+1, r); break;
        case 2: acceptorB = std::make_shared<ItemAcceptor>(x-1, y, r); break;
        case 3: acceptorB = std::make_shared<ItemAcceptor>(x, y-1, r); break;
        default: throw std::invalid_argument("invalid stacker rotation");
    }
    acceptorA->takesColor = false;
    acceptorB->takesColor = false;
}

void Stacker::Init() {
    MapMachines[{x, y}] = shared_from_this();
    MapMachines[{acceptorB->x, acceptorB->y}] = shared_from_this();
    acceptorA->Init();
    acceptorB->Init();
    ejector->Init();
    AddChild(acceptorA);
    AddChild(acceptorB);
    AddChild(ejector);
}

void Stacker::Delete() {
    MapMachines.erase({x, y});
    MapMachines.erase({acceptorB->x, acceptorB->y});
    acceptorA->Delete();
    acceptorB->Delete();
    ejector->Delete();
    RemoveChild(acceptorA);
    RemoveChild(acceptorB);
    RemoveChild(ejector);
}

std::shared_ptr<Shape> Stack(std::shared_ptr<Shape> bottom, std::shared_ptr<Shape> top) {
    std::string b = "--";
    std::vector<std::vector<std::string>> bottomQuads, topQuads;
    for (int i=0; i<4; i++) {
        bottomQuads.push_back(std::vector<std::string>());
        topQuads.push_back(std::vector<std::string>());
        for (int j=0; j<4; j++) {
            bottomQuads.back().push_back("--");
            topQuads.back().push_back("--");
        }
    }

    std::string bottomCode = bottom->getCode(), topCode = top->getCode();
    for (int i=0; i<=bottomCode.length()/9; i++) {
        for (int j=0; j<4; j++) {
            bottomQuads[i][j] = bottomCode.substr(9*i+2*j, 2);
        }
    }
    for (int i=0; i<=topCode.length()/9; i++) {
        for (int j=0; j<4; j++) {
            topQuads[i][j] = topCode.substr(9*i+2*j, 2);
        }
    }

    int stackLayer = 4;
    bool collisionFlag = false;
    for (int i=3; i>=0; i--) {
        collisionFlag = false;
        for (int j=0; j<4-i; j++) {for (int k=0; k<4; k++) {
            if (topQuads[j][k] == "--") {continue;}
            if (bottomQuads[j+i][k] != "--") {collisionFlag = true; break;}
        }}
        if (collisionFlag) {break;}
        stackLayer = i;
    }
    for (int i=0; i<4-stackLayer; i++) {
        for (int j=0; j<4; j++) {
            if (topQuads[i][j] == "--") {continue;}
            bottomQuads[i+stackLayer][j] = topQuads[i][j];
        }
    }

    std::string res="", layerStr="";
    for (int i=0; i<4; i++) {
        layerStr = "";
        for (int j=0; j<4; j++) {layerStr += bottomQuads[i][j];}
        if (layerStr == "--------") {break;}
        if (i) {res += ":";}
        res += layerStr;
    }
    return std::make_shared<Shape>(res);
}

void Stacker::Update() {
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);

    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;

    cooldown += rate;
    if ((cooldown >= 1)
        && (acceptorA->item != nullptr)
        && (acceptorA->progress >= 1)
        && (acceptorB->item != nullptr)
        && (acceptorB->progress >= 1)
        && (ejector->item == nullptr)) {
        ejector->item = Stack(
            std::dynamic_pointer_cast<Shape>(acceptorA->item),
            std::dynamic_pointer_cast<Shape>(acceptorB->item));
        ejector->progress = 0;
        acceptorA->progress = 0;
        acceptorA->RemoveChild(acceptorA->item);
        acceptorA->item = nullptr;
        acceptorB->progress = 0;
        acceptorB->RemoveChild(acceptorB->item);
        acceptorB->item = nullptr;
        cooldown = 0;
    }
    if (cooldown > 1) {cooldown = 1;}

    acceptorA->Update();
    acceptorB->Update();
    ejector->Update();
}
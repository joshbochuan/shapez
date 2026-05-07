//
// Created by joshb on 2026/3/23.
//
#include "buildings/Tunnel.hpp"
#include "World.hpp"
#include "Util/Image.hpp"
#include <iostream>
using namespace World;

Tunnel::Tunnel(int x, int y, int r, TunnelType type, bool upgraded)
    : Machine(x, y, r, BELT_RATE, MachineName::TUNNEL) {
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }
    this->type = type;
    this->acceptor = nullptr;
    this->ejector = nullptr;
    this->distance = 0;
    this->upgraded = upgraded;

    if (acceptor != nullptr) {AddChild(acceptor);}
    if (ejector != nullptr) {AddChild(ejector);}
    this->SetZIndex(50 + (x+y)%2);
    this->m_Transform.rotation = M_PI * 0.5 * r;
}

std::string Tunnel::getSaveString() {
    std::string res = "TUNNEL ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    if (type == TunnelType::OUT) {res += "OUT ";}
    else {res += "IN ";}
    res += std::to_string(upgraded);
    return res;
}

std::shared_ptr<Machine> Tunnel::fromSaveString(std::vector<std::string> prop) {
    int x = std::stoi(prop[1]);
    int y = std::stoi(prop[2]);
    int r = std::stoi(prop[3]);
    TunnelType type = TunnelType::IN;
    if (prop[4] == "OUT") {type = TunnelType::OUT;}
    bool upgraded = std::stoi(prop[5]);
    return std::make_shared<Tunnel>(x, y, r, type, upgraded);
}

void Tunnel::Pair() {
    int dx, dy;
    other = nullptr;
    std::shared_ptr<Tunnel> potentialOther;
    std::shared_ptr<Machine> machineOther;
    if (type == TunnelType::IN) {
        switch (r) {
            case 0: dx = 0; dy = 1; break;
            case 1: dx = -1; dy = 0; break;
            case 2: dx = 0; dy = -1; break;
            case 3: dx = 1; dy = 0; break;
            default: throw std::invalid_argument("unknown tunnel rotation");
        }
    }
    else {
        switch (r) {
            case 0: dx = 0; dy = -1; break;
            case 1: dx = 1; dy = 0; break;
            case 2: dx = 0; dy = 1; break;
            case 3: dx = -1; dy = 0; break;
            default: throw std::invalid_argument("unknown tunnel rotation");
        }
    }

    int maxDistance = 5;
    if (upgraded) {maxDistance = 9;}
    for (int i=1; i <= maxDistance; i++) {
        machineOther = MapMachines[{x+(i*dx), y+(i*dy)}];
        if (machineOther == nullptr) {continue;}
        if (machineOther->getName() != MachineName::TUNNEL) {continue;}
        potentialOther = std::dynamic_pointer_cast<Tunnel>(machineOther);
        if (potentialOther->upgraded != upgraded) {continue;}
        if (potentialOther->r != r) {continue;}
        if (potentialOther->type == type) {break;} // no possible connection behind

        other = potentialOther;
        distance = i;
        if (other->other != nullptr) {other->other->other = nullptr;}
        other->other = std::dynamic_pointer_cast<Tunnel>(shared_from_this());
        break;
    }
}

void Tunnel::Init() {
    MACHINE_COUNT++;

    switch (type) {
        case TunnelType::IN:
            this->acceptor = std::make_shared<ItemAcceptor>(x, y, r);
            this->acceptor->Init();
            AddChild(acceptor);
            SetDrawable(tunnelInTextures[upgraded]);
            break;
        case TunnelType::OUT:
            this->ejector = std::make_shared<ItemEjector>(x, y, r, shared_from_this());
            this->ejector->Init();
            AddChild(ejector);
            SetDrawable(tunnelOutTextures[upgraded]);
            break;
        default: throw std::invalid_argument("unknown tunnel type");
    }

    MapMachines[{x, y}] = shared_from_this();
    Pair();
}

void Tunnel::Delete() {
    MACHINE_COUNT--;
    MapMachines.erase({x, y});
    if (acceptor != nullptr) {acceptor->Delete();}
    if (ejector != nullptr) {ejector->Delete();}
    if (other != nullptr) {other->Pair();}
    other = nullptr;
}

void Tunnel::Update() {
    restored = false;

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.125;
    this->m_Transform.scale.y = cam.scale.y * 1.125;
    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    if (type == TunnelType::OUT) {
        ejector->Update();
        return;
    }

    acceptor->Update();
    if (acceptor->item == nullptr) {return;}
    if (acceptor->progress <= 1.0f) {return;}
    if (other == nullptr) {return;}
    if (other->ejector == nullptr) {throw std::invalid_argument("intunnel's other has no ejector");}
    other->ejector->prep = acceptor->item;
    other->ejector->prepProgress = acceptor->progress;
    acceptor->RemoveItem();
    acceptor->progress = 0;
}

void Tunnel::Restore(int arg) {
    if (type == TunnelType::IN) {return;}
    if (other == nullptr) {return;}

    if (restored) {return;}
    restored = true;
    restored = true;
    other->acceptor->progress = 1;
    other->acceptor->Restore(1);
    if (ejector->prep != nullptr) {other->acceptor->SetItem(ejector->prep);}
    ejector->prep = nullptr;
}

void Tunnel::Promote() {
    if (acceptor != nullptr) {acceptor->Promote();}
    if (ejector != nullptr) {ejector->Promote();}
}

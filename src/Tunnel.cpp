//
// Created by joshb on 2026/3/23.
//
#include "Tunnel.hpp"
#include "Global.hpp"
#include "Util/Image.hpp"
#include <iostream>

Tunnel::Tunnel(int x, int y, int r, TunnelType type, bool upgraded)
    : Machine(x, y, r, BELT_RATE, MachineName::TUNNEL) {
    this->type = type;
    this->acceptor = nullptr;
    this->ejector = nullptr;
    this->distance = 0;
    this->upgraded = upgraded;

    std::string path = "../Resources/Sprites/buildings/underground_belt_";
    switch (type) {
        case TunnelType::IN:
            this->acceptor = std::make_shared<ItemAcceptor>(x, y, r);
            path += "entry";
            break;
        case TunnelType::OUT:
            this->ejector = std::make_shared<ItemEjector>(x, y, r);
            path += "exit";
            break;
        default: throw std::invalid_argument("unknown tunnel type");
    }

    if (acceptor != nullptr) {AddChild(acceptor);}
    if (ejector != nullptr) {AddChild(ejector);}
    if (upgraded) {path += "-tier2";}
    this->SetDrawable(std::make_shared<Util::Image>(path + ".png"));
    this->SetZIndex(50 + (x+y)%2);
    this->m_Transform.rotation = M_PI * 0.5 * r;
}

void Tunnel::Pair() {
    int dx, dy;
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
    if (type == TunnelType::IN) {std::cout << "in\n";}
    else {std::cout << "out\n";}
    for (int i=1; i <= maxDistance; i++) {
        machineOther = MapMachines[{x+(i*dx), y+(i*dy)}];
        std::cout << "0\n";
        if (machineOther == nullptr) {continue;}
        std::cout << "1\n";
        if (machineOther->getName() != MachineName::TUNNEL) {continue;}
        std::cout << "2\n";
        potentialOther = std::dynamic_pointer_cast<Tunnel>(machineOther);
        if (potentialOther->upgraded != upgraded) {continue;}
        std::cout << "3\n";
        if (potentialOther->r != r) {continue;}
        std::cout << "4\n";
        if (potentialOther->type == type) {break;} // no possible connection behind
        std::cout << "5\n";

        other = potentialOther;
        distance = i;
        if (other->other != nullptr) {other->other->other = nullptr;}
        other->other = std::dynamic_pointer_cast<Tunnel>(shared_from_this());
        break;
    }
}

void Tunnel::Init() {
    MapMachines[{x, y}] = shared_from_this();
    if (acceptor != nullptr) {acceptor->Init();}
    if (ejector != nullptr) {ejector->Init();}
    Pair();
    std::cout << (other == nullptr) << std::endl;
}

void Tunnel::Delete() {
    MapMachines.erase({x, y});
    if (acceptor != nullptr) {acceptor->Delete();}
    if (ejector != nullptr) {ejector->Delete();}
    other->Pair(); // re-pair the tunnel on the other side
    other = nullptr;
}

void Tunnel::Update() {
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.125;
    this->m_Transform.scale.y = cam.scale.y * 1.125;

    if (type == TunnelType::OUT) {
        ejector->Update();
        return;
    }

    acceptor->Update();
    if (acceptor->item == nullptr) {return;}
    if (acceptor->progress < 1) {return;}
    if (other == nullptr) {return;}
    if (other->ejector == nullptr) {throw std::invalid_argument("WHAT");}
    if (other->ejector->item != nullptr) {return;}
    other->ejector->AddChild(acceptor->item);
    other->ejector->item = acceptor->item;
    other->ejector->progress = acceptor->progress-1;
    acceptor->RemoveChild(acceptor->item);
    acceptor->item = nullptr;
    acceptor->progress = 0;
}
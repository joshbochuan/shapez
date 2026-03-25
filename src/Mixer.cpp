//
// Created by joshb on 2026/3/25.
//

#include "Mixer.hpp"
#include "Global.hpp"
#include "Color.hpp"

Mixer::Mixer(int x, int y, int r)
    : Machine(x, y, r, MIXER_RATE, MachineName::MIXER){
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }
    std::shared_ptr<Machine> tmp;
    switch (r) {
        case 0: tmp = MapMachines[{x+1, y}]; break;
        case 1: tmp = MapMachines[{x, y+1}]; break;
        case 2: tmp = MapMachines[{x-1, y}]; break;
        case 3: tmp = MapMachines[{x, y-1}]; break;
        default: throw std::invalid_argument("invalid mixer rotation " + std::to_string(r));
    }
    if (tmp != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }

    SetPivot({-84, 0});
    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetDrawable(mixerTexture);

    // cooking up a better X-index for 2-wide objects
    this->SetZIndex(60 + fmod((4.0f*x+y), 16.0f)/16.0f);

    acceptorA = std::make_shared<ItemAcceptor>(x, y, r);
    ejector = std::make_shared<ItemEjector>(x, y, r);
    switch (r) {
        case 0: acceptorB = std::make_shared<ItemAcceptor>(x+1, y, r); break;
        case 1: acceptorB = std::make_shared<ItemAcceptor>(x, y+1, r); break;
        case 2: acceptorB = std::make_shared<ItemAcceptor>(x-1, y, r); break;
        case 3: acceptorB = std::make_shared<ItemAcceptor>(x, y-1, r); break;
        default: throw std::invalid_argument("invalid mixer rotation");
    }
    acceptorA->takesShape = false;
    acceptorB->takesShape = false;
}

void Mixer::Init() {
    MapMachines[{x, y}] = shared_from_this();
    MapMachines[{acceptorB->x, acceptorB->y}] = shared_from_this();
    acceptorA->Init();
    acceptorB->Init();
    ejector->Init();
    AddChild(acceptorA);
    AddChild(acceptorB);
    AddChild(ejector);
}

void Mixer::Delete() {
    MapMachines.erase({x, y});
    MapMachines.erase({acceptorB->x, acceptorB->y});
    acceptorA->Delete();
    acceptorB->Delete();
    ejector->Delete();
    RemoveChild(acceptorA);
    RemoveChild(acceptorB);
    RemoveChild(ejector);
}

std::shared_ptr<Color> Mix(std::shared_ptr<Color> colorA, std::shared_ptr<Color> colorB) {
    return std::make_shared<Color>(colorA->getColor() | colorB->getColor());
}

void Mixer::Update() {
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
        ejector->item = Mix(
            std::dynamic_pointer_cast<Color>(acceptorA->item),
            std::dynamic_pointer_cast<Color>(acceptorB->item));
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
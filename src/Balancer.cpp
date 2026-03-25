//
// Created by joshb on 2026/3/22.
//
#include "Balancer.hpp"
#include "Global.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include <cmath>
#include <iostream>
#include "Util/Time.hpp"

Balancer::Balancer(int x, int y, int r)
    : Machine(x, y, r, BELT_RATE, MachineName::BALANCER) {
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

    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetDrawable(balancerTexture);

    // cooking up a better X-index for 2-wide objects
    this->SetZIndex(60 + fmod((4.0f*x+y), 16.0f)/16.0f);

    this->acceptorA = std::make_shared<ItemAcceptor>(x, y, r);
    this->ejectorA = std::make_shared<ItemEjector>(x, y, r);

    switch (r) {
        case 0:
            this->acceptorB = std::make_shared<ItemAcceptor>(x+1, y, r);
            this->ejectorB = std::make_shared<ItemEjector>(x+1, y, r);
            break;
        case 1:
            this->acceptorB = std::make_shared<ItemAcceptor>(x, y+1, r);
            this->ejectorB = std::make_shared<ItemEjector>(x, y+1, r);
            break;
        case 2:
            this->acceptorB = std::make_shared<ItemAcceptor>(x-1, y, r);
            this->ejectorB = std::make_shared<ItemEjector>(x-1, y, r);
            break;
        case 3:
            this->acceptorB = std::make_shared<ItemAcceptor>(x, y-1, r);
            this->ejectorB = std::make_shared<ItemEjector>(x, y-1, r);
            break;
        default: throw std::invalid_argument("illegal balancer rotation " + std::to_string(r));
    }

    this->AddChild(this->acceptorA);
    this->AddChild(this->acceptorB);
    this->AddChild(this->ejectorA);
    this->AddChild(this->ejectorB);

    /*
    std::vector<std::string> tops, bottoms;
    for (int i = 0; i <= 13; i++) {
        tops.push_back("../Resources/sprites/belt/built/forward_" + std::to_string(i) + "_top.png");
        bottoms.push_back("../Resources/sprites/belt/built/forward_" + std::to_string(i) + "_bottom.png");
    }
    */

    acceptorA->SetDrawable(balancerInTextures[0]);
    acceptorB->SetDrawable(balancerInTextures[0]);
    ejectorA->SetDrawable(balancerOutTextures[0]);
    ejectorB->SetDrawable(balancerOutTextures[0]);

    acceptPriority = 0;
    ejectPriority = 0;
}

void Balancer::Init() {
    MapMachines[{x, y}] = shared_from_this();
    MapMachines[{ejectorB->x, ejectorB->y}] = shared_from_this();
    acceptorA->Init();
    acceptorB->Init();
    ejectorA->Init();
    ejectorB->Init();
    std::cout << acceptorA.use_count() << " " << ejectorA.use_count() << " ";
    std::cout << acceptorB.use_count() << " " << ejectorB.use_count() << std::endl;
}

void Balancer::Delete() {
    // similar to free for traditional pointers
    MapMachines.erase({x, y});
    MapMachines.erase({ejectorB->x, ejectorB->y});
    acceptorA->Delete();
    acceptorB->Delete();
    ejectorA->Delete();
    ejectorB->Delete();
    RemoveChild(acceptorA);
    RemoveChild(acceptorB);
    RemoveChild(ejectorA);
    RemoveChild(ejectorB);
    std::cout << acceptorA.use_count() << " " << ejectorA.use_count() << " ";
    std::cout << acceptorB.use_count() << " " << ejectorB.use_count() << std::endl;
}

void Balancer::Update() {
    int frame = static_cast<int>(std::fmod(Util::Time::GetElapsedTimeMs()*0.042f, 14.0f));
    acceptorA->SetDrawable(balancerInTextures[frame]);
    acceptorB->SetDrawable(balancerInTextures[frame]);
    ejectorA->SetDrawable(balancerOutTextures[frame]);
    ejectorB->SetDrawable(balancerOutTextures[frame]);
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

    // 0 in or 0 out: don't do anything
    // 1 in 1 out: input goes to output, set priority to opposite
    // 1 in 2 out: input goes to prioritized output
    // 2 in 1 out: takes input from prioritized input
    // 2 in 2 out: same side input goes to same side output

    std::shared_ptr<Item> item;
    float progress;
    const bool aA = acceptorA->item != nullptr && acceptorA->progress >= 1;
    const bool aB = acceptorB->item != nullptr && acceptorB->progress >= 1;
    const bool eA = ejectorA->item==nullptr && ejectorA->next!=nullptr;
    const bool eB = ejectorB->item==nullptr && ejectorB->next!=nullptr;
    if ((aA && aB) && (eA && eB)) { // 2 in 2 out
        ejectorA->item = acceptorA->item;
        ejectorA->AddChild(ejectorA->item);
        ejectorA->progress = acceptorA->progress-1;
        acceptorA->RemoveChild(acceptorA->item);
        acceptorA->item = nullptr;
        acceptorA->progress = 0;
        ejectorB->item = acceptorB->item;
        ejectorB->AddChild(ejectorB->item);
        ejectorB->progress = acceptorB->progress-1;
        acceptorB->RemoveChild(acceptorB->item);
        acceptorB->item = nullptr;
        acceptorB->progress = 0;
    }
    else if ((aA ^ aB) && (eA && eB)) {
        // 1 in 2 out
        if (aA) {
            item = acceptorA->item;
            progress = acceptorA->progress;
            acceptPriority = 1;
            acceptorA->RemoveChild(acceptorA->item);
            acceptorA->item = nullptr;
            acceptorA->progress = 0;
        }
        else {
            item = acceptorB->item;
            progress = acceptorB->progress;
            acceptPriority = 0;
            acceptorB->RemoveChild(acceptorB->item);
            acceptorB->item = nullptr;
            acceptorB->progress = 0;
        }
        if (ejectPriority) { // outputs to prioritized one (B)
            ejectorB->item = item;
            ejectorB->progress = progress-1;
            ejectorB->AddChild(item);
            ejectPriority = 0;
        }
        else {
            ejectorA->item = item;
            ejectorA->progress = progress-1;
            ejectorA->AddChild(item);
            ejectPriority = 1;
        }
    }
    else if ((aA && aB) && (eA ^ eB)) {
        // 2 in 1 out
        if (acceptPriority) { // B
            item = acceptorB->item;
            progress = acceptorB->progress;
            acceptPriority = 0;
            acceptorB->RemoveChild(acceptorB->item);
            acceptorB->item = nullptr;
            acceptorB->progress = 0;
        }
        else {
            item = acceptorA->item;
            progress = acceptorA->progress;
            acceptPriority = 1;
            acceptorA->RemoveChild(acceptorA->item);
            acceptorA->item = nullptr;
            acceptorA->progress = 0;
        }
        if (eA) {
            ejectorA->item = item;
            ejectorA->progress = progress-1;
            ejectorA->AddChild(item);
            ejectPriority = 1;
        }
        else {
            ejectorB->item = item;
            ejectorB->progress = progress-1;
            ejectorB->AddChild(item);
            ejectPriority = 0;
        }
    }
    else if ((aA ^ aB) && (eA ^ eB)) { // 1 in 1 out
        if (aA) {
            item = acceptorA->item;
            progress = acceptorA->progress;
            acceptPriority = 1;
            acceptorA->RemoveChild(acceptorA->item);
            acceptorA->item = nullptr;
            acceptorA->progress = 0;
        }
        else {
            item = acceptorB->item;
            progress = acceptorB->progress;
            acceptPriority = 0;
            acceptorB->RemoveChild(acceptorB->item);
            acceptorB->item = nullptr;
            acceptorB->progress = 0;
        }
        if (eA) {
            ejectorA->item = item;
            ejectorA->progress = progress-1;
            ejectorA->AddChild(item);
            ejectPriority = 1;
        }
        else {
            ejectorB->item = item;
            ejectorB->progress = progress-1;
            ejectorB->AddChild(item);
            ejectPriority = 0;
        }
    }
    acceptorA->Update();
    acceptorB->Update();
    ejectorA->Update();
    ejectorB->Update();
}


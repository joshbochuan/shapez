//
// Created by joshb on 2026/3/22.
//
#include "buildings/Balancer.hpp"
#include "buildings/Belt.hpp"
#include "World.hpp"
#include "Util/Image.hpp"
#include <cmath>
#include <iostream>
#include "Util/Time.hpp"
using namespace World;

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

    SetPivot({-84, 0});
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

    acceptorA->SetDrawable(Belt::beltInTexture[0]);
    acceptorB->SetDrawable(Belt::beltInTexture[0]);
    ejectorA->SetDrawable(Belt::beltOutTexture[0]);
    ejectorB->SetDrawable(Belt::beltOutTexture[0]);

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
}

void Balancer::Update() {
    int frame = static_cast<int>(std::fmod(Util::Time::GetElapsedTimeMs()*0.042f, 14.0f));
    acceptorA->SetDrawable(Belt::beltInTexture[frame]);
    acceptorB->SetDrawable(Belt::beltInTexture[frame]);
    ejectorA->SetDrawable(Belt::beltOutTexture[frame]);
    ejectorB->SetDrawable(Belt::beltOutTexture[frame]);

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);

    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;

    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*384 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*384 < WINDOW_HEIGHT>>1));

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

Splitter::Splitter(int x, int y, int r, bool mirrored)
    : Machine(x, y, r, BELT_RATE, MachineName::SPLITTER) {
    this->mirrored = mirrored;
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }

    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetDrawable(splitterTexture[mirrored]);

    // cooking up a better X-index for 2-wide objects
    this->SetZIndex(62 + (x+y)%2);

    this->acceptor = std::make_shared<ItemAcceptor>(x, y, r);
    this->ejectorA = std::make_shared<ItemEjector>(x, y, r);
    this->ejectorB = std::make_shared<ItemEjector>(x, y, (r+3 + 2*mirrored)%4);

    this->AddChild(this->acceptor);
    this->AddChild(this->ejectorA);
    this->AddChild(this->ejectorB);

    acceptor->SetDrawable(Belt::beltInTexture[0]);
    ejectorA->SetDrawable(Belt::beltOutTexture[0]);
    ejectPriority = 0;
}

void Splitter::Init() {
    MapMachines[{x, y}] = shared_from_this();
    acceptor->Init();
    ejectorA->Init();
    ejectorB->Init();
}

void Splitter::Delete() {
    MapMachines.erase({x, y});
    acceptor->Delete();
    ejectorA->Delete();
    ejectorB->Delete();
    RemoveChild(acceptor);
    RemoveChild(ejectorA);
    RemoveChild(ejectorB);
}

void Splitter::Update() {
    int frame = static_cast<int>(std::fmod(Util::Time::GetElapsedTimeMs()*0.042f, 14.0f));
    acceptor->SetDrawable(Belt::beltInTexture[frame]);
    ejectorA->SetDrawable(Belt::beltOutTexture[frame]);

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);

    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;

    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    const bool acceptorReady = acceptor->item!=nullptr && acceptor->progress>=1;
    const bool eA = ejectorA->item==nullptr && ejectorA->next!=nullptr;
    const bool eB = ejectorB->item==nullptr && ejectorB->next!=nullptr;
    if (acceptorReady && (eA || eB)) {
        std::shared_ptr<Item> item = acceptor->item;
        float progress = acceptor->progress;
        acceptor->RemoveChild(acceptor->item);
        acceptor->item = nullptr;
        acceptor->progress = 0;
        if ((ejectPriority || !eA) && eB) { // outputs to prioritized one (B)
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
        item = nullptr;
    }
    acceptor->Update();
    ejectorA->Update();
    ejectorB->Update();
}

Merger::Merger(int x, int y, int r, bool mirrored)
    : Machine(x, y, r, BELT_RATE, MachineName::MERGER) {
    this->mirrored = mirrored;
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }

    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetDrawable(mergerTexture[mirrored]);

    // cooking up a better X-index for 2-wide objects
    this->SetZIndex(62 + (x+y)%2);

    this->acceptorA = std::make_shared<ItemAcceptor>(x, y, r);
    this->acceptorB = std::make_shared<ItemAcceptor>(x, y, (r+1 + 2*mirrored)%4);
    this->ejector = std::make_shared<ItemEjector>(x, y, r);

    this->AddChild(this->acceptorA);
    this->AddChild(this->acceptorB);
    this->AddChild(this->ejector);

    acceptorA->SetDrawable(Belt::beltInTexture[0]);
    ejector->SetDrawable(Belt::beltOutTexture[0]);
    acceptPriority = 0;
}

void Merger::Init() {
    MapMachines[{x, y}] = shared_from_this();
    acceptorA->Init();
    acceptorB->Init();
    ejector->Init();
}

void Merger::Delete() {
    MapMachines.erase({x, y});
    acceptorA->Delete();
    acceptorB->Delete();
    ejector->Delete();
    RemoveChild(acceptorA);
    RemoveChild(acceptorB);
    RemoveChild(ejector);
}

void Merger::Update() {
    int frame = static_cast<int>(std::fmod(Util::Time::GetElapsedTimeMs()*0.042f, 14.0f));
    acceptorA->SetDrawable(Belt::beltInTexture[frame]);
    ejector->SetDrawable(Belt::beltOutTexture[frame]);

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);

    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;

    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    const bool ejectorReady = ejector->next!=nullptr && ejector->item == nullptr;
    const bool aA = acceptorA->item!=nullptr && acceptorA->progress>=1;
    const bool aB = acceptorB->item!=nullptr && acceptorB->progress>=1;
    if (ejectorReady && (aA || aB)) {
        std::shared_ptr<Item> item = nullptr;
        float progress = 0;
        if ((acceptPriority || !aA) && aB) {
            item = acceptorB->item;
            progress = acceptorB->progress;
            acceptPriority = false;
            acceptorB->RemoveChild(acceptorB->item);
            acceptorB->item = nullptr;
            acceptorB->progress = 0;
        }
        else {
            item = acceptorA->item;
            progress = acceptorA->progress;
            acceptPriority = true;
            acceptorA->RemoveChild(acceptorA->item);
            acceptorA->item = nullptr;
            acceptorA->progress = 0;
        }
        ejector->item = item;
        ejector->progress = progress-1;
        ejector->AddChild(item);
        item = nullptr;
    }
    acceptorA->Update();
    acceptorB->Update();
    ejector->Update();
}
//
// Created by joshb on 2026/3/22.
//
#include "buildings/Balancer.hpp"
#include "buildings/Belt.hpp"
#include "World.hpp"
#include "Util/Image.hpp"
#include <cmath>
#include <complex>
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

    acceptPriority = 0;
    ejectPriority = 0;
}

std::string Balancer::getSaveString() {
    std::string res = "BALANCER ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    res += std::to_string(acceptPriority) + " ";
    res += std::to_string(ejectPriority);
    return res;
}

std::shared_ptr<Machine> Balancer::fromSaveString(std::vector<std::string> prop) {
    int x = std::stoi(prop[1]);
    int y = std::stoi(prop[2]);
    int r = std::stoi(prop[3]);
    int acceptPriority = std::stoi(prop[4]);
    int ejectPriority = std::stoi(prop[5]);
    std::shared_ptr<Balancer> res = std::make_shared<Balancer>(x, y, r);
    res->acceptPriority = acceptPriority;
    res->ejectPriority = ejectPriority;
    return res;
}

void Balancer::Init() {
    this->acceptorA = std::make_shared<ItemAcceptor>(x, y, r);
    this->ejectorA = std::make_shared<ItemEjector>(x, y, r, shared_from_this());

    switch (r) {
        case 0:
            this->acceptorB = std::make_shared<ItemAcceptor>(x+1, y, r);
            this->ejectorB = std::make_shared<ItemEjector>(x+1, y, r, shared_from_this());
            break;
        case 1:
            this->acceptorB = std::make_shared<ItemAcceptor>(x, y+1, r);
            this->ejectorB = std::make_shared<ItemEjector>(x, y+1, r, shared_from_this());
            break;
        case 2:
            this->acceptorB = std::make_shared<ItemAcceptor>(x-1, y, r);
            this->ejectorB = std::make_shared<ItemEjector>(x-1, y, r, shared_from_this());
            break;
        case 3:
            this->acceptorB = std::make_shared<ItemAcceptor>(x, y-1, r);
            this->ejectorB = std::make_shared<ItemEjector>(x, y-1, r, shared_from_this());
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

    MapMachines[{x, y}] = shared_from_this();
    MapMachines[{ejectorB->x, ejectorB->y}] = shared_from_this();
    acceptorA->Init();
    acceptorB->Init();
    ejectorA->Init();
    ejectorB->Init();
    MACHINE_COUNT++;
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
    MACHINE_COUNT--;
}

void Balancer::Update() {
    restored = false;

    int frame = static_cast<int>(std::fmod(Util::Time::GetElapsedTimeMs()*0.042f*MULTIPLIER_BELT, 14.0f));
    acceptorA->SetDrawable(Belt::beltInTexture[frame]);
    acceptorB->SetDrawable(Belt::beltInTexture[frame]);
    ejectorA->SetDrawable(Belt::beltOutTexture[frame]);
    ejectorB->SetDrawable(Belt::beltOutTexture[frame]);
    acceptorA->SetVisible(acceptorA->prev != nullptr);
    acceptorB->SetVisible(acceptorB->prev != nullptr);
    ejectorA->SetVisible(ejectorA->next != nullptr);
    ejectorB->SetVisible(ejectorB->next != nullptr);

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

    acceptorA->Update();
    acceptorB->Update();
    ejectorA->Update();
    ejectorB->Update();
    std::shared_ptr<Item> item;
    float progress;
    const bool aA = acceptorA->item != nullptr && acceptorA->progress >= 1;
    const bool aB = acceptorB->item != nullptr && acceptorB->progress >= 1;
    const bool eA = (ejectorA->next!=nullptr) && (ejectorA->item == nullptr);
    const bool eB = (ejectorB->next!=nullptr) && (ejectorB->item == nullptr);
    transferStates = (aA<<3) | (aB<<2) | (eA<<1) | (eB);

    if ((!aA) && (!aB)) {return;}
    if ((!eA) && (!eB)) {return;}

    if ((aA && aB) && (eA && eB)) { // 2 in 2 out
        ejectorA->prep = acceptorA->item;
        ejectorA->prepProgress = acceptorA->progress;
        acceptorA->RemoveItem();
        ejectorA->progress = 0;
        ejectorB->prep = acceptorB->item;
        ejectorB->prepProgress = acceptorB->progress;
        acceptorB->RemoveItem();
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
            ejectorB->prep = item;
            ejectorB->progress = progress-1;
            ejectorB->AddChild(item);
            ejectPriority = 0;
        }
        else {
            ejectorA->prep = item;
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
            ejectorA->prep = item;
            ejectorA->progress = progress-1;
            ejectorA->AddChild(item);
            ejectPriority = 1;
        }
        else {
            ejectorB->prep = item;
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
            ejectorA->prep = item;
            ejectorA->progress = progress-1;
            ejectorA->AddChild(item);
            ejectPriority = 1;
        }
        else {
            ejectorB->prep = item;
            ejectorB->progress = progress-1;
            ejectorB->AddChild(item);
            ejectPriority = 0;
        }
    }

}

void Balancer::Restore(int arg, std::shared_ptr<ItemEjector> from) {
    if (!arg) {return;}
    if (transferStates == 0b0000) {return;}
    if (arg && (transferStates == 0b1111)) {
        if (acceptPriority) {transferStates = 0b0111;}
        else {transferStates = 0b1011;}
    }
    switch (transferStates) {
        case 0b0111: break;
        case 0b1011: break;
        case 0b0101: break;
        case 0b0110: break;
        case 0b1001: break;
        case 0b1010: break;
        default: transferStates = 0b0000; break;
    }

    restored = true;
    if (ejectorA->prep != nullptr) {
        ejectorA->RemoveChild(ejectorA->prep);
        ejectorA->prep = nullptr;
    }
    if (ejectorB->prep != nullptr) {
        ejectorB->RemoveChild(ejectorB->prep);
        ejectorB->prep = nullptr;
    }
    if (backupItemA != nullptr) {
        acceptorA->item = backupItemA;
        AddChild(acceptorA);
        acceptorA->progress = 1;
        acceptorA->Restore(arg);
    }
    if (backupItemB != nullptr) {
        acceptorB->item = backupItemB;
        AddChild(acceptorB);
        acceptorB->progress = 1;
        acceptorB->Restore(arg);
    }
}

void Balancer::Promote() {
    acceptorA->Promote();
    acceptorB->Promote();
    ejectorA->Promote();
    ejectorB->Promote();
}

Splitter::Splitter(int x, int y, int r, bool mirrored)
    : Machine(x, y, r, BELT_RATE, MachineName::SPLITTER) {
    this->mirrored = mirrored;
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }

    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetDrawable(splitterTexture[mirrored]);

    this->SetZIndex(62 + (x+y)%2);

    ejectPriority = 0;
}

std::string Splitter::getSaveString() {
    std::string res = "SPLITTER ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    res += std::to_string(mirrored) + " ";
    res += std::to_string(ejectPriority);
    return res;
}

std::shared_ptr<Machine> Splitter::fromSaveString(std::vector<std::string> prop) {
    int x = std::stoi(prop[1]);
    int y = std::stoi(prop[2]);
    int r = std::stoi(prop[3]);
    bool mirrored = std::stoi(prop[4]);
    int ejectPriority = std::stoi(prop[5]);
    std::shared_ptr<Splitter> res = std::make_shared<Splitter>(x, y, r, mirrored);
    res->ejectPriority = ejectPriority;
    res->lastEjectPriority = ejectPriority;
    return res;
}

void Splitter::Init() {
    this->acceptor = std::make_shared<ItemAcceptor>(x, y, r);
    this->ejectorA = std::make_shared<ItemEjector>(x, y, r, shared_from_this());
    this->ejectorB = std::make_shared<ItemEjector>(x, y, (r+3 + 2*mirrored)%4, shared_from_this());

    acceptor->Init();
    ejectorA->Init();
    ejectorB->Init();

    this->AddChild(this->acceptor);
    this->AddChild(this->ejectorA);
    this->AddChild(this->ejectorB);

    acceptor->SetDrawable(Belt::beltInTexture[0]);
    ejectorA->SetDrawable(Belt::beltOutTexture[0]);

    MapMachines[{x, y}] = shared_from_this();
    MACHINE_COUNT++;
}

void Splitter::Delete() {
    MapMachines.erase({x, y});
    acceptor->Delete();
    ejectorA->Delete();
    ejectorB->Delete();
    RemoveChild(acceptor);
    RemoveChild(ejectorA);
    RemoveChild(ejectorB);
    MACHINE_COUNT--;
}

void Splitter::Update() {
    restored = false;

    int frame = static_cast<int>(std::fmod(Util::Time::GetElapsedTimeMs()*0.042f*MULTIPLIER_BELT, 14.0f));
    acceptor->SetDrawable(Belt::beltInTexture[frame]);
    ejectorA->SetDrawable(Belt::beltOutTexture[frame]);
    acceptor->SetVisible(acceptor->prev != nullptr);
    ejectorA->SetVisible(ejectorA->next != nullptr);

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);

    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;

    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    acceptor->Update();
    ejectorA->Update();
    ejectorB->Update();
    lastEjectPriority = ejectPriority;
    const bool acceptorReady = acceptor->item!=nullptr && acceptor->progress>1;
    const bool eA = (ejectorA->next!=nullptr) && (ejectorA->item == nullptr);
    const bool eB = (ejectorB->next!=nullptr) && (ejectorB->item == nullptr);
    if (acceptorReady && (eA || eB)) {
        std::shared_ptr<Item> item = acceptor->item;
        float progress = acceptor->progress;
        acceptor->RemoveItem();
        acceptor->progress = 0;
        if ((ejectPriority || !eA) && eB) { // outputs to prioritized one (B)
            ejectorB->prep = item;
            ejectorB->prepProgress = progress;
            ejectPriority = 0;
        }
        else {
            ejectorA->prep = item;
            ejectorA->prepProgress = progress;
            ejectPriority = 1;
        }
        item = nullptr;
    }
}

void Splitter::Restore(int arg, std::shared_ptr<ItemEjector> from) {
    if (restored) {return;}
    if (from->prep == nullptr) {return;}
    restored = true;
    acceptor->progress = 1;
    acceptor->Restore(1);
    acceptor->SetItem(from->prep);
    from->prep = nullptr;
}

void Splitter::Promote() {
    acceptor->Promote();
    ejectorA->Promote();
    ejectorB->Promote();
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

    acceptPriority = 0;
    lastAcceptPriority = 0;
}

std::string Merger::getSaveString() {
    std::string res = "MERGER ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    res += std::to_string(mirrored) + " ";
    res += std::to_string(acceptPriority);
    return res;
}

std::shared_ptr<Machine> Merger::fromSaveString(std::vector<std::string> prop) {
    int x = std::stoi(prop[1]);
    int y = std::stoi(prop[2]);
    int r = std::stoi(prop[3]);
    bool mirrored = std::stoi(prop[4]);
    int acceptPriority = std::stoi(prop[5]);
    std::shared_ptr<Merger> res = std::make_shared<Merger>(x, y, r, mirrored);
    res->acceptPriority = acceptPriority;
    res->lastAcceptPriority = acceptPriority;
    return res;
}

void Merger::Init() {
    MapMachines[{x, y}] = shared_from_this();

    this->acceptorA = std::make_shared<ItemAcceptor>(x, y, r);
    this->acceptorB = std::make_shared<ItemAcceptor>(x, y, (r+1 + 2*mirrored)%4);
    this->ejector = std::make_shared<ItemEjector>(x, y, r, shared_from_this());

    acceptorA->Init();
    acceptorB->Init();
    ejector->Init();

    acceptorA->SetDrawable(Belt::beltInTexture[0]);
    ejector->SetDrawable(Belt::beltOutTexture[0]);

    this->AddChild(this->acceptorA);
    this->AddChild(this->acceptorB);
    this->AddChild(this->ejector);

    MACHINE_COUNT++;
}

void Merger::Delete() {
    MapMachines.erase({x, y});
    acceptorA->Delete();
    acceptorB->Delete();
    ejector->Delete();
    RemoveChild(acceptorA);
    RemoveChild(acceptorB);
    RemoveChild(ejector);
    MACHINE_COUNT--;
}

void Merger::Update() {
    restored = false;

    int frame = static_cast<int>(std::fmod(Util::Time::GetElapsedTimeMs()*0.042f*MULTIPLIER_BELT, 14.0f));
    acceptorA->SetDrawable(Belt::beltInTexture[frame]);
    ejector->SetDrawable(Belt::beltOutTexture[frame]);
    acceptorA->SetVisible(acceptorA->prev != nullptr);
    ejector->SetVisible(ejector->next != nullptr);

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);

    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;

    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    acceptorA->Update();
    acceptorB->Update();
    ejector->Update();
    const bool ejectorReady = (ejector->next != nullptr) && (ejector->item == nullptr);
    const bool aA = acceptorA->item!=nullptr && acceptorA->progress>1;
    const bool aB = acceptorB->item!=nullptr && acceptorB->progress>1;
    lastAcceptPriority = acceptPriority;
    if (ejectorReady && (aA || aB)) {
        std::shared_ptr<Item> item = nullptr;
        float progress = 0;
        if ((acceptPriority || !aA) && aB) {
            item = acceptorB->item;
            progress = acceptorB->progress;
            acceptPriority = false;
            acceptorB->RemoveItem();
            acceptorB->progress = 0;
        }
        else {
            item = acceptorA->item;
            progress = acceptorA->progress;
            acceptPriority = true;
            acceptorA->RemoveItem();
            acceptorA->progress = 0;
        }
        ejector->prep = item;
        ejector->prepProgress = progress;
        item = nullptr;
    }
}

void Merger::Restore(int arg, std::shared_ptr<ItemEjector> from) {
    if (restored) {return;}
    restored = true;
    if (ejector->prep != nullptr) {
        if (acceptPriority) { // previously taken priority 0, or A
            acceptorA->progress = 1;
            acceptorA->Restore(1);
            acceptorA->SetItem(ejector->prep);
        }
        else {
            acceptorB->progress = 1;
            acceptorB->Restore(1);
            acceptorB->SetItem(ejector->prep);
        }
        ejector->prep = nullptr;
        acceptPriority = lastAcceptPriority;
    }
}

void Merger::Promote() {
    acceptorA->Promote();
    acceptorB->Promote();
    ejector->Promote();
}

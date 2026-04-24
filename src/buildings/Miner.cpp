//
// Created by joshb on 2026/3/20.
//

#include "buildings/Miner.hpp"
#include "buildings/Machine.hpp"
#include "Util/Image.hpp"
#include <iostream>
using namespace World;

Miner::Miner(int x, int y, int r, std::shared_ptr<Item> product, bool chained)
    : Machine(x, y, r, MINE_RATE, MachineName::MINER) {
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }
    this->chained = chained;
    this->cooldown = 0;
    this->product = product;
    if (product != nullptr) {
        this->product->MachineItemZIndex(44);
        this->AddChild(this->product);
    }
    this->ejector = std::make_shared<ItemEjector>(x, y, r);
    this->AddChild(this->ejector);
    if (chained) {SetDrawable(chainedMinerTexture);}
    else {this->SetDrawable(minerTexture);}
    this->m_Transform.rotation = M_PI * 0.5 * r;
    this->SetZIndex(42 + (x+y)%2);
    this->cover = std::make_shared<OptiObject>();
    cover->SetDrawable(minerCoverTexture);
    cover->SetZIndex(40);
    this->AddChild(cover);
}

void Miner::Init() {
    MapMachines[{x, y}] = shared_from_this();
    ejector->Init();
    if (!chained) {return;}

    int nextX, nextY;
    switch (r) {
        case 0: nextX=0, nextY=1; break;
        case 1: nextX=-1, nextY=0; break;
        case 2: nextX=0, nextY=-1; break;
        case 3: nextX=1, nextY=0; break;
        default: throw std::invalid_argument("invalid miner rotation");
    }

    std::shared_ptr<Machine> machineNext = MapMachines[{x+nextX, y+nextY}];
    std::shared_ptr<Miner> minerNext = nullptr;

    if (machineNext != nullptr && machineNext->getName() == MachineName::MINER) {
        minerNext = std::dynamic_pointer_cast<Miner>(machineNext);
    }
    if (minerNext != nullptr && minerNext->isChained()) {
        next = minerNext;
        minerNext->prev.push_back(std::dynamic_pointer_cast<Miner>(shared_from_this()));
    }
    for (int i=1; i<=3; i++) {
        std::swap(nextX, nextY);
        nextX = -nextX; // ccw
        machineNext = MapMachines[{x+nextX, y+nextY}];
        if (machineNext == nullptr) {continue;}
        if (machineNext->getName() != MachineName::MINER) {continue;}
        if (machineNext->r != (r+i+2)%4) {continue;}
        minerNext = std::dynamic_pointer_cast<Miner>(machineNext);
        if (!minerNext->isChained()) {continue;}
        minerNext->next = std::dynamic_pointer_cast<Miner>(shared_from_this());
        prev.push_back(minerNext);
    }
    std::shared_ptr<Miner> root = FindRoot();
    if (root == nullptr) {
        return;
    }
    root->UpdateChainLen();
}

void Miner::Delete() {
    MapMachines.erase({x, y});
    ejector->Delete();
    std::shared_ptr<Miner> root;
    if (next != nullptr) {
        next->prev.erase(
            std::remove(next->prev.begin(),
                next->prev.end(),
                std::dynamic_pointer_cast<Miner>(shared_from_this())),
            next->prev.end());
        root = next->FindRoot();
        if (next->FindRoot() != nullptr) {root->UpdateChainLen();}
    }
    for (int i=0; i<prev.size(); i++) {
        prev[i]->next = nullptr;
        prev[i]->UpdateChainLen();
    }
}

void Miner::Update() {
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.1f;
    this->m_Transform.scale.y = cam.scale.y * 1.1f;

    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    cover->m_Transform.scale.x = cam.scale.x * 0.85f;
    cover->m_Transform.scale.y = cam.scale.y * 0.85f;
    cover->m_Transform.translation = this->m_Transform.translation;
    if (this->product != nullptr) {
        this->product->m_Transform.translation = this->m_Transform.translation;
        this->product->SetItemSize(cam.scale);
        this->product->Update();
    }

    cooldown += rate * static_cast<float>(chainLen) * MULTIPLIER_MINE;
    if ((cooldown >= 1)
        && (product != nullptr)
        && (ejector->item == nullptr)) {
        cooldown -= 1;
        ejector->item = product->copy();
        ejector->AddChild(ejector->item);
        ejector->progress = 0;
    }
    if (cooldown > 1) {cooldown = 1;}

    ejector->Update();
}

bool Miner::isChained() {
    return chained;
}

bool isChainedMiner(std::shared_ptr<Machine> machine) {
    // determines if certain machine is a chained miner
    if (machine == nullptr) {return false;}
    if (machine->getName() != MachineName::MINER) {return false;}
    std::shared_ptr<Miner> miner = std::dynamic_pointer_cast<Miner>(machine);
    return miner->isChained();
}

std::shared_ptr<Miner> Miner::FindRoot() {
    // floyd's cycle-finding algorithm
    // O(1) space and avoids looping in the loop
    // hacker type shi
    std::shared_ptr<Miner> slow = std::dynamic_pointer_cast<Miner>(shared_from_this());
    std::shared_ptr<Miner> fast = std::dynamic_pointer_cast<Miner>(shared_from_this());
    do {
        if (slow->next != nullptr) {slow = slow->next;} else {return slow;}
        if (fast->next != nullptr) {fast = fast->next;} else {return fast;}
        if (fast->next != nullptr) {fast = fast->next;} else {return fast;}
    } while (slow != fast);
    // code going here means there's a loop
    return nullptr;
};

int Miner::UpdateChainLen() {
    // called from the root and will be stuck if there's a loop
    chainLen = 1;
    for (int i=0; i<prev.size(); i++) {
        chainLen += prev[i]->UpdateChainLen();
    }
    return chainLen;
}
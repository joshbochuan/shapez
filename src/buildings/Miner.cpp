//
// Created by joshb on 2026/3/20.
//

#include "buildings/Miner.hpp"
#include "buildings/Machine.hpp"
#include "Util/Image.hpp"
#include <iostream>

#include "items/Color.hpp"
using namespace World;

Miner::Miner(int x, int y, int r, std::shared_ptr<Item> product, bool chained)
    : Machine(x, y, r, MINE_RATE, MachineName::MINER) {
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }
    this->chained = chained;
    this->cooldown = 0;
    this->product = product;
    std::shared_ptr<Chunk> insideChunk = chunks[{x>>4, y>>4}];
    if (insideChunk == nullptr) {chunks.erase({x>>4, y>>4});}
    if ((product == nullptr)
        && (insideChunk != nullptr)
        && (insideChunk->item != nullptr)
        && (insideChunk->minePoints[x&15][y&15])) {
        this->product = insideChunk->item->copy();
    }
    if (this->product != nullptr) {
        this->product->MachineItemZIndex(44);
        this->AddChild(this->product);
    }
    if (chained) {SetDrawable(chainedMinerTexture);}
    else {this->SetDrawable(minerTexture);}
    this->m_Transform.rotation = M_PI * 0.5 * r;
    this->SetZIndex(42 + (x+y)%2);
    this->cover = std::make_shared<OptiObject>();
    cover->SetZIndex(40);
    this->AddChild(cover);

    if (this->product == nullptr) {return;}
    if (this->product->getType() == ItemType::SHAPE) {
        cover->SetDrawable(minerCoverTextureGray);
        return;
    }
    switch (std::dynamic_pointer_cast<Color>(this->product)->getColor()) {
        case 1: cover->SetDrawable(minerCoverTextureBlue); break;
        case 2: cover->SetDrawable(minerCoverTextureGreen); break;
        case 4: cover->SetDrawable(minerCoverTextureRed); break;
        default: cover->SetDrawable(minerCoverTextureGray); break;
    }
}

std::string Miner::getSaveString() {
    std::string res = "MINER ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    if (product == nullptr) {res += "NULL ";}
    else {res += product->getCode() + " ";}
    res += std::to_string(chained) + " ";
    res += std::to_string(cooldown);
    return res;
}

std::shared_ptr<Machine> Miner::fromSaveString(std::vector<std::string> prop) {
    int x = std::stoi(prop[1]);
    int y = std::stoi(prop[2]);
    int r = std::stoi(prop[3]);
    std::shared_ptr<Item> item = nullptr;
    if (prop[4].substr(0, 5) == "Color") {item = std::make_shared<Color>(prop[4]);}
    else if (prop[4] != "NULL") {item = std::make_shared<Shape>(prop[4]);}
    bool chained = std::stoi(prop[5]);
    float cd = std::stof(prop[6]);
    auto res = std::make_shared<Miner>(x, y, r, item, chained);
    res->cooldown = cd;
    return res;
}

void Miner::Init() {
    this->ejector = std::make_shared<ItemEjector>(x, y, r, shared_from_this());
    this->AddChild(this->ejector);

    MACHINE_COUNT++;
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
    if ((minerNext != nullptr)
        && (minerNext->isChained())
        && (product != nullptr)
        && (minerNext->product != nullptr)
        && (minerNext->product->getCode() == this->product->getCode())) {
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
        if (product == nullptr) {continue;}
        minerNext = std::dynamic_pointer_cast<Miner>(machineNext);
        if (!minerNext->isChained()) {continue;}
        if (minerNext->product == nullptr) {continue;}
        if (minerNext->product->getCode() != this->product->getCode()) {continue;}
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
    MACHINE_COUNT--;
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
    restored = false;

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.1f;
    this->m_Transform.scale.y = cam.scale.y * 1.1f;

    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    cover->m_Transform.scale.x = cam.scale.x * 160.0f;
    cover->m_Transform.scale.y = cam.scale.y * 160.0f;
    cover->m_Transform.translation = this->m_Transform.translation;
    if (this->product != nullptr) {
        this->product->m_Transform.translation = this->m_Transform.translation;
        this->product->SetItemSize(cam.scale);
        this->product->Update();
    }

    cooldown += rate * static_cast<float>(chainLen) * MULTIPLIER_MINE;
    if ((cooldown >= 1)
        && (product != nullptr)
        && (next == nullptr)) {
        cooldown -= 1;
        ejector->prep = product->copy();
        ejector->prepProgress = 1;
        ejector->prep->m_Transform.translation = m_Transform.translation;
        ejector->prep->SetItemSize(cam.scale);
        ejector->prep->Update();
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

void Miner::Restore(int arg) {
    std::cout << "called miner restore\n";
    restored = true;
    if (ejector->prep != nullptr) {
        cooldown += 1;
        ejector->prep = nullptr;
    }
}

void Miner::Promote() {
    ejector->Promote();
}

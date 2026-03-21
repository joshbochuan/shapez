//
// Created by joshb on 2026/3/20.
//

#include "Miner.hpp"
#include "Machine.hpp"
#include "Util/Image.hpp"
#include <iostream>

Miner::Miner(int x, int y, int r, std::shared_ptr<Item> product)
    : Machine(x, y, r, MINE_RATE) {
    this->cooldown = 0;
    this->product = product;
    if (product != nullptr) {
        this->product->MinerZIndex(44);
        this->AddChild(this->product);
    }
    this->ejector = std::make_shared<ItemEjector>(x, y, r);
    ejectors[std::make_tuple(x, y, r)] = this->ejector;
    this->AddChild(this->ejector);
    this->SetDrawable(std::make_shared<Util::Image>("../Resources/Sprites/buildings/miner.png"));
    this->m_Transform.rotation = M_PI * 0.5 * r;
    this->SetZIndex(42 + (x+y)%2);
    this->cover = std::make_shared<Util::GameObject>();
    cover->SetDrawable(std::make_shared<Util::Image>("../Resources/Sprites/buildings/miner-cover.png"));
    cover->SetZIndex(40);
    this->AddChild(cover);
}

void Miner::Update() {
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.1f;
    this->m_Transform.scale.y = cam.scale.y * 1.1f;
    cover->m_Transform.scale.x = cam.scale.x * 0.85f;
    cover->m_Transform.scale.y = cam.scale.y * 0.85f;
    cover->m_Transform.translation = this->m_Transform.translation;
    if (this->product != nullptr) {
        this->product->m_Transform.translation = this->m_Transform.translation;
        this->product->m_Transform.scale.x = 0.25f * cam.scale.x;
        this->product->m_Transform.scale.y = 0.25f * cam.scale.y;
        this->product->Update();
    }

    cooldown += rate;
    if ((cooldown >= 1)
        &&(product != nullptr)
        && (ejector->item == nullptr)) {
        cooldown -= 1;
        ejector->item = product->copy();
        ejector->AddChild(ejector->item);
        ejector->progress = 0;
    }
    if (cooldown > 1) {cooldown = 1;}

    ejector->Update();
}

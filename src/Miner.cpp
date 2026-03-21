//
// Created by joshb on 2026/3/20.
//

#include "Miner.hpp"
#include "Machine.hpp"
#include "Util/Image.hpp"

Miner::Miner(int x, int y, int r, float rate, std::shared_ptr<Item> product)
    : Machine(x, y, r, rate) {
    this->product = product;
    this->ejector = std::make_shared<ItemEjector>(x, y, r, rate);
    ejectors[std::make_tuple(x, y, r)] = this->ejector;
    this->AddChild(this->ejector);
    this->SetDrawable(std::make_shared<Util::Image>("../Resources/Sprites/buildings/miner.png"));
    this->m_Transform.rotation = M_PI * 0.5 * r;
    this->SetZIndex(10 + (x+y)%2);
}

void Miner::Update() {
    // update position
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * (192.0/174.0);
    this->m_Transform.scale.y = cam.scale.y * (192.0/174.0);

    if ((product != nullptr) && (ejector->item == nullptr)) {
        ejector->item = product->copy();
        ejector->AddChild(ejector->item);
        ejector->progress = 0;
    }

    ejector->Update();
}

//
// Created by joshb on 2026/3/20.
//

#include "Miner.hpp"
#include "Machine.hpp"
#include "Util/Image.hpp"

Miner::Miner(int x, int y, int r, float rate, std::shared_ptr<Item> product)
    : Machine(x, y, r, rate) {
    this->product = product;

    this->SetDrawable(std::make_shared<Util::Image>("../Resources/Sprites/buildings/miner.png"));
    this->m_Transform.rotation = M_PI * 0.5 * r;
    this->SetZIndex(10);
}

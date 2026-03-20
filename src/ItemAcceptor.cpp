//
// Created by joshb on 2026/3/20.
//

#include "ItemAcceptor.hpp"

ItemAcceptor::ItemAcceptor(int x, int y, int r, float rate)
    : Machine(x, y, r, rate) {
    this->item = nullptr;
    this->progress = 0;
}

void ItemAcceptor::Update() {
    if (item == nullptr) {return;}
    if (progress < 1) {progress += rate;}
}
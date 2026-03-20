//
// Created by joshb on 2026/3/20.
//

#include "ItemAcceptor.hpp"

ItemAcceptor::ItemAcceptor(float rate) {
    this->item = nullptr;
    this->rate = rate;
    this->progress = 0;
}

void ItemAcceptor::Update() {
    if (item == nullptr) {return;}
    if (progress < 1) {progress += rate;}
}
//
// Created by joshb on 2026/3/20.
//
#include "ItemEjector.hpp"

ItemEjector::ItemEjector(float rate) {
    this->item = nullptr;
    this->rate = rate;
    this->progress = 0;
}

void ItemEjector::Update() {
    if (item == nullptr) {return;}
    if (progress < 1) {progress += rate;}
}
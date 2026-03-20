//
// Created by joshb on 2026/3/20.
//
#include "ItemEjector.hpp"

ItemEjector::ItemEjector(int x, int y, int r, float rate)
    : Machine(x, y, r, rate) {
    this->item = nullptr;
    this->progress = 0;
}

void ItemEjector::Update() {
    if (item == nullptr) {return;}
    if (progress < 1) {progress += rate;}
}
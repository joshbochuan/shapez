//
// Created by User on 2026/3/20.
//

#include "Belt.hpp"

Belt::Belt(int x, int y, int r)
    : Machine(x, y, r) {}

void Belt::Update() {
    // add progress to both accept progress and eject progress
    float advanceProgress = 0.1;
    this->ejectProgress += advanceProgress;
    this->acceptProgress += advanceProgress;

    // if item can be transferred from input slot to output slot
    if ((this->ejectProgress >= 1) && (this->ejectSlot == nullptr)) {
        this->ejectSlot = this->acceptSlot;
        this->acceptSlot = nullptr;
        this->ejectProgress = this->acceptProgress-1;
    }

    // accepter determines whether to take
    // todo: finish accepter and ejecter logic

}
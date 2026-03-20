//
// Created by User on 2026/3/20.
//

#include <iostream>
#include "Belt.hpp"

#include "BeltForward.hpp"
#include "Global.hpp"

Belt::Belt(int x, int y, int r, float rate)
    : Machine(x, y, r, rate) {
    this->acceptor = std::make_shared<ItemAcceptor>(rate);
    this->ejector = std::make_shared<ItemEjector>(rate);
}

void Belt::Update() {
    // add progress to both accept progress and eject progress
    this->m_Transform.translation.x = ((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x;
    this->m_Transform.translation.y = ((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y;
    this->m_Transform.scale = cam.scale;

    // if item can be transferred from input slot to output slot
    if ((acceptor->item != nullptr) && (acceptor->progress >= 1) && (ejector->item == nullptr)) {
        ejector->item = acceptor->item;
        acceptor->item = nullptr;
        ejector->progress = acceptor->progress-1;
        acceptor->progress = 0;
    }

    acceptor->Update();
    ejector->Update();


    // accepter determines whether to take
    // todo: finish accepter and ejecter logic

}
//
// Created by joshb on 2026/3/20.
//

#include <cmath>
#include "BeltForward.hpp"
#include "Util/Animation.hpp"

BeltForward::BeltForward(int x, int y, int r, float rate)
    : Belt(x, y, r, rate), m_Animation(std::make_shared<Util::Animation>(
        std::vector<std::string>{
            "../Resources/sprites/belt/built/forward_0.png",
            "../Resources/sprites/belt/built/forward_1.png",
            "../Resources/sprites/belt/built/forward_2.png",
            "../Resources/sprites/belt/built/forward_3.png",
            "../Resources/sprites/belt/built/forward_4.png",
            "../Resources/sprites/belt/built/forward_5.png",
            "../Resources/sprites/belt/built/forward_6.png",
            "../Resources/sprites/belt/built/forward_7.png",
            "../Resources/sprites/belt/built/forward_8.png",
            "../Resources/sprites/belt/built/forward_9.png",
            "../Resources/sprites/belt/built/forward_10.png",
            "../Resources/sprites/belt/built/forward_11.png",
            "../Resources/sprites/belt/built/forward_12.png",
            "../Resources/sprites/belt/built/forward_13.png"
        },
      true, 30, true, 0)) {
    SetDrawable(m_Animation);
    this->SetZIndex(4);
}

void BeltForward::Update() {
    Belt::Update();
    // updates item's position

    float p1, p2;
    if (acceptor->item != nullptr) {
        acceptor->item->m_Transform.translation.x = this->m_Transform.translation.x;
        p2 = this->m_Transform.translation.y;
        p1 = p2 - 96;
        acceptor->item->m_Transform.translation.y = std::round(p1 * (1-acceptor->progress) + p2 * acceptor->progress);
        acceptor->item->Update();
    }

    if (ejector->item != nullptr) {
        ejector->item->m_Transform.translation.x = this->m_Transform.translation.x;
        p1 = this->m_Transform.translation.y;
        p2 = p1 + 96;
        ejector->item->m_Transform.translation.y = std::round(p1 * (1-ejector->progress) + p2 * ejector->progress);
        ejector->item->Update();
    }
}
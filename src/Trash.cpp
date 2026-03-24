//
// Created by joshb on 2026/3/21.
//

#include "Trash.hpp"
#include "Global.hpp"
#include "Util/Image.hpp"
#include <iostream>

Trash::Trash(int x, int y)
    : Machine(x, y, 0, BELT_RATE, MachineName::TRASH) {
    // acceptor on all side
    for (int i=0; i<4; i++) {
        this->m_Acceptors.push_back(std::make_shared<ItemAcceptor>(x, y, i));
        this->AddChild(this->m_Acceptors[i]);
    }
    this->SetDrawable(std::make_shared<Util::Image>("../Resources/Sprites/buildings/trash.png"));
    this->SetZIndex(46 + (x+y)%2);
}

void Trash::Init() {
    MapMachines[{x, y}] = shared_from_this();
    for (int i=0; i<4; i++) {m_Acceptors[i]->Init();}
}

void Trash::Delete() {
    MapMachines.erase({x, y});
    for (int i=0; i<4; i++) {m_Acceptors[i]->Delete();}
}

void Trash::Update() {
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.125;
    this->m_Transform.scale.y = cam.scale.y * 1.125;

    for (int i=0; i<4; i++) {
        m_Acceptors[i]->Update();
        if ((m_Acceptors[i]->item != nullptr) && (m_Acceptors[i]->progress >= 1)) {
            m_Acceptors[i]->item->Update();
            m_Acceptors[i]->RemoveChild(this->m_Acceptors[i]->item);
            std::weak_ptr<Item> tmp = m_Acceptors[i]->item;
            m_Acceptors[i]->item = nullptr;
            m_Acceptors[i]->progress = 0;
        }
    }
}

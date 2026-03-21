//
// Created by joshb on 2026/3/21.
//
#include "Hub.hpp"
#include "Global.hpp"
#include "Util/Image.hpp"
#include "Shape.hpp"
#include "Util/Text.hpp"

Hub::Hub()
    : Machine(0, 0, 0, BELT_RATE) {
    this->level = 1;
    this->targetItem = std::make_shared<Shape>("CuCuCuCu"); // first level
    this->progress = 0;
    this->targetAmount = 30; // first level target amount

    this->SetDrawable(std::make_shared<Util::Image>("../Resources/Sprites/buildings/hub.png"));
    this->SetZIndex(50);

    levelTxt = std::make_shared<Text>("LVL", 40, Util::Color::FromRGB(255, 255, 255));
    levelNum = std::make_shared<Text>("1", 64, Util::Color::FromRGB(255, 255, 255));
    this->AddChild(levelTxt);
    this->AddChild(levelNum);
    levelTxt->SetZIndex(51);
    levelNum->SetZIndex(51);

    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(-2, -2, 0));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(-1, -2, 0));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(-0, -2, 0));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(1, -2, 0));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(1, -2, 1));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(1, -1, 1));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(1, 0, 1));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(1, 1, 1));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(1, 1, 2));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(0, 1, 2));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(-1, 1, 2));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(-2, 1, 2));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(-2, 1, 3));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(-2, 0, 3));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(-2, -1, 3));
    m_Acceptors.push_back(std::make_shared<ItemAcceptor>(-2, -2, 3));

    std::shared_ptr<ItemAcceptor> a;
    for (int i=0; i<m_Acceptors.size(); i++) {
        a = m_Acceptors[i];
        acceptors[std::make_tuple(a->x, a->y, a->r)] = a;
        this->AddChild(a);
    }
}

void Hub::Update() {
    this->m_Transform.translation.x = std::round(((192.0*x) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*y) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.12; // adding tiny size to avoid gap
    this->m_Transform.scale.y = cam.scale.y * 1.12;

    levelTxt->m_Transform.translation.x = m_Transform.translation.x - cam.scale.x * 244;
    levelTxt->m_Transform.translation.y = m_Transform.translation.y + cam.scale.y * 292;
    levelTxt->m_Transform.scale.x = cam.scale.x * 1;
    levelTxt->m_Transform.scale.y = cam.scale.y * 1;

    levelNum->m_Transform.translation.x = m_Transform.translation.x - cam.scale.x * 244;
    levelNum->m_Transform.translation.y = m_Transform.translation.y + cam.scale.y * 240;
    levelNum->m_Transform.scale.x = cam.scale.x * 1;
    levelNum->m_Transform.scale.y = cam.scale.y * 1;

    for (int i=0; i<m_Acceptors.size(); i++) {
        m_Acceptors[i]->Update();
        if (m_Acceptors[i]->item == nullptr) {continue;}
        if (m_Acceptors[i]->progress < 1) {continue;}

        if (targetItem == m_Acceptors[i]->item) {progress++;}

        m_Acceptors[i]->item->Update();
        m_Acceptors[i]->RemoveChild(this->m_Acceptors[i]->item);
        std::weak_ptr<Item> tmp = m_Acceptors[i]->item;
        m_Acceptors[i]->item = nullptr;
        m_Acceptors[i]->progress = 0;
    }

    if (progress >= targetAmount) {
        progress = 0;
        level++;
        levelNum->m_Text->SetText(std::to_string(level));
    }

}

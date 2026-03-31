//
// Created by joshb on 2026/3/21.
//
#include "buildings/Hub.hpp"

#include <iostream>

#include "World.hpp"
#include "Util/Image.hpp"
#include "Shape.hpp"
#include "Util/Text.hpp"
#include "BigNumStr.hpp"
using namespace World;

Hub::Hub()
    : Machine(0, 0, 0, BELT_RATE, MachineName::HUB) {
    levelUpSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/level_complete.wav");

    this->level = 1;
    auto [a, b, c] = levelTargets[0];
    this->targetItem = std::make_shared<Shape>(a); // first level
    this->progress = 0;
    this->targetAmount = b;

    this->targetItem->MachineItemZIndex(51);
    this->AddChild(this->targetItem);

    this->SetDrawable(std::make_shared<Util::Image>("../Resources/Sprites/buildings/hub.png"));
    this->SetZIndex(50);

    levelTxt = std::make_shared<Text>("LVL", 40, Util::Color::FromRGB(255, 255, 255));
    levelNumTxt = std::make_shared<Text>("1", 64, Util::Color::FromRGB(255, 255, 255));
    deliverTxt = std::make_shared<Text>("DELIVER", 64, Util::Color::FromRGB(100, 102, 110));
    progressTxt = std::make_shared<Text>(BigNumStr(progress), 160, Util::Color::FromRGB(100, 102, 110));
    targetTxt = std::make_shared<Text>("/ " + BigNumStr(targetAmount), 80, Util::Color::FromRGB(164, 166, 176));
    toUnlockTxt = std::make_shared<Text>("TO UNLOCK", 64, Util::Color::FromRGB(100, 102, 110));
    lockedItemTxt = std::make_shared<Text>(c, 56, Util::Color::FromRGB(253, 7, 82));

    this->AddChild(levelTxt);
    this->AddChild(levelNumTxt);
    this->AddChild(targetTxt);
    this->AddChild(progressTxt);
    this->AddChild(toUnlockTxt);
    this->AddChild(lockedItemTxt);
    this->AddChild(deliverTxt);
    levelTxt->SetZIndex(51);
    levelNumTxt->SetZIndex(51);
    deliverTxt->SetZIndex(51);
    progressTxt->SetZIndex(51);
    targetTxt->SetZIndex(51);
    toUnlockTxt->SetZIndex(51);
    lockedItemTxt->SetZIndex(51);

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

    for (int i=0; i<m_Acceptors.size(); i++) {this->AddChild(m_Acceptors[i]);}
}

void Hub::Init() {
    for (int i=-2; i<2; i++) {
        for (int j=-2; j<2; j++) {
            MapMachines[{i, j}] = shared_from_this();
        }
    }
    for (int i=0; i<m_Acceptors.size(); i++) {
        m_Acceptors[i]->Init();
    }
}

void Hub::Delete() {
    throw std::invalid_argument("Hub can't be deleted");
    for (int i=-2; i<2; i++) {
        for (int j=-2; j<2; j++) {
            MapMachines.erase({i, j});
        }
    }
    for (int i=0; i<m_Acceptors.size(); i++) {
        m_Acceptors[i]->Delete();
    }
}

std::shared_ptr<Shape> GenerateRandomTarget(int seed, int level) {
    return std::make_shared<Shape>("CuCuCuCu");
}

void Hub::Update() {
    for (int i=0; i<m_Acceptors.size(); i++) {
        m_Acceptors[i]->Update();
        if (m_Acceptors[i]->item == nullptr) {continue;}
        if (m_Acceptors[i]->progress < 1) {continue;}

        if (targetItem->getCode() == m_Acceptors[i]->item->getCode()) {progress++;}

        m_Acceptors[i]->item->Update();
        m_Acceptors[i]->RemoveChild(this->m_Acceptors[i]->item);
        m_Acceptors[i]->item = nullptr;
        m_Acceptors[i]->progress = 0;
    }

    // level up
    if (progress >= targetAmount) {
        progress = 0;
        level++;

        RemoveChild(targetItem);

        if (level <= levelTargets.size()) {
            auto [a, b, c] = levelTargets[level-1];
            targetItem = std::make_shared<Shape>(a);
            targetAmount = b;
            lockedItemTxt->m_Text->SetText(c);
        }
        else {
            targetItem = GenerateRandomTarget(SEED, level);
            targetAmount = 9999999;
            toUnlockTxt->m_Text->SetText("Next Level");
        }

        targetItem->MachineItemZIndex(51);
        AddChild(targetItem);

        levelNumTxt->m_Text->SetText(std::to_string(level));
        targetTxt->m_Text->SetText("/ " + BigNumStr(targetAmount));

        levelUpSFX->Play();
    }
    progressTxt->m_Text->SetText(BigNumStr(progress));

    this->m_Transform.translation.x = std::round(((192.0f*static_cast<float>(x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0f*static_cast<float>(y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.12f;
    this->m_Transform.scale.y = cam.scale.y * 1.12f;

    targetItem->m_Transform.translation.x = m_Transform.translation.x - cam.scale.x * 128;
    targetItem->m_Transform.translation.y = m_Transform.translation.y + cam.scale.y * 32;
    targetItem->SetItemSize(cam.scale * 2.4f);
    targetItem->Update();

    levelTxt->m_Transform.translation.x = m_Transform.translation.x - cam.scale.x * 244;
    levelTxt->m_Transform.translation.y = m_Transform.translation.y + cam.scale.y * 292;
    levelTxt->m_Transform.scale = cam.scale;

    levelNumTxt->m_Transform.translation.x = m_Transform.translation.x - cam.scale.x * 244;
    levelNumTxt->m_Transform.translation.y = m_Transform.translation.y + cam.scale.y * 240;
    levelNumTxt->m_Transform.scale = cam.scale;

    deliverTxt->m_Transform.translation.x = m_Transform.translation.x + cam.scale.x * 0;
    deliverTxt->m_Transform.translation.y = m_Transform.translation.y + cam.scale.y * 256;
    deliverTxt->m_Transform.scale = cam.scale;

    progressTxt->m_Transform.translation.x = m_Transform.translation.x + cam.scale.x * 32;
    progressTxt->m_Transform.translation.x += progressTxt->m_Text->GetSize().x * 0.5f * cam.scale.x;
    progressTxt->m_Transform.translation.y = m_Transform.translation.y + cam.scale.y * 88;
    progressTxt->m_Transform.scale = cam.scale;

    targetTxt->m_Transform.translation.x = m_Transform.translation.x + cam.scale.x * 32;
    targetTxt->m_Transform.translation.x += targetTxt->m_Text->GetSize().x * 0.5f * cam.scale.x;
    targetTxt->m_Transform.translation.y = m_Transform.translation.y - cam.scale.y * 32;
    targetTxt->m_Transform.scale = cam.scale;

    toUnlockTxt->m_Transform.translation.x = m_Transform.translation.x + cam.scale.x * 0;
    toUnlockTxt->m_Transform.translation.y = m_Transform.translation.y - cam.scale.y * 160;
    toUnlockTxt->m_Transform.scale = cam.scale;

    lockedItemTxt->m_Transform.translation.x = m_Transform.translation.x - cam.scale.x * 0;
    lockedItemTxt->m_Transform.translation.y = m_Transform.translation.y - cam.scale.y * 256;
    lockedItemTxt->m_Transform.scale = cam.scale;
}


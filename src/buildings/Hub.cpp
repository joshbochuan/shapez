//
// Created by joshb on 2026/3/21.
//
#include "buildings/Hub.hpp"

#include <iostream>
#include <sstream>
#include "World.hpp"
#include "Util/Image.hpp"
#include "../../include/items/Shape.hpp"
#include "Util/Text.hpp"
#include "BigNumStr.hpp"
#include "Util/Input.hpp"
using namespace World;

Hub::Hub()
    : Machine(0, 0, 0, BELT_RATE, MachineName::HUB)
    , throughputCounter(5) {
    levelUpSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/level_complete.wav");

    auto [a, b, c] = levelTargets[0];
    this->targetItem = std::make_shared<Shape>(a); // first level
    this->targetAmount = b;

    this->targetItem->MachineItemZIndex(51);
    this->AddChild(this->targetItem);

    this->SetDrawable(std::make_shared<OptiImage>("../Resources/Sprites/buildings/hub.png"));
    this->SetZIndex(50);

    levelTxt = std::make_shared<Text>("LVL", 40, Util::Color::FromRGB(255, 255, 255));
    levelNumTxt = std::make_shared<Text>("1", 64, Util::Color::FromRGB(255, 255, 255));
    deliverTxt = std::make_shared<Text>("DELIVER", 64, Util::Color::FromRGB(100, 102, 110));
    progressTxt = std::make_shared<Text>(BigNumStr(PROGRESS), 160, Util::Color::FromRGB(100, 102, 110));
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

    for (int i = 0; i < m_Acceptors.size(); i++) { this->AddChild(m_Acceptors[i]); }
}

std::string Hub::getSaveString() {
    return "HUB";
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
    for (int i=-2; i<2; i++) {
        for (int j=-2; j<2; j++) {
            MapMachines.erase({i, j});
        }
    }
    for (int i=0; i<m_Acceptors.size(); i++) {
        m_Acceptors[i]->Delete();
    }
}

std::string GenerateRandomLayer(int seed, int level, int layer, bool hasMissingPiece, int colorMain) {
    bool hasUncolored = (level >= 36);
    int randomVal = pseudo_random(seed, level, layer);

    // each color main takes 6 bits
    // int colorMain = (0b111111 & (randomVal >> 0));
    int colorNeighborA = (0b111111 & (randomVal >> 8));
    int colorNeighborB = (0b111111 & (randomVal >> 16));
    // each shape takes 2 bits
    int shapeA = (0b11 & (randomVal >> 6));
    int shapeB = (0b11 & (randomVal >> 14));
    int shapeC = (0b11 & (randomVal >> 22));

    if (hasUncolored) {colorMain = colorMain % 8;}
    else {colorMain = (colorMain % 7) + 1;}

    std::vector<std::vector<int>> neighbors = {
        {0b100, 0b010, 0b001}, // 0b000
        {0b101, 0b011, 0b000}, // 0b001
        {0b110, 0b011, 0b000}, // 0b010
        {0b111, 0b001, 0b010}, // 0b011
        {0b110, 0b101, 0b000}, // 0b100
        {0b001, 0b111, 0b100}, // 0b101
        {0b010, 0b100, 0b111}, // 0b110
        {0b011, 0b101, 0b110} // 0b111
    };

    int colorA = neighbors[colorMain][colorNeighborA % 3];
    if ((colorA == 0) && (!hasUncolored)) {colorA = neighbors[colorMain][colorNeighborA % 2];}
    int colorB = neighbors[colorMain][colorNeighborB % 3];
    if ((colorB == 0) && (!hasUncolored)) {colorB = neighbors[colorMain][colorNeighborB % 2];}

    int missingQuarter = 3 & (randomVal >> 24);
    int symmetryProp = 3 & (randomVal >> 26);
    bool symmetryType = 1 & (randomVal >> 28);

    if (((shapeA == 3) || (shapeC == 3)) && (symmetryProp < 2)) {
        symmetryType = true;
    }

    std::string shapes = "CRSW";
    std::string colors = "ubgcrpyw";
    std::vector<std::string> quads = {"--", "--", "--", "--"};
    if (symmetryType) {
        quads[0] = shapes[shapeC];
        quads[0] += colors[colorMain];
        quads[1] = shapes[shapeA];
        quads[1] += colors[colorA];
        quads[2] = shapes[shapeC];
        quads[2] += colors[colorMain];
        quads[3] = shapes[shapeA];
        quads[3] += colors[colorA];
    }
    else if (symmetryProp == 0) {
        quads[0] = shapes[shapeC];
        quads[0] += colors[colorMain];
        quads[1] = shapes[shapeC];
        quads[1] += colors[colorMain];
        quads[2] = shapes[shapeA];
        quads[2] += colors[colorA];
        quads[3] = shapes[shapeA];
        quads[3] += colors[colorA];
    }
    else if (symmetryProp == 1) {
        quads[0] = shapes[shapeA];
        quads[0] += colors[colorA];
        quads[1] = shapes[shapeC];
        quads[1] += colors[colorMain];
        quads[2] = shapes[shapeC];
        quads[2] += colors[colorMain];
        quads[3] = shapes[shapeA];
        quads[3] += colors[colorA];
    }
    else if (symmetryProp == 2) {
        quads[0] = shapes[shapeC];
        quads[0] += colors[colorMain];
        quads[1] = shapes[shapeA];
        quads[1] += colors[colorA];
        quads[2] = shapes[shapeC];
        quads[2] += colors[colorMain];
        quads[3] = shapes[shapeB];
        quads[3] += colors[colorB];
    }
    else if (symmetryProp == 3) {
        quads[0] = shapes[shapeA];
        quads[0] += colors[colorA];
        quads[1] = shapes[shapeC];
        quads[1] += colors[colorMain];
        quads[2] = shapes[shapeB];
        quads[2] += colors[colorB];
        quads[3] = shapes[shapeC];
        quads[3] += colors[colorMain];
    }
    if (hasMissingPiece && missingQuarter < 4) {quads[missingQuarter] = "--";}
    return quads[0] + quads[1] + quads[2] + quads[3];
}

std::shared_ptr<Shape> GenerateRandomTarget(int seed, int level) {
    int layerCnt = 2;
    if (level >= 76) {layerCnt = 4;}
    else if (level >= 51) {layerCnt = 3;}

    std::string code = "";
    int randomVal = pseudo_random(seed, level, 0);
    int missingQuarterLayer = randomVal % 8;
    int colorMain = randomVal >> 3;
    colorMain = (colorMain % 6) + 1;

    code = GenerateRandomLayer(seed, level, 0, (0==missingQuarterLayer) && (level>=76), colorMain);
    for (int i=1; i<layerCnt; i++) {
        code += ":" + GenerateRandomLayer(seed, level, i, (i==missingQuarterLayer) && (level>=76), colorMain);
    }
    return std::make_shared<Shape>(code);
}

void Hub::LoadState() { // properly update contents after loading a save
    throughputCounter.Reset();
    RemoveChild(targetItem);
    if (LEVEL <= levelTargets.size()) {
        auto [a, b, c] = levelTargets[LEVEL-1];
        targetItem = std::make_shared<Shape>(a);
        targetAmount = b;
        lockedItemTxt->m_Text->SetText(c);
    }
    else {
        targetItem = GenerateRandomTarget(SEED, LEVEL);
        targetAmount = -4.0f - 0.25f * static_cast<float>(LEVEL - 21);
        if (targetAmount < -200) {targetAmount = -200;}
        lockedItemTxt->m_Text->SetText("Next Level");
    }
    targetItem->MachineItemZIndex(51);
    AddChild(targetItem);

    levelNumTxt->m_Text->SetText(std::to_string(LEVEL));
    if (targetAmount >= 0) {
        targetTxt->SetVisible(true);
        targetTxt->m_Text->SetText("/ " + BigNumStr(static_cast<int>(targetAmount)));
        progressTxt->m_Text->SetText(BigNumStr(PROGRESS));
    }
    else {
        targetTxt->SetVisible(false);
        progressTxt->m_Text->SetText(std::to_string(-targetAmount) + " / s");
    }

}

void Hub::Update() {
    restored = false;

    int frameProgress = 0;
    for (int i=0; i<m_Acceptors.size(); i++) {
        m_Acceptors[i]->Update();
        if (m_Acceptors[i]->item == nullptr) {continue;}
        if (m_Acceptors[i]->progress < 1) {continue;}

        if (targetItem->getCode() == m_Acceptors[i]->item->getCode()) {frameProgress++;}
        warehouse[m_Acceptors[i]->item->getCode()];
        warehouse[m_Acceptors[i]->item->getCode()]++; // ik this is scuffed

        m_Acceptors[i]->item->Update();
        m_Acceptors[i]->RemoveChild(this->m_Acceptors[i]->item);
        m_Acceptors[i]->item = nullptr;
        m_Acceptors[i]->progress = 0;
    }

    // level up
    PROGRESS += frameProgress;
    throughputCounter.Push(frameProgress);
    if ((targetAmount >= 0) && (PROGRESS >= static_cast<int>(targetAmount))
        || (targetAmount < 0) && (throughputCounter.throughput >= -targetAmount)
        || (Util::Input::IsKeyDown(Util::Keycode::KP_PLUS) && CHEATS)) {
        PROGRESS = 0;
        throughputCounter.Reset();
        LEVEL++;
        LoadState();
        levelUpSFX->Play();
    }

    this->m_Transform.translation.x = std::round(((192.0f*static_cast<float>(x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0f*static_cast<float>(y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.12f;
    this->m_Transform.scale.y = cam.scale.y * 1.12f;

    if (targetAmount >= 0) {
        targetTxt->SetVisible(true);
        targetTxt->m_Text->SetText("/ " + BigNumStr(targetAmount));
        progressTxt->m_Text->SetText(BigNumStr(PROGRESS));
        progressTxt->SetPivot({-progressTxt->m_Text->GetSize().x * 0.5f, 0});
        progressTxt->m_Transform.translation.x = m_Transform.translation.x + cam.scale.x * 32;
        progressTxt->m_Transform.translation.y = m_Transform.translation.y + cam.scale.y * 88;
        progressTxt->m_Transform.scale = cam.scale;
    }
    else {
        targetTxt->SetVisible(false);
        std::ostringstream oss;
        oss << -targetAmount;
        progressTxt->m_Text->SetText(oss.str() + " / s");
        progressTxt->SetPivot({0, 0});
        progressTxt->m_Transform.translation.x = m_Transform.translation.x + cam.scale.x * 168;
        progressTxt->m_Transform.translation.y = m_Transform.translation.y + cam.scale.y * 16;
        progressTxt->m_Transform.scale = cam.scale * 0.5f;
    }

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

void Hub::Restore(int arg, std::shared_ptr<ItemEjector> from) {
    // ts shouldn't happen either
    restored = true;
}

void Hub::Promote() {
    for (auto& ac: m_Acceptors) {ac->Promote();}
}

//
// Created by joshb on 2026/3/31.
//
#include "App.hpp"

#include "Test.hpp"
#include "World.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "buildings/Belt.hpp"
#include "buildings/Rotator.hpp"
#include "buildings/Cutter.hpp"
#include "buildings/Hub.hpp"
#include "buildings/Balancer.hpp"
#include "buildings/Tunnel.hpp"
#include "buildings/Stacker.hpp"
#include "buildings/Painter.hpp"
#include "buildings/Mixer.hpp"
#include "buildings/Miner.hpp"
#include "buildings/Trash.hpp"
#include "items/Shape.hpp"
#include "items/Color.hpp"
#include "../../include/scenes/GameScene.hpp"
#include "App.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>
#include "version.h"

#include "scenes/PauseScene.hpp"
#include "scenes/UpgradeScene.hpp"
#include "Util/Time.hpp"
using namespace World;

void GameScene::UserMoveCamera() {
    glm::vec2 cursorPos = Util::Input::GetCursorPosition();

    if (Util::Input::IsKeyDown(Util::Keycode::SPACE) && (movingMarkerCooldown == 0)) {
        movingMarker = {0, 0};
        movingMarkerCooldown = FPS_CAP;
    }
    if (movingMarkerCooldown > 0) {
        cam.translation += ((60.0f / static_cast<float>(FPS_CAP)) * 0.1f * (movingMarker - cam.translation));
        movingMarkerCooldown--;
        lastMousePos = cursorPos;
        return;
    }

    float camSpeed = 10;
    if (Util::Input::IsKeyPressed(Util::Keycode::LSHIFT)) {camSpeed = 50;}

    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && (!toolbar->hovered) && heldMachine==MachineName::NONE) {
        cam.translation.x -= (cursorPos.x-lastMousePos.x) / cam.scale.x;
        cam.translation.y -= (cursorPos.y-lastMousePos.y) / cam.scale.y;
    }
    else {
        if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
            cam.translation.y += camSpeed / cam.scale.y;
        }
        if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
            cam.translation.y -= camSpeed / cam.scale.y;
        }
        if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
            cam.translation.x -= camSpeed / cam.scale.x;
        }
        if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
            cam.translation.x += camSpeed / cam.scale.x;
        }
    }

    if (Util::Input::IfScroll()) {
        auto delta = Util::Input::GetScrollDistance();
        // fix: change PTSD/src/Util/Input.cpp line 110
        if (delta.y >= 10) {delta.y = 0;}
        glm::vec2 newScale = cam.scale;
        newScale.x += delta.y * 0.1;
        newScale.y += delta.y * 0.1;
        newScale.x = std::clamp(newScale.x, 0.2f, 2.0f);
        newScale.y = std::clamp(newScale.y, 0.2f, 2.0f);
        cam.translation.x += cursorPos.x/cam.scale.x - cursorPos.x/newScale.x;
        cam.translation.y += cursorPos.y/cam.scale.y - cursorPos.y/newScale.y;
        cam.scale = newScale;
    }

    lastMousePos = cursorPos;
}

std::string displayFloat(float value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;

    std::string str = oss.str();

    // Remove trailing zeros
    str.erase(str.find_last_not_of('0') + 1);

    // Remove trailing decimal point if needed
    if (!str.empty() && str.back() == '.')
        str.pop_back();

    return str;
}

void GameScene::UserSelectMachine() {
    bool unselectMachine = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB);
    for (auto& btn : buttons) {unselectMachine |= (btn->released && !btn->selected);}
    if (unselectMachine) {
        for (auto& btn : buttons) {btn->selected = false;}
        heldMachine = MachineName::NONE;
        heldR = 0;
        heldPreview->SetVisible(false);
        previewBorder->SetVisible(false);

        heldMachineTitleBackground->SetVisible(false);
        heldMachineTitle->SetVisible(false);
        heldMachineSpeedTxt->SetVisible(false);
        heldMachineRateTxt->SetVisible(false);
    }

    bool isMachineSelected = false;
    for (int i=0; i<10; i++) {
        if (buttons[i]->released && buttons[i]->selected) {
            isMachineSelected = true;
            heldIdx = i;
        }
    }

    if (Util::Input::IsKeyDown(Util::Keycode::KP_ENTER) && CHEATS) {
        isMachineSelected = true;
        heldIdx = 10;
    }

    if (!isMachineSelected) {return;}

    beltType = BeltType::FORWARD;
    rotatorType = RotatorType::ROTATE_CW;
    tunnelType = TunnelType::IN;
    tunnelUpgraded = false;
    minerChained = (LEVEL >= CHAIN_MINER_LEVEL);
    minerCheat = (heldIdx == 10);
    previewMirrored = false;
    heldR = 0; // rotation of m_MachineHeld

    if (heldIdx == 10) {
        heldIdx = 3;
        minerChained = true;
    }

    std::vector<MachineName> previewNames = {
        MachineName::BELT,
        MachineName::BALANCER,
        MachineName::TUNNEL,
        MachineName::MINER,
        MachineName::CUTTER,
        MachineName::ROTATOR,
        MachineName::STACKER,
        MachineName::MIXER,
        MachineName::PAINTER,
        MachineName::TRASH
    };
    std::vector<std::string> displayNames = {
        "CONVEYER BELT",
        "BALANCER",
        "TUNNEL",
        "EXTRACTOR",
        "CUTTER",
        "ROTATOR",
        "STACKER",
        "MIXER",
        "PAINTER",
        "TRASH"
    };
    std::vector<float> displayRates = {
        2.0f,
        4.0f,
        2.0f,
        0.4f,
        0.5f,
        1.0f,
        0.25f,
        0.4f,
        0.33f,
        2.0f
    };
    std::vector<float*> multipliers = {
        &MULTIPLIER_BELT,
        &MULTIPLIER_BELT,
        &MULTIPLIER_BELT,
        &MULTIPLIER_MINE,
        &MULTIPLIER_PROCESS,
        &MULTIPLIER_PROCESS,
        &MULTIPLIER_PROCESS,
        &MULTIPLIER_PAINT,
        &MULTIPLIER_PAINT,
        &MULTIPLIER_BELT,
    };
    std::vector<std::string> previewImagePaths = {
        "../Resources/sprites/blueprints/belt_top.png",
        "../Resources/sprites/blueprints/balancer.png",
        "../Resources/sprites/blueprints/underground_belt_entry.png",
        "../Resources/sprites/blueprints/miner.png",
        "../Resources/sprites/blueprints/cutter.png",
        "../Resources/sprites/blueprints/rotater.png",
        "../Resources/sprites/blueprints/stacker.png",
        "../Resources/sprites/blueprints/mixer.png",
        "../Resources/sprites/blueprints/painter.png",
        "../Resources/sprites/blueprints/trash.png"
    };
    std::vector<glm::vec2> previewPivots = {
        {0, 0},
        {-84, 0},
        {0, 0},
        {0, 0},
        {-84, 0},
        {0, 0},
        {-84, 0},
        {-84, 0},
        {-84, 0},
        {0, 0}
    };
    std::vector<int> previewWidth = {1, 2, 1, 1, 2, 1, 2, 2, 2, 1};
    std::vector<std::vector<glm::vec2>> previewInArrows = {
        {{0, -1}},
        {{0, -1}, {1, -1}},
        {{0, -1}},
        {},
        {{0, -1}},
        {{0, -1}},
        {{0, -1}, {1, -1}},
        {{0, -1}, {1, -1}},
        {{-1, 0}, {1, 1}},
        {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}
    };
    std::vector<std::vector<float>> previewInArrowsRotation = {
        {0},
        {0, 0},
        {0},
        {},
        {0},
        {0},
        {0, 0},
        {0, 0},
        {3, 2},
        {0, 1, 2, 3}
    };
    std::vector<std::vector<glm::vec2>> previewOutArrows = {
        {{0, 1}},
        {{0, 1}, {1, 1}},
        {}
    };

    heldMachine = previewNames[heldIdx];
    heldPreview->SetDrawable(std::make_shared<Util::Image>(previewImagePaths[heldIdx]));
    if ((heldMachine == MachineName::MINER) && (minerChained)) {
        heldPreview->SetDrawable(std::make_shared<Util::Image>("../Resources/sprites/blueprints/miner-chainable.png"));
    }
    heldPreview->SetPivot(previewPivots[heldIdx]);
    heldPreview->SetVisible(true);

    previewBorder->SetDrawable(std::make_shared<Util::Image>("../Resources/sprites/blueprints/green_border_1x" + std::to_string(previewWidth[heldIdx]) + ".png"));
    if (previewWidth[heldIdx] == 2) {
        previewBorder->SetPivot({-96, 0});
    }
    else {previewBorder->SetPivot({0, 0});}
    previewBorder->SetVisible(true);

    heldMachineTitleBackground->SetVisible(true);

    heldMachineTitle->SetText(displayNames[heldIdx]);
    heldMachineTitle->AlignToLeft();
    heldMachineTitle->SetVisible(true);

    heldMachineSpeedTxt->SetVisible(true);

    heldMachineRateTxt->SetText(displayFloat(*(multipliers[heldIdx]) * displayRates[heldIdx]) + " items / s");
    heldMachineRateTxt->AlignToLeft();
    heldMachineRateTxt->SetVisible(true);
}

void SelectBeltVariant(std::shared_ptr<OptiObject>& m_MachineHeldPreview, BeltType& beltType) {
    if (beltType == BeltType::FORWARD) {
        beltType = BeltType::LEFT;
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/belt_left.png"));
    }
    else if (beltType == BeltType::LEFT) {
        beltType = BeltType::RIGHT;
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/belt_right.png"));
    }
    else if (beltType == BeltType::RIGHT) {
        beltType = BeltType::FORWARD;
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/belt_top.png"));
    }
}

void SelectBalancerVariant(std::shared_ptr<OptiObject>& m_MachineHeldPreview, MachineName& m_MachineHeld, bool& previewMirrored) {
    if (m_MachineHeld == MachineName::BALANCER && (LEVEL >= MERGER_LEVEL)) {
        m_MachineHeld = MachineName::MERGER;
        m_MachineHeldPreview->SetVisible(true);
        previewMirrored = false;
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/balancer-merger.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    else if (m_MachineHeld == MachineName::MERGER && !previewMirrored) {
        previewMirrored = true;
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/balancer-merger-inverse.png"));
    }
    else if (m_MachineHeld == MachineName::MERGER && (LEVEL >= SPLITTER_LEVEL)) {
        m_MachineHeld = MachineName::SPLITTER;
        previewMirrored = false;
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/balancer-splitter.png"));
    }
    else if (m_MachineHeld == MachineName::SPLITTER && !previewMirrored) {
        previewMirrored = true;
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/balancer-splitter-inverse.png"));
    }
    else {
        m_MachineHeld = MachineName::BALANCER;
        previewMirrored = false;
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/balancer.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
}

void SelectTunnelVariant(std::shared_ptr<OptiObject>& heldPreview, TunnelType& tunnelType, bool& tunnelUpgraded) {
    if (tunnelType == TunnelType::IN && !tunnelUpgraded) {
        tunnelType = TunnelType::OUT;
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/underground_belt_exit.png"));
    }
    else if ((tunnelType == TunnelType::OUT && !tunnelUpgraded) && (LEVEL >= TUNNEL_T2_LEVEL)) {
        tunnelType = TunnelType::IN;
        tunnelUpgraded = true;
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/underground_belt_entry-tier2.png"));
    }
    else if (tunnelType == TunnelType::IN && tunnelUpgraded) {
        tunnelType = TunnelType::OUT;
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/underground_belt_exit-tier2.png"));
    }
    else {
        tunnelType = TunnelType::IN;
        tunnelUpgraded = false;
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/underground_belt_entry.png"));
    }
}

void SelectMinerVariant(std::shared_ptr<OptiObject>& heldPreview, bool& minerChained) {
    if (!minerChained && (LEVEL >= CHAIN_MINER_LEVEL)) {
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/miner-chainable.png"));
    }
    else {
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/miner.png"));
    }
    minerChained = !minerChained;
}

void SelectCutterVariant(std::shared_ptr<OptiObject>& heldPreview) {
    return;
}

void SelectRotatorVariant(std::shared_ptr<OptiObject>& heldPreview, RotatorType& rotatorType) {
    if ((rotatorType == RotatorType::ROTATE_CW) && (LEVEL >= ROTATOR_180_LEVEL)) {
        rotatorType = RotatorType::ROTATE_180;
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/rotater-rotate180.png"));
    }
    else if ((rotatorType == RotatorType::ROTATE_180) || (rotatorType == RotatorType::ROTATE_CW)) {
        rotatorType = RotatorType::ROTATE_CCW;
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/rotater-ccw.png"));
    }
    else {
        rotatorType = RotatorType::ROTATE_CW;
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
        "../Resources/sprites/blueprints/rotater.png"));
    }
}

void SelectPainterVariant(std::shared_ptr<OptiObject>& heldPreview, bool& previewMirrored) {
    if (!previewMirrored) {
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/painter-mirrored.png"));
    }
    else {
        heldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/painter.png"));
    }
    previewMirrored = !previewMirrored;
}

void GameScene::UserSelectVariant() {
    switch (heldIdx) {
        case 0: SelectBeltVariant(heldPreview, beltType); break;
        case 1: SelectBalancerVariant(heldPreview, heldMachine, previewMirrored); break;
        case 2: SelectTunnelVariant(heldPreview, tunnelType, tunnelUpgraded); break;
        case 3: break;
        case 4: SelectCutterVariant(heldPreview); break;
        case 5: SelectRotatorVariant(heldPreview, rotatorType); break;
        case 6: break;
        case 7: break;
        case 8: SelectPainterVariant(heldPreview, previewMirrored); break;
        case 9: break;
        default: break;
    }
}

void GameScene::UserPlaceMachine(int mouseX, int mouseY) {
    std::shared_ptr<Machine> MachineToAdd = nullptr;
    try {
        switch (heldMachine) {
        case MachineName::BELT:
            if (!beltPlannerOn) {MachineToAdd = std::make_shared<Belt>(mouseX, mouseY, heldR, beltType);}
            break;

        case MachineName::BALANCER:
            MachineToAdd = std::make_shared<Balancer>(mouseX, mouseY, heldR);
            break;

        case MachineName::SPLITTER:
            MachineToAdd = std::make_shared<Splitter>(mouseX, mouseY, heldR, previewMirrored);
            break;

        case MachineName::MERGER:
            MachineToAdd = std::make_shared<Merger>(mouseX, mouseY, heldR, previewMirrored);
            break;

        case MachineName::TUNNEL:
            MachineToAdd = std::make_shared<Tunnel>(mouseX, mouseY, heldR, tunnelType, tunnelUpgraded);
            break;

        case MachineName::MINER:
            if (!minerCheat) {MachineToAdd = std::make_shared<Miner>(mouseX, mouseY, heldR,nullptr, minerChained);}
            else {MachineToAdd = std::make_shared<Miner>(mouseX, mouseY, heldR, hub->targetItem->copy(), true);}
            break;

        case MachineName::CUTTER:
            MachineToAdd = std::make_shared<Cutter>(mouseX, mouseY, heldR);
            break;

        case MachineName::ROTATOR:
            MachineToAdd = std::make_shared<Rotator>(mouseX, mouseY, heldR, rotatorType);
            break;

        case MachineName::STACKER:
            MachineToAdd = std::make_shared<Stacker>(mouseX, mouseY, heldR);
            break;

        case MachineName::MIXER:
            MachineToAdd = std::make_shared<Mixer>(mouseX, mouseY, heldR);
            break;

        case MachineName::PAINTER:
            MachineToAdd = std::make_shared<Painter>(mouseX, mouseY, heldR, previewMirrored);
            break;

        case MachineName::TRASH:
            MachineToAdd = std::make_shared<Trash>(mouseX, mouseY);
            break;

        case MachineName::NONE:
            break;
        }
    }
    catch (const std::invalid_argument&) {}

    if (MachineToAdd != nullptr) {
        if (heldMachine == MachineName::BELT) {placeBeltSFX->Play();} // play place_belt.wav
        else {placeBuildingSFX->Play();} // play place_building.wav
        MachineToAdd->Init();
        m_Root.AddChild(MachineToAdd);
        LstMachines.push_back(MachineToAdd);
        MachineToAdd = nullptr;
    }
}

void GameScene::UserRemoveMachine(int mouseX, int mouseY) {
    std::shared_ptr<Machine> MachineToRemove = MapMachines[{mouseX, mouseY}];
    if (MachineToRemove == nullptr) {return;}
    if (MachineToRemove->getName() == MachineName::HUB) {return;}
    destroyBuildingSFX->Play();
    m_Root.RemoveChild(MachineToRemove);
    LstMachines.erase(std::remove(LstMachines.begin(), LstMachines.end(), MachineToRemove), LstMachines.end());
    MachineToRemove->Delete();
    if (MachineToRemove.use_count() != 1) {throw std::invalid_argument("machine not properly removed");}
    MachineToRemove = nullptr;
}

BeltPlannerPreview::BeltPlannerPreview(int x, int y, int r, BeltType type) {
    this->x = x;
    this->y = y;
    this->r = r;
    this->type = type;

    SetDrawable(lineTexture);
    SetZIndex(81);
    m_Transform.translation.x = (((192.0f*(0.5f+x)) - cam.translation.x) * cam.scale.x);
    m_Transform.translation.y = (((192.0f*(0.5f+y)) - cam.translation.y) * cam.scale.y);
    m_Transform.scale.x = cam.scale.x * 60.0f;
    m_Transform.scale.y = cam.scale.y * 192.0f;
    m_Transform.rotation = M_PI * 0.5f * static_cast<float>(r);
    SetPivot({0, 0.34375});

    arrow = std::make_shared<OptiObject>();
    arrow->SetDrawable(arrowTexture);
    arrow->m_Transform.scale = cam.scale;
    arrow->m_Transform.rotation = m_Transform.rotation;
    arrow->m_Transform.translation = m_Transform.translation;
    arrow->SetZIndex(82);
    AddChild(arrow);

    const float arrowDelta = 192.0f * (std::fmod(Util::Time::GetElapsedTimeMs() * 0.002f, 1)-1.0f);
    switch (r) {
        case 0: arrow->m_Transform.translation.y += cam.scale.y * arrowDelta; break;
        case 1: arrow->m_Transform.translation.x -= cam.scale.x * arrowDelta; break;
        case 2: arrow->m_Transform.translation.y -= cam.scale.y * arrowDelta; break;
        case 3: arrow->m_Transform.translation.x += cam.scale.x * arrowDelta; break;
        default: break;
    }
}

GameScene::GameScene() {
    lastMousePos = Util::Input::GetCursorPosition();
    glm::vec2 windowPercentVec = {1, 1}; // the window size compared to 1440p
    windowPercentVec.x = static_cast<float>(WINDOW_WIDTH)/2560.0f;
    windowPercentVec.y = static_cast<float>(WINDOW_HEIGHT)/1440.0f;
    float windowPercent = 0;
    if (windowPercentVec.x < windowPercentVec.y) {windowPercent = windowPercentVec.x;}
    else {windowPercent = windowPercentVec.y;}

    hoveredTexture = std::make_shared<Util::Image>("../Resources/ui/blobs/toolHovered.png");
    selectedTexture = std::make_shared<Util::Image>("../Resources/ui/blobs/toolSelected.png");
    lockedTexture = std::make_shared<Util::Image>("../Resources/ui/locked_building.png");

    for (int i=0; i<10; i++) {
        buttons.push_back(std::make_shared<Button>(hoveredTexture));
        buttons.back()->m_Transform.translation = {windowPercent*(-517.5 + 115*i), -windowPercent*630};
        buttons.back()->idleBackground = nullptr;
        buttons.back()->hoveredBackground = hoveredTexture;
        buttons.back()->heldBackground = hoveredTexture;
        buttons.back()->selectedBackground = selectedTexture;
        buttons.back()->SetZIndex(81);
        buttons.back()->idleSpeed = 3;
        buttons.back()->hoverSpeed = 3;
        buttons.back()->heldSpeed = 3;
        buttons.back()->selectSpeed = 3;
        buttons.back()->heldScale = 0.88;
        buttons.back()->image->SetZIndex(82);
        buttons.back()->imageScale = 0.6;
        buttons.back()->lockedImage->SetDrawable(lockedTexture);
        buttons.back()->lockedImage->SetZIndex(82);
        buttons.back()->lockedImageScale = 0.3;
        buttons.back()->locked = true;
        AddChild(buttons.back());
    }

    buttons[0]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/belt.png"));
    buttons[1]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/balancer.png"));
    buttons[2]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/underground_belt.png"));
    buttons[3]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/miner.png"));
    buttons[4]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/cutter.png"));
    buttons[5]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/rotater.png"));
    buttons[6]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/stacker.png"));
    buttons[7]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/mixer.png"));
    buttons[8]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/painter.png"));
    buttons[9]->SetImage(std::make_shared<Util::Image>("../Resources/ui/building_icons/trash.png"));

    buttons[0]->keys.push_back(Util::Keycode::NUM_1);
    buttons[1]->keys.push_back(Util::Keycode::NUM_2);
    buttons[2]->keys.push_back(Util::Keycode::NUM_3);
    buttons[3]->keys.push_back(Util::Keycode::NUM_4);
    buttons[4]->keys.push_back(Util::Keycode::NUM_5);
    buttons[5]->keys.push_back(Util::Keycode::NUM_6);
    buttons[6]->keys.push_back(Util::Keycode::NUM_7);
    buttons[7]->keys.push_back(Util::Keycode::NUM_8);
    buttons[8]->keys.push_back(Util::Keycode::NUM_9);
    buttons[9]->keys.push_back(Util::Keycode::NUM_0);

    toolbar = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/toolbar.png"));
    toolbar->lockedBackground = toolbar->idleBackground;
    toolbar->m_Transform.translation = {0, -windowPercent * 630};
    toolbar->m_Transform.scale = {windowPercent, windowPercent};
    toolbar->SetZIndex(80);
    toolbar->locked = true;
    AddChild(toolbar);

    heldPreview = std::make_shared<OptiObject>();
    heldPreview->SetVisible(false);
    heldPreview->SetZIndex(99);
    AddChild(heldPreview);

    previewBorder = std::make_shared<OptiObject>();
    previewBorder->SetVisible(false);
    previewBorder->SetZIndex(98);
    AddChild(previewBorder);

    pauseButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/main_menu_settings_idle.png"));
    pauseButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/icons/main_menu_settings.png");
    pauseButton->keys.push_back(Util::Keycode::ESCAPE);
    pauseButton->SetZIndex(81);
    pauseButton->m_Transform.translation = {(WINDOW_WIDTH>>1)-64.0f*windowPercent, (WINDOW_HEIGHT>>1)-64.0f*windowPercent};
    pauseButton->idleSpeed = 3;
    pauseButton->hoverSpeed = 3;
    pauseButton->heldSpeed = 3;
    pauseButton->selectSpeed = 3;
    pauseButton->heldScale = 0.88;
    pauseButton->imageScale = 0.6;
    pauseButton->Update();
    AddChild(pauseButton);

    saveButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/save_idle.png"));
    saveButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/icons/save.png");
    saveButton->SetZIndex(81);
    saveButton->m_Transform.translation = {(WINDOW_WIDTH>>1)-176.0f*windowPercent, (WINDOW_HEIGHT>>1)-64.0f*windowPercent};
    saveButton->idleSpeed = 3;
    saveButton->hoverSpeed = 3;
    saveButton->heldSpeed = 3;
    saveButton->selectSpeed = 3;
    saveButton->heldScale = 0.88;
    saveButton->imageScale = 0.6;
    saveButton->Update();
    AddChild(saveButton);

    upgradeButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/shop_idle.png"));
    upgradeButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/icons/shop.png");
    upgradeButton->keys.push_back(Util::Keycode::F);
    upgradeButton->SetZIndex(82);
    upgradeButton->m_Transform.translation = {(WINDOW_WIDTH>>1)-288.0f*windowPercent, (WINDOW_HEIGHT>>1)-64.0f*windowPercent};
    upgradeButton->idleSpeed = 3;
    upgradeButton->hoverSpeed = 3;
    upgradeButton->heldSpeed = 3;
    upgradeButton->selectSpeed = 3;
    upgradeButton->heldScale = 0.88;
    upgradeButton->imageScale = 0.6;
    upgradeButton->Update();
    AddChild(upgradeButton);

    tickRateText = std::make_shared<Text>("Tickrate: " + std::to_string(FPS_CAP), 28, Util::Color::FromRGB(255, 255, 255));
    tickRateText->SetPivot({0.5 * tickRateText->m_Text->GetSize().x, 0});
    tickRateText->m_Transform.scale = {windowPercent, windowPercent};
    tickRateText->m_Transform.translation.x = (WINDOW_WIDTH >> 1) - 10.0f * windowPercent;
    tickRateText->m_Transform.translation.y = 120.0f * windowPercent - (WINDOW_HEIGHT >> 1);
    tickRateText->SetVisible(false);
    tickRateText->SetZIndex(91);
    AddChild(tickRateText);

    TickTimeText = std::make_shared<Text>("Tick: 0ms", 28, Util::Color::FromRGB(255, 255, 255));
    TickTimeText->SetPivot({0.5 * TickTimeText->m_Text->GetSize().x, 0});
    TickTimeText->m_Transform.scale = {windowPercent, windowPercent};
    TickTimeText->m_Transform.translation.x = (WINDOW_WIDTH >> 1) - 10.0f * windowPercent;
    TickTimeText->m_Transform.translation.y = 90.0f * windowPercent - (WINDOW_HEIGHT >> 1);
    TickTimeText->SetVisible(false);
    TickTimeText->SetZIndex(91);
    AddChild(TickTimeText);

    fpsText = std::make_shared<Text>("FPS: 0 (0 ms)", 28, Util::Color::FromRGB(255, 255, 255));
    fpsText->SetPivot({0.5 * fpsText->m_Text->GetSize().x, 0});
    fpsText->m_Transform.scale = {windowPercent, windowPercent};
    fpsText->m_Transform.translation.x = (WINDOW_WIDTH >> 1) - 10.0f * windowPercent;
    fpsText->m_Transform.translation.y = 60.0f * windowPercent - (WINDOW_HEIGHT >> 1);
    fpsText->SetVisible(false);
    fpsText->SetZIndex(91);
    AddChild(fpsText);

    versionText = std::make_shared<Text>(APP_VERSION " @ " APP_ENV " @ " GIT_HASH, 28, Util::Color::FromRGB(255, 255, 255));
    versionText->SetPivot({0.5 * versionText->m_Text->GetSize().x, 0});
    versionText->m_Transform.scale = {windowPercent, windowPercent};
    versionText->m_Transform.translation.x = (WINDOW_WIDTH >> 1) - 10.0f * windowPercent;
    versionText->m_Transform.translation.y = 30.0f * windowPercent - (WINDOW_HEIGHT >> 1);
    versionText->SetVisible(false);
    versionText->SetZIndex(91);
    AddChild(versionText);

    beltPlannerStart = std::make_shared<OptiObject>();
    beltPlannerStart->SetDrawable(std::make_shared<OptiImage>("../Resources/sprites/blueprints/belt_planner_96px.png"));
    beltPlannerStart->SetZIndex(84);
    AddChild(beltPlannerStart);

    beltPlannerEnd = std::make_shared<OptiObject>();
    beltPlannerEnd->SetDrawable(std::make_shared<OptiImage>("../Resources/sprites/blueprints/belt_planner_60px.png"));
    beltPlannerEnd->SetZIndex(83);
    AddChild(beltPlannerEnd);

    itemPins.push_back(std::make_shared<ItemPin>(hub->targetItem->copy(), hub->targetAmount, true));
    itemPins.back()->Update();
    AddChild(itemPins.back());

    heldMachineTitleBackground = std::make_shared<OptiObject>();
    heldMachineTitleBackground->SetVisible(false);
    heldMachineTitleBackground->SetZIndex(85);
    heldMachineTitleBackground->m_Transform.scale = {windowPercent, windowPercent};
    heldMachineTitleBackground->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/ratePreviewBlob.png"));
    heldMachineTitleBackground->m_Transform.translation.x = (WINDOW_WIDTH >> 1) - 200.0f * windowPercent;
    heldMachineTitleBackground->m_Transform.translation.y = (WINDOW_HEIGHT >> 1) - 300.0f * windowPercent;
    AddChild(heldMachineTitleBackground);

    heldMachineTitle = std::make_shared<Text>(" ", 36, Util::Color::FromRGB(255, 255, 255));
    heldMachineTitle->SetZIndex(86);
    heldMachineTitle->SetVisible(false);
    heldMachineTitle->m_Transform.scale = {windowPercent, windowPercent};
    heldMachineTitle->m_Transform.translation.x = (WINDOW_WIDTH >> 1) - 365.0f * windowPercent;
    heldMachineTitle->m_Transform.translation.y = (WINDOW_HEIGHT >> 1) - 280.0f * windowPercent;
    AddChild(heldMachineTitle);

    heldMachineSpeedTxt = std::make_shared<Text>("Speed:", 28, Util::Color::FromRGB(102, 187, 106));
    heldMachineSpeedTxt->SetZIndex(86);
    heldMachineSpeedTxt->SetVisible(false);
    heldMachineSpeedTxt->AlignToLeft();
    heldMachineSpeedTxt->m_Transform.scale = {windowPercent, windowPercent};
    heldMachineSpeedTxt->m_Transform.translation.x = (WINDOW_WIDTH >> 1) - 365.0f * windowPercent;
    heldMachineSpeedTxt->m_Transform.translation.y = (WINDOW_HEIGHT >> 1) - 325.0f * windowPercent;
    AddChild(heldMachineSpeedTxt);

    heldMachineRateTxt = std::make_shared<Text>(" ", 28, Util::Color::FromRGB(187, 187, 187));
    heldMachineRateTxt->SetZIndex(86);
    heldMachineRateTxt->SetVisible(false);
    heldMachineRateTxt->m_Transform.scale = {windowPercent, windowPercent};
    heldMachineRateTxt->m_Transform.translation.x = (WINDOW_WIDTH >> 1) - 265.0f * windowPercent;
    heldMachineRateTxt->m_Transform.translation.y = (WINDOW_HEIGHT >> 1) - 325.0f * windowPercent;
    AddChild(heldMachineRateTxt);

    shapezBGM->Play();
}

std::shared_ptr<Scene> GameScene::Update() {
    saveCooldown--;
    if ((saveCooldown <= 0) || (saveButton->released)) {
        saveCooldown = 120 * FPS_CAP;
        SaveWorld("../Saves/" + WORLD_NAME + ".txt");
        if (notification != nullptr) {
            RemoveChild(notification);
            notification = nullptr;
        }
        notification = std::make_shared<Notification>(
            "Your game has been saved.",
            std::make_shared<Util::Image>("../Resources/ui/icons/notification_saved.png"));
        AddChild(notification);
    }

    if (notification != nullptr) {notification->Update();}
    if ((notification != nullptr) && (notification->frameToLive <= 0)) {
        RemoveChild(notification);
        notification = nullptr;
    }

    // update debug info
    // im too lazy to put a dedicated timer to update debug info so im using this
    if (!(saveCooldown % 120)) {
        std::ostringstream fps_ss;
        fps_ss << "FPS: " << static_cast<int>(1000.0f / Util::Time::GetDeltaTimeMs())
               << " (" << std::fixed << std::setprecision(2) << Util::Time::GetDeltaTimeMs() << " ms)";
        fpsText->m_Text->SetText(fps_ss.str());
        fpsText->SetPivot({0.5 * fpsText->m_Text->GetSize().x, 0});

        std::ostringstream tick_ss;
        tick_ss << "Tick: " << std::fixed << std::setprecision(3) << MSPT << "ms";
        TickTimeText->m_Text->SetText(tick_ss.str());
        TickTimeText->SetPivot({0.5f * TickTimeText->m_Text->GetSize().x, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::F4)) {toggleDebug = !toggleDebug;}
    tickRateText->SetVisible(toggleDebug);
    fpsText->SetVisible(toggleDebug);
    TickTimeText->SetVisible(toggleDebug);
    versionText->SetVisible(toggleDebug);

    for (auto& ip: itemPins) {ip->Update();}
    for (int i=itemPins.size()-1; i>=0; i--) {
        if (!itemPins[i]->unpinButton->released) {continue;}
        itemPins.erase(itemPins.begin()+i);
        break;
    }
    for (int i=0; i<itemPins.size(); i++) {
        itemPins[i]->m_Transform.translation.x = -static_cast<float>(WINDOW_WIDTH >> 1) + 75.0f * windowPercent;
        itemPins[i]->m_Transform.translation.y = static_cast<float>(WINDOW_HEIGHT >> 1) - (400.0f + 100.0f * i) * windowPercent;
        itemPins[i]->Update();
    }

    if (subScene != nullptr) {
        auto next = subScene->Update();
        if (next == nullptr) {
            RemoveChild(subScene);
            subScene = nullptr;
            return shared_from_this();
        }
        if (next != subScene) {
            return next;
        }
        return shared_from_this();
    }
    else if (upgradeButton->released && (LEVEL >= UPGRADE_LEVEL)) {
        for (auto& btn : buttons) {btn->selected = false;}
        heldMachine = MachineName::NONE;
        heldR = 0;
        heldPreview->SetVisible(false);
        previewBorder->SetVisible(false);

        heldMachineRateTxt->SetVisible(false);
        heldMachineSpeedTxt->SetVisible(false);
        heldMachineTitle->SetVisible(false);
        heldMachineTitleBackground->SetVisible(false);

        subScene = std::make_shared<UpgradeScene>();
        AddChild(subScene);
    }
    else if (pauseButton->released) {
        for (auto& btn : buttons) {btn->selected = false;}
        heldMachine = MachineName::NONE;
        heldR = 0;
        heldPreview->SetVisible(false);
        previewBorder->SetVisible(false);

        heldMachineRateTxt->SetVisible(false);
        heldMachineSpeedTxt->SetVisible(false);
        heldMachineTitle->SetVisible(false);
        heldMachineTitleBackground->SetVisible(false);

        long long time = std::chrono::duration_cast<std::chrono::seconds>(
                      std::chrono::system_clock::now().time_since_epoch()
                  ).count();
        subScene = std::make_shared<PauseScene>(BELT_COUNT, MACHINE_COUNT, PLAYTIME + time - LAST_PLAYED_AT);
        AddChild(subScene);
    }

    UserMoveCamera();
    pauseButton->Update();
    saveButton->Update();
    upgradeButton->Update();
    toolbar->Update();
    buttons[0]->locked = (LEVEL < BELT_LEVEL);
    buttons[1]->locked = (LEVEL < BALANCER_LEVEL);
    buttons[2]->locked = (LEVEL < TUNNEL_LEVEL);
    buttons[3]->locked = (LEVEL < MINER_LEVEL);
    buttons[4]->locked = (LEVEL < CUTTER_LEVEL);
    buttons[5]->locked = (LEVEL < ROTATOR_LEVEL);
    buttons[6]->locked = (LEVEL < STACKER_LEVEL);
    buttons[7]->locked = (LEVEL < MIXER_LEVEL);
    buttons[8]->locked = (LEVEL < PAINTER_LEVEL);
    buttons[9]->locked = (LEVEL < TRASH_LEVEL);
    for (auto& btn : buttons) {btn->Update();}
    for (int i=0; i<10; i++) {
        if (!(buttons[i]->released && buttons[i]->selected)) {continue;}
        for (int j=0; j<10; j++) {
            if (i == j) {continue;}
            buttons[j]->selected = false;
        }
    }

    int mouseX = std::floor((((Util::Input::GetCursorPosition().x / cam.scale.x) + cam.translation.x))/192.0f);
    int mouseY = std::floor((((Util::Input::GetCursorPosition().y / cam.scale.y) + cam.translation.y))/192.0f);

    if (Util::Input::IsKeyPressed(Util::Keycode::LSHIFT)
        && (heldMachine == MachineName::BELT)
        && (!beltPlannerOn)) {
        beltPlannerOn = true;
        previewBorder->SetVisible(false);
        heldPreview->SetDrawable(std::make_shared<Util::Image>("../Resources/sprites/blueprints/belt_planner_50px.png"));
        }
    if (beltStartX == mouseX && beltStartY == mouseY) {beltPreferenceSet = false;}
    if (beltPlannerOn && (!beltPreferenceSet)) {
        if (beltStartX != mouseX) {
            beltPrefersXAxis = true;
            beltPreferenceSet = true;
        }
        else if (beltStartY != mouseY) {
            beltPrefersXAxis = false;
            beltPreferenceSet = true;
        }
    }
    if (beltPlannerOn && (heldMachine != MachineName::BELT)) {
        beltPlannerOn = false;
        if (heldPreview->m_Visible) {previewBorder->SetVisible(true);}

    }
    if (beltPlannerOn && (!Util::Input::IsKeyPressed(Util::Keycode::LSHIFT))) {
        beltPlannerOn = false;
        if (heldPreview->m_Visible) {previewBorder->SetVisible(true);}
        if (beltType == BeltType::LEFT) {
            heldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/belt_left.png"));
        }
        else if (beltType == BeltType::RIGHT) {
            heldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/belt_right.png"));
        }
        else if (beltType == BeltType::FORWARD) {
            heldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/belt_top.png"));
        }
    }
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) && beltPlannerOn) {
        beltStartX = mouseX;
        beltStartY = mouseY;
    }
    if (beltPlannerOn && Util::Input::IsKeyDown(Util::Keycode::R)) {beltPrefersXAxis = !beltPrefersXAxis;}
    std::shared_ptr<Machine> MachineToAdd = nullptr;

    if ((!beltPlannerOn)
        || (beltPlannerOn && !Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))) {
        for (auto& bp: beltPlannerPreviews) {RemoveChild(bp);}
        beltPlannerPreviews.clear();
    }

    beltPlannerStart->SetVisible(false);
    beltPlannerEnd->SetVisible(false);

    if (beltPlannerOn && Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        if (beltStartX == mouseX && beltStartY == mouseY) {
            beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, beltStartY, 3, BeltType::FORWARD));
        }
        else if (beltStartX == mouseX && beltStartY < mouseY) {
            // belt goes up
            for (int y = beltStartY; y <= mouseY; y++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, y, 0, BeltType::FORWARD));
            }
        }
        else if (beltStartX == mouseX && beltStartY > mouseY) {
            // belt goes down
            for (int y = beltStartY; y >= mouseY; y--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, y, 2, BeltType::FORWARD));
            }
        }
        else if (beltStartX < mouseX && beltStartY == mouseY) {
            // belt goes right
            for (int x = beltStartX; x <= mouseX; x++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, beltStartY, 3, BeltType::FORWARD));
            }
        }
        else if (beltStartX > mouseX && beltStartY == mouseY) {
            // belt goes left
            for (int x = beltStartX; x >= mouseX; x--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, beltStartY, 1, BeltType::FORWARD));
            }
        }
        else if (beltPrefersXAxis && (beltStartX < mouseX && (beltStartY < mouseY))) {
            for (int x = beltStartX; x < mouseX; x++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, beltStartY, 3, BeltType::FORWARD));
            }
            beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(mouseX, beltStartY, 3, BeltType::LEFT));
            for (int y = beltStartY+1; y <= mouseY; y++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(mouseX, y, 0, BeltType::FORWARD));
            }
        }
        else if (beltPrefersXAxis && (beltStartX < mouseX && (beltStartY > mouseY))) {
            for (int x = beltStartX; x < mouseX; x++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, beltStartY, 3, BeltType::FORWARD));
            }
            beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(mouseX, beltStartY, 3, BeltType::RIGHT));
            for (int y = beltStartY-1; y >= mouseY; y--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(mouseX, y, 2, BeltType::FORWARD));
            }
        }
        else if (beltPrefersXAxis && (beltStartX > mouseX && (beltStartY < mouseY))) {
            for (int x = beltStartX; x > mouseX; x--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, beltStartY, 1, BeltType::FORWARD));
            }
            beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(mouseX, beltStartY, 1, BeltType::RIGHT));
            for (int y = beltStartY+1; y <= mouseY; y++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(mouseX, y, 0, BeltType::FORWARD));
            }
        }
        else if (beltPrefersXAxis && (beltStartX > mouseX && (beltStartY > mouseY))) {
            for (int x = beltStartX; x > mouseX; x--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, beltStartY, 1, BeltType::FORWARD));
            }
            beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(mouseX, beltStartY, 1, BeltType::LEFT));
            for (int y = beltStartY-1; y >= mouseY; y--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(mouseX, y, 2, BeltType::FORWARD));
            }
        }
        else if (!beltPrefersXAxis && (beltStartX < mouseX && (beltStartY < mouseY))) {
            for (int y = beltStartY; y < mouseY; y++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, y, 0, BeltType::FORWARD));
            }
            beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, mouseY, 0, BeltType::RIGHT));
            for (int x = beltStartX+1; x <= mouseX; x++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, mouseY, 3, BeltType::FORWARD));
            }
        }
        else if (!beltPrefersXAxis && (beltStartX < mouseX && (beltStartY > mouseY))) {
            for (int y = beltStartY; y > mouseY; y--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, y, 2, BeltType::FORWARD));
            }
            beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, mouseY, 2, BeltType::LEFT));
            for (int x = beltStartX+1; x <= mouseX; x++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, mouseY, 3, BeltType::FORWARD));
            }
        }
        else if (!beltPrefersXAxis && (beltStartX > mouseX && (beltStartY < mouseY))) {
            for (int y = beltStartY; y < mouseY; y++) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, y, 0, BeltType::FORWARD));
            }
            beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, mouseY, 0, BeltType::LEFT));
            for (int x = beltStartX-1; x >= mouseX; x--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, mouseY, 1, BeltType::FORWARD));
            }
        }
        else if (!beltPrefersXAxis && (beltStartX > mouseX && (beltStartY > mouseY))) {
            for (int y = beltStartY; y > mouseY; y--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, y, 2, BeltType::FORWARD));
            }
            beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(beltStartX, mouseY, 2, BeltType::RIGHT));
            for (int x = beltStartX-1; x >= mouseX; x--) {
                beltPlannerPreviews.push_back(std::make_shared<BeltPlannerPreview>(x, mouseY, 1, BeltType::FORWARD));
            }
        }
        for (auto& bp: beltPlannerPreviews) {AddChild(bp);}
        if (!beltPlannerPreviews.empty()) {beltPlannerPreviews[0]->SetVisible(false);}

        beltPlannerStart->SetVisible(true);
        beltPlannerStart->m_Transform.scale = cam.scale;
        beltPlannerStart->m_Transform.translation.x = (((192.0f*(0.5f+beltStartX)) - cam.translation.x) * cam.scale.x);
        beltPlannerStart->m_Transform.translation.y = (((192.0f*(0.5f+beltStartY)) - cam.translation.y) * cam.scale.y);

        beltPlannerEnd->SetVisible(true);
        beltPlannerEnd->m_Transform.scale = cam.scale;
        beltPlannerEnd->m_Transform.translation.x = (((192.0f*(0.5f+mouseX)) - cam.translation.x) * cam.scale.x);
        beltPlannerEnd->m_Transform.translation.y = (((192.0f*(0.5f+mouseY)) - cam.translation.y) * cam.scale.y);
    }
    std::shared_ptr<Belt> beltToPlace = nullptr;
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) && beltPlannerOn) {
        placeBeltSFX->Play();
        for (auto& bp: beltPlannerPreviews) {
            std::shared_ptr<Machine> MachineToRemove = MapMachines[{bp->x, bp->y}];
            if (MachineToRemove != nullptr && MachineToRemove->getName() != MachineName::BELT) {continue;}
            if (MachineToRemove != nullptr && MachineToRemove->getName() == MachineName::BELT) {
                m_Root.RemoveChild(MachineToRemove);
                LstMachines.erase(std::remove(LstMachines.begin(), LstMachines.end(), MachineToRemove), LstMachines.end());
                MachineToRemove->Delete();
                if (MachineToRemove.use_count() != 1) {throw std::invalid_argument("machine not properly removed");}
                MachineToRemove = nullptr;
            }
            beltToPlace = std::make_shared<Belt>(bp->x, bp->y, bp->r, bp->type);
            beltToPlace->Init();
            m_Root.AddChild(beltToPlace);
            LstMachines.push_back(beltToPlace);
        }
    }


    UserSelectMachine();
    if (Util::Input::IsKeyDown(Util::Keycode::R)) {heldR = (heldR + 3) % 4;}
    if (Util::Input::IsKeyDown(Util::Keycode::T)) {UserSelectVariant();}

    heldPreview->m_Transform.translation = Util::Input::GetCursorPosition();
    heldPreview->m_Transform.rotation = 0.5 * M_PI * heldR;
    heldPreview->m_Transform.scale = World::cam.scale;

    previewBorder->m_Transform.translation.x = (((192.0*(0.5+mouseX)) - cam.translation.x) * cam.scale.x);
    previewBorder->m_Transform.translation.y = (((192.0*(0.5+mouseY)) - cam.translation.y) * cam.scale.y);
    previewBorder->m_Transform.rotation = 0.5 * M_PI * heldR;
    previewBorder->m_Transform.scale = World::cam.scale;

    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && !(toolbar->hovered)) {UserPlaceMachine(mouseX, mouseY);}
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB) && !(toolbar->hovered)) {UserRemoveMachine(mouseX, mouseY);}

    return shared_from_this();
}

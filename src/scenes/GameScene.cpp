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
#include <vector>
using namespace World;

void GameScene::UserMoveCamera() {
    float camSpeed = 10;
    glm::vec2 cursorPos = Util::Input::GetCursorPosition();
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

void GameScene::UserSelectMachine() {
    bool unselectMachine = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB);
    for (auto& btn : buttons) {unselectMachine |= (btn->released && !btn->selected);}
    if (unselectMachine) {
        for (auto& btn : buttons) {btn->selected = false;}
        heldMachine = MachineName::NONE;
        heldR = 0;
        heldPreview->SetVisible(false);
    }

    bool isMachineSelected = false;
    for (int i=0; i<10; i++) {
        if (buttons[i]->released && buttons[i]->selected) {
            isMachineSelected = true;
            heldIdx = i;
        }
    }
    if (!isMachineSelected) {return;}

    beltType = BeltType::FORWARD;
    rotatorType = RotatorType::ROTATE_CW;
    tunnelType = TunnelType::IN;
    tunnelUpgraded = false;
    minerChained = false;
    previewMirrored = false;
    heldR = 0; // rotation of m_MachineHeld

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
    heldMachine = previewNames[heldIdx];
    heldPreview->SetDrawable(std::make_shared<Util::Image>(previewImagePaths[heldIdx]));
    heldPreview->SetPivot(previewPivots[heldIdx]);
    heldPreview->SetVisible(true);
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
        case 3: SelectMinerVariant(heldPreview, minerChained); break;
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
            MachineToAdd = std::make_shared<Belt>(mouseX, mouseY, heldR, beltType);
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
            MachineToAdd = std::make_shared<Miner>(
                mouseX, mouseY, heldR,
                std::make_shared<Shape>("CuCuCuCu"),
                minerChained
            );
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
        buttons.back()->idleImage = nullptr;
        buttons.back()->hoveredImage = hoveredTexture;
        buttons.back()->heldImage = hoveredTexture;
        buttons.back()->selectedImage = selectedTexture;
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
        m_Root.AddChild(buttons.back());
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
    toolbar->m_Transform.translation = {0, -windowPercent * 630};
    toolbar->m_Transform.scale = {windowPercent, windowPercent};
    toolbar->SetZIndex(80);
    m_Root.AddChild(toolbar);

    heldPreview = std::make_shared<OptiObject>();
    heldPreview->SetVisible(false);
    heldPreview->SetZIndex(99);
    m_Root.AddChild(heldPreview);
}

std::shared_ptr<Scene> GameScene::Update() {
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
    UserSelectMachine();
    if (Util::Input::IsKeyDown(Util::Keycode::R)) {heldR = (heldR + 3) % 4;}
    if (Util::Input::IsKeyDown(Util::Keycode::T)) {UserSelectVariant();}
    heldPreview->m_Transform.translation = Util::Input::GetCursorPosition();
    heldPreview->m_Transform.rotation = 0.5 * M_PI * heldR;
    heldPreview->m_Transform.scale = World::cam.scale;
    int mouseX = std::floor((((Util::Input::GetCursorPosition().x / cam.scale.x) + cam.translation.x))/192.0f);
    int mouseY = std::floor((((Util::Input::GetCursorPosition().y / cam.scale.y) + cam.translation.y))/192.0f);
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && !(toolbar->hovered)) {UserPlaceMachine(mouseX, mouseY);}
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB) && !(toolbar->hovered)) {UserRemoveMachine(mouseX, mouseY);}
    UserMoveCamera();
    return shared_from_this();
}

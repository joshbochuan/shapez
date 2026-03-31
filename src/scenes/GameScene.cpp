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
    if (Util::Input::IfScroll()) {
        auto delta = Util::Input::GetScrollDistance();
        // fix: change PTSD/src/Util/Input.cpp line 110
        // if (delta.y >= 10) {delta.y = 0;}
        cam.scale.x += delta.y * 0.05;
        cam.scale.y += delta.y * 0.05;
        cam.scale.x = std::clamp(cam.scale.x, 0.1f, 2.0f);
        cam.scale.y = std::clamp(cam.scale.y, 0.1f, 2.0f);
        LOG_DEBUG("Scrolling: x: {}, y: {}", delta.x, delta.y);
    }
}

void GameScene::UserSelectMachine() {
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB)) {
        m_MachineHeld = MachineName::NONE;
        m_MachineHeldR = 0;
        m_MachineHeldPreview->SetVisible(false);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        m_MachineHeld = MachineName::BELT;
        beltType = BeltType::FORWARD;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/belt_top.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
        m_MachineHeld = MachineName::BALANCER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/balancer.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
        m_MachineHeld = MachineName::TUNNEL;
        tunnelType = TunnelType::IN;
        tunnelUpgraded = false;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/underground_belt_entry.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_4)) {
        m_MachineHeld = MachineName::MINER;
        minerChained = false;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/miner.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_5)) {
        m_MachineHeld = MachineName::CUTTER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/cutter.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_6)) {
        m_MachineHeld = MachineName::ROTATOR;
        rotatorType = RotatorType::ROTATE_CW;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/rotater.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_7)) {
        m_MachineHeld = MachineName::STACKER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/stacker.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_8)) {
        m_MachineHeld = MachineName::MIXER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/mixer.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_9)) {
        previewMirrored = false;
        m_MachineHeld = MachineName::PAINTER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/painter.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_0)) {
        m_MachineHeld = MachineName::TRASH;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/trash.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
}

void GameScene::UserSelectVariant() {
    if (m_MachineHeld == MachineName::BELT) {
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
    else if (m_MachineHeld == MachineName::BALANCER) {
        m_MachineHeld = MachineName::SPLITTER;
        m_MachineHeldPreview->SetVisible(true);
        previewMirrored = false;
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/balancer-splitter.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    else if (m_MachineHeld == MachineName::SPLITTER) {
        if (!previewMirrored) {
            previewMirrored = true;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/balancer-splitter-inverse.png"));
        }
        else {
            m_MachineHeld = MachineName::MERGER;
            previewMirrored = false;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/balancer-merger.png"));
        }
    }
    else if (m_MachineHeld == MachineName::MERGER) {
        if (!previewMirrored) {
            previewMirrored = true;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/balancer-merger-inverse.png"));
        }
        else {
            m_MachineHeld = MachineName::BALANCER;
            previewMirrored = false;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/balancer.png"));
            m_MachineHeldPreview->SetPivot({-84, 0});
        }
    }
    else if (m_MachineHeld == MachineName::MINER) {
        if (!minerChained) {
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/miner-chainable.png"));
        }
        else {
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/miner.png"));
        }
        minerChained = !minerChained;
    }
    else if (m_MachineHeld == MachineName::ROTATOR) {
        if (rotatorType == RotatorType::ROTATE_CW) {
            rotatorType = RotatorType::ROTATE_180;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/rotater-rotate180.png"));
        }
        else if (rotatorType == RotatorType::ROTATE_180) {
            rotatorType = RotatorType::ROTATE_CCW;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/rotater-ccw.png"));
        }
        else if (rotatorType == RotatorType::ROTATE_CCW) {
            rotatorType = RotatorType::ROTATE_CW;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/rotater.png"));
        }
    }
    else if (m_MachineHeld == MachineName::TUNNEL) {
        if (tunnelType == TunnelType::IN && !tunnelUpgraded) {
            tunnelType = TunnelType::OUT;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/underground_belt_exit.png"));
        }
        else if (tunnelType == TunnelType::OUT && !tunnelUpgraded) {
            tunnelType = TunnelType::IN;
            tunnelUpgraded = true;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/underground_belt_entry-tier2.png"));
        }
        else if (tunnelType == TunnelType::IN && tunnelUpgraded) {
            tunnelType = TunnelType::OUT;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/underground_belt_exit-tier2.png"));
        }
        else if (tunnelType == TunnelType::OUT && tunnelUpgraded) {
            tunnelType = TunnelType::IN;
            tunnelUpgraded = false;
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/underground_belt_entry.png"));
        }
    }
    else if (m_MachineHeld == MachineName::PAINTER) {
        if (!previewMirrored) {
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/painter-mirrored.png"));
        }
        else {
            m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/painter.png"));
        }
        previewMirrored = !previewMirrored;
    }
}

void GameScene::UserPlaceMachine(int mouseX, int mouseY) {
    std::shared_ptr<Machine> MachineToAdd = nullptr;
    if (m_MachineHeld == MachineName::BELT) {
        try {MachineToAdd = std::make_shared<Belt>(mouseX, mouseY, m_MachineHeldR, beltType);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::BALANCER) {
        try {MachineToAdd = std::make_shared<Balancer>(mouseX, mouseY, m_MachineHeldR);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::SPLITTER) {
        try {MachineToAdd = std::make_shared<Splitter>(mouseX, mouseY, m_MachineHeldR, previewMirrored);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::MERGER) {
        try {MachineToAdd = std::make_shared<Merger>(mouseX, mouseY, m_MachineHeldR, previewMirrored);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::TUNNEL) {
        try {MachineToAdd = std::make_shared<Tunnel>(mouseX, mouseY, m_MachineHeldR, tunnelType, tunnelUpgraded);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::MINER) {
        try {MachineToAdd = std::make_shared<Miner>(mouseX, mouseY, m_MachineHeldR, std::make_shared<Shape>("CuCuCuCu"), minerChained);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::CUTTER) {
        try {MachineToAdd = std::make_shared<Cutter>(mouseX, mouseY, m_MachineHeldR);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::ROTATOR) {
        try {MachineToAdd = std::make_shared<Rotator>(mouseX, mouseY, m_MachineHeldR, rotatorType);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::STACKER) {
        try {MachineToAdd = std::make_shared<Stacker>(mouseX, mouseY, m_MachineHeldR);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::MIXER) {
        try {MachineToAdd = std::make_shared<Mixer>(mouseX, mouseY, m_MachineHeldR);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::PAINTER) {
        try {MachineToAdd = std::make_shared<Painter>(mouseX, mouseY, m_MachineHeldR, previewMirrored);}
        catch (const std::invalid_argument& e) {}
    }
    if (m_MachineHeld == MachineName::TRASH) {
        try {MachineToAdd = std::make_shared<Trash>(mouseX, mouseY);}
        catch (const std::invalid_argument& e) {}
    }
    if (MachineToAdd != nullptr) {
        if (m_MachineHeld == MachineName::BELT) {placeBeltSFX->Play();} // play place_belt.wav
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
    m_MachineHeldPreview = std::make_shared<Util::GameObject>();
    m_MachineHeldPreview->SetVisible(false);
    m_MachineHeldPreview->SetZIndex(99);
    m_Root.AddChild(m_MachineHeldPreview);
}

std::shared_ptr<Scene> GameScene::Update() {
    UserSelectMachine();
    if (Util::Input::IsKeyDown(Util::Keycode::R)) {m_MachineHeldR = (m_MachineHeldR + 3) % 4;}
    if (Util::Input::IsKeyDown(Util::Keycode::T)) {UserSelectVariant();}
    m_MachineHeldPreview->m_Transform.translation = Util::Input::GetCursorPosition();
    m_MachineHeldPreview->m_Transform.rotation = 0.5 * M_PI * m_MachineHeldR;
    m_MachineHeldPreview->m_Transform.scale = World::cam.scale;
    int mouseX = std::floor((((Util::Input::GetCursorPosition().x / cam.scale.x) + cam.translation.x))/192.0f);
    int mouseY = std::floor((((Util::Input::GetCursorPosition().y / cam.scale.y) + cam.translation.y))/192.0f);
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {UserPlaceMachine(mouseX, mouseY);}
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB)) {UserRemoveMachine(mouseX, mouseY);}
    UserMoveCamera();
    return shared_from_this();
}

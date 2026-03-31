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
#include "../include/scenes/GameScene.hpp"
#include <iostream>
#include <cmath>
using namespace World;

void loadTextures() {
    Shape::shapeTexture = std::make_shared<Util::Image>("../Resources/shapes/shape.png");
    std::string quadChoice="CRSW", colorChoice="bcgpruwy", code;
    for (int i=0; i<4; i++) {for (int j=0; j<8; j++) {for (int k=0; k<4; k++) {
        code = quadChoice[i];
        code += colorChoice[j];
        code += std::to_string(k);
        Quad::quadTextures[code] = std::make_shared<Util::Image>("../Resources/shapes/" + code + ".png");
    }}}
    Color::colorTextures.clear();
    Color::colorTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/colors/uncolored.png"));
    Color::colorTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/colors/blue.png"));
    Color::colorTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/colors/green.png"));
    Color::colorTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/colors/cyan.png"));
    Color::colorTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/colors/red.png"));
    Color::colorTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/colors/purple.png"));
    Color::colorTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/colors/yellow.png"));
    Color::colorTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/colors/white.png"));
    Belt::beltForwardTexture.clear();
    Belt::beltLeftTexture.clear();
    Belt::beltRightTexture.clear();
    Belt::beltInTexture.clear();
    Belt::beltOutTexture.clear();
    for (int i=0; i<=13; i++) {
        Belt::beltForwardTexture.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/forward_" + std::to_string(i) +".png"));
        Belt::beltLeftTexture.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/left_" + std::to_string(i) +".png"));
        Belt::beltRightTexture.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/right_" + std::to_string(i) +".png"));
        Belt::beltInTexture.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/forward_" + std::to_string(i) + "_bottom.png"));
        Belt::beltOutTexture.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/forward_" + std::to_string(i) + "_top.png"));
    }
    Splitter::splitterTexture.clear();
    Splitter::splitterTexture.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/balancer-splitter.png"));
    Splitter::splitterTexture.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/balancer-splitter-inverse.png"));
    Merger::mergerTexture.clear();
    Merger::mergerTexture.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/balancer-merger.png"));
    Merger::mergerTexture.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/balancer-merger-inverse.png"));
    Balancer::balancerTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/balancer.png");
    Cutter::cutterTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/cutter.png");
    Miner::minerTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/miner.png");
    Miner::chainedMinerTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/miner-chainable.png");
    Miner::minerCoverTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/miner-cover.png");
    Mixer::mixerTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/mixer.png");
    Rotator::rotatorCWTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/rotater.png");
    Rotator::rotator180Texture = std::make_shared<Util::Image>("../Resources/sprites/buildings/rotater-rotate180.png");
    Rotator::rotatorCCWTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/rotater-ccw.png");
    Stacker::stackerTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/stacker.png");
    Trash::trashTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/trash.png");
    Painter::painterTextures.clear();
    Painter::painterTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/painter.png"));
    Painter::painterTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/painter-mirrored.png"));
    Tunnel::tunnelInTextures.clear();
    Tunnel::tunnelOutTextures.clear();
    Tunnel::tunnelInTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/underground_belt_entry.png"));
    Tunnel::tunnelInTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/underground_belt_entry-tier2.png"));
    Tunnel::tunnelOutTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/underground_belt_exit.png"));
    Tunnel::tunnelOutTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/underground_belt_exit-tier2.png"));
}

void loadAudio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    GameScene::placeBeltSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/place_belt.wav");
    GameScene::placeBuildingSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/place_building.wav");
    GameScene::destroyBuildingSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/destroy_building.wav");
}

void App::Start() {
    LOG_TRACE("Start");
    loadAudio();
    loadTextures();

    inTitle = true;
    title = std::make_shared<Util::GameObject>();
    title->SetDrawable(std::make_shared<Util::Image>("../Resources/logo.png"));
    title->SetZIndex(100);
    title->m_Transform.translation = glm::vec2(-252, 128);
    title->m_Transform.scale = glm::vec2(0.5, 0.5);
    m_Root.AddChild(title);
    playButton = std::make_shared<UIButton>(
        glm::vec2(350, 200),
        "../Resources/1px/66BB6A.png",
        "P L A Y", 48, Util::Color::FromRGB(255, 255, 255));
    titleWindow = std::make_shared<Util::GameObject>();
    titleWindow->SetDrawable(std::make_shared<Util::Image>("../Resources/1px/D2D6DF.png"));
    titleWindow->SetZIndex(89);
    titleWindow->m_Transform.translation = glm::vec2(-252, 0);
    titleWindow->m_Transform.scale = glm::vec2(384, 384);
    m_Root.AddChild(titleWindow);
    playButton->m_Transform.scale = glm::vec2(350, 200);
    playButton->imagePressed = std::make_shared<Util::Image>("../Resources/1px/58B55C.png");
    playButton->SetZIndex(89.1);
    playButton->text->SetZIndex(89.2);
    playButton->m_Transform.translation = glm::vec2(-252, -72);
    m_Root.AddChild(playButton);

    // 41 minutes of ABSOLUTE BANGER
    shapezBGM = std::make_shared<Util::BGM>("../Resources/sounds/music/theme-full.mp3");
    shapezBGM->Play();

    background = std::make_shared<Util::GameObject>();
    background->SetDrawable(std::make_shared<Util::Image>("../Resources/background.png"));
    background->m_Transform.scale = glm::vec2(256, 256);
    background->SetZIndex(0);
    m_Root.AddChild(background);

    vignette = std::make_shared<Util::GameObject>();
    vignette->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/vignette.lossless.png"));
    vignette->m_Transform.scale = glm::vec2(20.0f/3.0f, 20.0f/3.0f);
    vignette->SetZIndex(90);
    m_Root.AddChild(vignette);

    std::vector<std::shared_ptr<Machine>> m_Machines;
    std::vector<std::shared_ptr<Machine>> vec2;
    m_Machines.push_back(std::make_shared<Hub>());
    vec2 = AddChainMinerTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    /*
    vec2 = AddTunnelTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddColorTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddBalancerTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddBeltTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddCutterTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddRotatorTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddPenguinTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    */

    for (int i=0; i<m_Machines.size(); i++) {
        m_Machines[i]->Init();
        m_Root.AddChild(m_Machines[i]);
        LstMachines.push_back(m_Machines[i]);
    }

    cam.scale = glm::vec2(0.5, 0.5);
    cam.translation.x = -504;

    scene = std::make_shared<GameScene>();
    OperateMachines();
    m_Root.Update();

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    if (inTitle) {
        playButton->Update();
        if (playButton->isReleased) {
            m_Root.RemoveChild(playButton);
            m_Root.RemoveChild(title);
            m_Root.RemoveChild(titleWindow);
            inTitle = false;
        }
    }
    scene->Update();
    OperateMachines();
    m_Root.Update();

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}



void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
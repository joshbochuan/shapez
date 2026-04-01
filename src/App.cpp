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
#include "scenes/TitleScene.hpp"
#include "AssetLoader.hpp"
#include <iostream>
#include <cmath>
#include <chrono>

using namespace World;

void App::Start() {
    LOG_TRACE("Start");
    AssetLoader::loadAudio();
    AssetLoader::loadTextures();

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
    // vec2 = AddChainMinerTest();
    // m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddDrawableBenchmark10k();
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

    scene = std::make_shared<TitleScene>();
    OperateMachines();
    m_Root.Update();

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    auto start = std::chrono::steady_clock::now();
    scene = scene->Update();
    auto t1 = std::chrono::steady_clock::now();
    OperateMachines();
    auto t2 = std::chrono::steady_clock::now();
    m_Root.Update();
    auto end = std::chrono::steady_clock::now();

    auto sceneDuration = std::chrono::duration_cast<std::chrono::microseconds>(t1-start);
    auto operationDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1);
    auto rootDuration = std::chrono::duration_cast<std::chrono::microseconds>(end-t2);
    std::cout << sceneDuration.count() << " ";
    std::cout << operationDuration.count() << " ";
    std::cout << rootDuration.count() << std::endl;
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
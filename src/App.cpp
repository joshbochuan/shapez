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
#include "../include/Opti/OptiObject.hpp"
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

    hub = std::make_shared<Hub>();
    hub->Init();
    m_Root.AddChild(hub);

    // 41 minutes of ABSOLUTE BANGER
    shapezBGM = std::make_shared<Util::BGM>("../Resources/sounds/music/theme-full.mp3");
    shapezBGM->Play();

    background = std::make_shared<OptiObject>();
    background->SetDrawable(std::make_shared<Util::Image>("../Resources/background.png"));
    background->m_Transform.scale = glm::vec2(256, 256);
    background->SetZIndex(0);

    vignette = std::make_shared<OptiObject>();
    vignette->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/vignette.lossless.png"));
    vignette->m_Transform.scale = glm::vec2(static_cast<float>(WINDOW_WIDTH)/192.0f, static_cast<float>(WINDOW_HEIGHT)/108);
    vignette->SetZIndex(90);
    m_Root.AddChild(vignette);

    gridLine = std::make_shared<OptiObject>();
    gridLine->SetDrawable(std::make_shared<Util::Image>("../Resources/1px/E3E7EA.png"));
    gridLine->SetZIndex(1);
    gridLine->m_Visible = true;

    std::vector<std::shared_ptr<Machine>> m_Machines;
    std::vector<std::shared_ptr<Machine>> vec2;
    //vec2 = AddMachineBenchmark50k();
    vec2 = AddDrawableBenchmark10k();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    // vec2 = AddChainMinerTest();
    // m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    // vec2 = AddDrawableBenchmark10k();
    // m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
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
    m_Root.Update(pool);

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    auto start = std::chrono::steady_clock::now();
    scene = scene->Update();
    auto t1 = std::chrono::steady_clock::now();
    OperateMachines();
    auto t2 = std::chrono::steady_clock::now();

    background->CalData();
    background->Draw();
    float gridThickness = 9; // thickness of gridlines when the cam scale is 1
    float gridStart = std::fmod(-cam.translation.x, 192.0f)*cam.scale.x;
    while (-gridStart < WINDOW_WIDTH>>1) {gridStart -= 192.0f * cam.scale.x;}
    gridLine->m_Transform.scale = {gridThickness*cam.scale.x, WINDOW_HEIGHT};
    for (float i = gridStart; i < static_cast<float>(WINDOW_WIDTH)/2.0f; i += cam.scale.x*192.0f) {
        gridLine->m_Transform.translation = {i, 0};
        gridLine->CalData();
        gridLine->Draw();
    }
    gridStart = std::fmod(-cam.translation.y, 192.0f)*cam.scale.y;
    while (-gridStart < WINDOW_HEIGHT>>1) {gridStart -= 192.0f * cam.scale.y;}
    gridLine->m_Transform.scale = {WINDOW_WIDTH, gridThickness*cam.scale.y};
    for (float i = gridStart; i < static_cast<float>(WINDOW_HEIGHT)/2.0f; i += cam.scale.y*192.0f) {
        gridLine->m_Transform.translation = {0, i};
        gridLine->CalData();
        gridLine->Draw();
    }
    m_Root.Update(pool);

    auto end = std::chrono::steady_clock::now();
    if (Util::Input::IsKeyDown(Util::Keycode::K)) {
        std::cout << SaveWorld("test.txt") << std::endl;
    }
    if (Util::Input::IsKeyDown(Util::Keycode::L)) {
        LoadWorld("test.txt");
    }

    auto sceneDuration = std::chrono::duration_cast<std::chrono::microseconds>(t1-start);
    auto operationDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1);
    auto rootDuration = std::chrono::duration_cast<std::chrono::microseconds>(end-t2);
    auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    std::cout << sceneDuration.count() << " us\t+ ";
    std::cout << operationDuration.count() << " us\t+ ";
    std::cout << rootDuration.count() << " us\t= ";
    std::cout << static_cast<float>(totalDuration.count())/1000.0f << " ms\n";

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
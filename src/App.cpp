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
#include <windows.h>
#include <filesystem>

#include "Util/Time.hpp"

using namespace World;

void App::Start() {
    LOG_TRACE("Start");
    char exePath[MAX_PATH];
    GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    std::filesystem::current_path(std::filesystem::path(exePath).parent_path());

    AssetLoader::loadAudio();
    AssetLoader::loadTextures();

    hub = std::make_shared<Hub>();
    hub->Init();
    m_Root.AddChild(hub);

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

    scene = std::make_shared<TitleScene>();
    m_Root.AddChild(scene);
    UpdateWorld();
    m_Root.Update(pool);

    std::cout << "finished initialization" << std::endl;
    std::cout << "app will use " + std::to_string(pool.size()) << " threads\n";

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    auto start = std::chrono::steady_clock::now();
    std::shared_ptr<Scene> nextScene = scene->Update();
    if (nextScene == nullptr || Util::Input::IfExit()) {
        m_CurrentState = State::END;
        return;
    }
    if (nextScene != scene) {
        m_Root.RemoveChild(scene);
        m_Root.AddChild(nextScene);
        scene = nextScene;
    }

    if (DOES_WORLD_TICK) {UpdateWorld();}

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

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
    auto end = std::chrono::steady_clock::now();
    auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    MSPT = static_cast<float>(totalDuration.count()) / 1000.0f;
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
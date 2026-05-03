//
// Created by joshb on 2026/5/3.
//
#include "scenes/PauseScene.hpp"
#include "scenes/TitleScene.hpp"
#include "config.hpp"
#include "World.hpp"
using namespace World;

PauseScene::PauseScene(int beltCount, int machineCount, long long playTime) {
    blur = std::make_shared<OptiObject>();
    blur->SetDrawable(std::make_shared<Util::Image>("../Resources/1px/blur.png"));
    blur->m_Transform.scale = {WINDOW_WIDTH, WINDOW_HEIGHT};
    blur->SetZIndex(91);
    AddChild(blur);

    resumeButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/settings_menu_play_idle.png"));
    resumeButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/icons/settings_menu_play_hovered.png");
    resumeButton->m_Transform.translation.x = -200.0f * windowPercent;
    resumeButton->SetZIndex(92);
    resumeButton->keys.push_back(Util::Keycode::ESCAPE);
    resumeButton->Update();
    AddChild(resumeButton);

    exitButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/settings_menu_exit_idle.png"));
    exitButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/icons/settings_menu_exit_hovered.png");
    exitButton->m_Transform.translation.x = 200.0f * windowPercent;
    exitButton->SetZIndex(92);
    exitButton->Update();
    AddChild(exitButton);

    beltCountLabel = std::make_shared<Text>("BELTS", 36, Util::Color::FromRGB(208, 211, 217));
    beltCountLabel->m_Transform.scale = {windowPercent, windowPercent};
    beltCountLabel->m_Transform.translation.x = -static_cast<float>(WINDOW_WIDTH >> 2) * (4.0f / 3.0f);
    beltCountLabel->m_Transform.translation.y = -static_cast<float>(WINDOW_HEIGHT >> 1) + windowPercent * 150.0f;
    beltCountLabel->SetZIndex(92);
    AddChild(beltCountLabel);

    beltCountNum = std::make_shared<Text>(std::to_string(beltCount), 48, Util::Color::FromRGB(255, 255, 255));
    beltCountNum->m_Transform.scale = {windowPercent, windowPercent};
    beltCountNum->m_Transform.translation.x = -static_cast<float>(WINDOW_WIDTH >> 2) * (4.0f / 3.0f);
    beltCountNum->m_Transform.translation.y = -static_cast<float>(WINDOW_HEIGHT >> 1) + windowPercent * 100.0f;
    beltCountNum->SetZIndex(92);
    AddChild(beltCountNum);

    machineCountLabel = std::make_shared<Text>("BUILDINGS", 36, Util::Color::FromRGB(208, 211, 217));
    machineCountLabel->m_Transform.scale = {windowPercent, windowPercent};
    machineCountLabel->m_Transform.translation.x = 0;
    machineCountLabel->m_Transform.translation.y = -static_cast<float>(WINDOW_HEIGHT >> 1) + windowPercent * 150.0f;
    machineCountLabel->SetZIndex(92);
    AddChild(machineCountLabel);

    machineCountNum = std::make_shared<Text>(std::to_string(machineCount), 48, Util::Color::FromRGB(255, 255, 255));
    machineCountNum->m_Transform.scale = {windowPercent, windowPercent};
    machineCountNum->m_Transform.translation.x = 0;
    machineCountNum->m_Transform.translation.y = -static_cast<float>(WINDOW_HEIGHT >> 1) + windowPercent * 100.0f;
    machineCountNum->SetZIndex(92);
    AddChild(machineCountNum);

    playTimeLabel = std::make_shared<Text>("PLAYTIME", 36, Util::Color::FromRGB(208, 211, 217));
    playTimeLabel->m_Transform.scale = {windowPercent, windowPercent};
    playTimeLabel->m_Transform.translation.x = static_cast<float>(WINDOW_WIDTH >> 2) * (4.0f / 3.0f);;
    playTimeLabel->m_Transform.translation.y = -static_cast<float>(WINDOW_HEIGHT >> 1) + windowPercent * 150.0f;
    playTimeLabel->SetZIndex(92);
    AddChild(playTimeLabel);

    std::string minuteCnt = std::to_string(playTime / 60);
    if (playTime >= 120) {minuteCnt += " minutes";}
    else {minuteCnt += " minute";}
    playTimeNum = std::make_shared<Text>(minuteCnt, 48, Util::Color::FromRGB(255, 255, 255));
    playTimeNum->m_Transform.scale = {windowPercent, windowPercent};
    playTimeNum->m_Transform.translation.x = static_cast<float>(WINDOW_WIDTH >> 2) * (4.0f / 3.0f);;
    playTimeNum->m_Transform.translation.y = -static_cast<float>(WINDOW_HEIGHT >> 1) + windowPercent * 100.0f;
    playTimeNum->SetZIndex(92);
    AddChild(playTimeNum);

    DOES_WORLD_TICK = false;
}

std::shared_ptr<Scene> PauseScene::Update() {
    resumeButton->Update();
    exitButton->Update();
    if (resumeButton->released) {
        DOES_WORLD_TICK = true;
        return nullptr;
    }
    if (exitButton->released) {
        DOES_WORLD_TICK = true;
        return std::make_shared<TitleScene>();
    }
    return shared_from_this();
}

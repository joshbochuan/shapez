//
// Created by joshb on 2026/3/31.
//
#include "scenes/TitleScene.hpp"
#include "scenes/GameScene.hpp"
#include "World.hpp"
using namespace World;

TitleScene::TitleScene() {
    cam.scale = glm::vec2(0.5, 0.5);
    cam.translation.x = -504;
    title = std::make_shared<OptiObject>();
    title->SetDrawable(std::make_shared<Util::Image>("../Resources/logo.png"));
    title->SetZIndex(100);
    title->m_Transform.translation = glm::vec2(-252, 128);
    title->m_Transform.scale = glm::vec2(0.5, 0.5);
    m_Root.AddChild(title);
    playButton = std::make_shared<Button>(
        glm::vec2(350, 200),
        "../Resources/1px/66BB6A.png",
        "P L A Y", 48, Util::Color::FromRGB(255, 255, 255));
    titleWindow = std::make_shared<OptiObject>();
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
}

std::shared_ptr<Scene> TitleScene::Update() {
    playButton->Update();
    if (playButton->isReleased) {
        m_Root.RemoveChild(playButton);
        m_Root.RemoveChild(title);
        m_Root.RemoveChild(titleWindow);
        return std::make_shared<GameScene>();
    }
    return shared_from_this();
}

//
// Created by joshb on 2026/3/31.
//
#include "scenes/TitleScene.hpp"
#include "scenes/GameScene.hpp"
#include "World.hpp"
using namespace World;

TitleScene::TitleScene() {
    glm::vec2 windowPercentVec = {1, 1}; // the window size compared to 1440p
    windowPercentVec.x = static_cast<float>(WINDOW_WIDTH)/2560.0f;
    windowPercentVec.y = static_cast<float>(WINDOW_HEIGHT)/1440.0f;
    float windowPercent = 0;
    if (windowPercentVec.x < windowPercentVec.y) {windowPercent = windowPercentVec.x;}
    else {windowPercent = windowPercentVec.y;}

    cam.scale = glm::vec2(0.2, 0.2);
    title = std::make_shared<OptiObject>();
    title->SetDrawable(std::make_shared<Util::Image>("../Resources/logo.png"));
    title->SetZIndex(80);
    title->m_Transform.translation = glm::vec2(0, windowPercent * 550);
    title->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    m_Root.AddChild(title);

    titleWindow = std::make_shared<OptiObject>();
    titleWindow->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/shapezbackground.png"));
    titleWindow->SetZIndex(80);
    titleWindow->m_Transform.translation = glm::vec2(windowPercent*0, windowPercent*(-100));
    titleWindow->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    m_Root.AddChild(titleWindow);

    playButton = std::make_shared<Button>(
        glm::vec2(windowPercent*395, windowPercent*147),
        "../Resources/ui/blobs/playButton.png",
        " PLAY", windowPercent*96, Util::Color::FromRGB(255, 255, 255));
    playButton->imagePressed = std::make_shared<Util::Image>("../Resources/ui/blobs/playButtonHovered.png");
    playButton->SetZIndex(81);
    playButton->text->SetZIndex(82);
    playButton->m_Transform.translation = titleWindow->m_Transform.translation;
    playButton->m_Transform.translation.x -= windowPercent * 170;
    playButton->m_Transform.translation.y += windowPercent * 318;
    playButton->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    m_Root.AddChild(playButton);

    importButton = std::make_shared<Button>(
        glm::vec2(windowPercent*316, windowPercent*67),
        "../Resources/ui/blobs/importButton.png",
        "   IMPORT", windowPercent*48, Util::Color::FromRGB(255, 255, 255));
    importButton->imagePressed = std::make_shared<Util::Image>("../Resources/ui/blobs/importButtonHovered.png");
    importButton->SetZIndex(81);
    importButton->text->SetZIndex(82);
    importButton->m_Transform.translation = titleWindow->m_Transform.translation;
    importButton->m_Transform.translation.x += windowPercent * 210;
    importButton->m_Transform.translation.y += windowPercent * (318+40);
    importButton->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    m_Root.AddChild(importButton);

    newGameButton = std::make_shared<Button>(
        glm::vec2(windowPercent*316, windowPercent*67),
        "../Resources/ui/blobs/importButton.png",
        "   NEW GAME", windowPercent*48, Util::Color::FromRGB(255, 255, 255));
    newGameButton->imagePressed = std::make_shared<Util::Image>("../Resources/ui/blobs/importButtonHovered.png");
    newGameButton->SetZIndex(81);
    newGameButton->text->SetZIndex(82);
    newGameButton->m_Transform.translation = titleWindow->m_Transform.translation;
    newGameButton->m_Transform.translation.x += windowPercent * 210;
    newGameButton->m_Transform.translation.y += windowPercent * (318-40);
    newGameButton->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    m_Root.AddChild(newGameButton);
}

std::shared_ptr<Scene> TitleScene::Update() {
    playButton->Update();
    newGameButton->Update();
    importButton->Update();
    if (playButton->isReleased) {
        m_Root.RemoveChild(playButton);
        m_Root.RemoveChild(title);
        m_Root.RemoveChild(titleWindow);
        m_Root.RemoveChild(importButton);
        m_Root.RemoveChild(newGameButton);
        return std::make_shared<GameScene>();
    }
    return shared_from_this();
}

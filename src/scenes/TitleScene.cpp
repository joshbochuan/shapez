//
// Created by joshb on 2026/3/31.
//
#include "scenes/TitleScene.hpp"
#include "scenes/GameScene.hpp"
#include "World.hpp"
using namespace World;

TitleScene::TitleScene() {
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

    playButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/playButton.png"));
    playButton->text = std::make_shared<Text>("P L A Y", 54, Util::Color::FromRGB(255, 255, 255));
    playButton->text->SetPivot({-5, 0});
    playButton->AddChild(playButton->text);
    playButton->hoveredImage = std::make_shared<Util::Image>("../Resources/ui/blobs/playButtonHovered.png");
    playButton->heldImage = playButton->hoveredImage;
    playButton->SetZIndex(81);
    playButton->text->SetZIndex(82);
    playButton->m_Transform.translation = titleWindow->m_Transform.translation;
    playButton->m_Transform.translation.x -= windowPercent * 170;
    playButton->m_Transform.translation.y += windowPercent * 318;
    playButton->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    m_Root.AddChild(playButton);

    importButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/importButton.png"));
    importButton->text = std::make_shared<Text>("IMPORT", 36, Util::Color::FromRGB(255, 255, 255));
    importButton->text->SetPivot({-5, 0});
    importButton->AddChild(importButton->text);
    importButton->hoveredImage = std::make_shared<Util::Image>("../Resources/ui/blobs/importButtonHovered.png");
    importButton->heldImage = importButton->hoveredImage;
    importButton->SetZIndex(81);
    importButton->text->SetZIndex(82);
    importButton->m_Transform.translation = titleWindow->m_Transform.translation;
    importButton->m_Transform.translation.x += windowPercent * 210;
    importButton->m_Transform.translation.y += windowPercent * (318+40);
    importButton->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    m_Root.AddChild(importButton);

    newGameButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/importButton.png"));
    newGameButton->text = std::make_shared<Text>("NEW GAME", 36, Util::Color::FromRGB(255, 255, 255));
    newGameButton->text->SetPivot({-5, 0});
    newGameButton->AddChild(newGameButton->text);
    newGameButton->hoveredImage = std::make_shared<Util::Image>("../Resources/ui/blobs/importButtonHovered.png");
    newGameButton->heldImage = newGameButton->hoveredImage;
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
    if (playButton->released) {
        m_Root.RemoveChild(playButton);
        m_Root.RemoveChild(title);
        m_Root.RemoveChild(titleWindow);
        m_Root.RemoveChild(importButton);
        m_Root.RemoveChild(newGameButton);
        return std::make_shared<GameScene>();
    }
    return shared_from_this();
}

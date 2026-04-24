//
// Created by joshb on 2026/3/31.
//
#include "scenes/TitleScene.hpp"
#include "scenes/GameScene.hpp"
#include "World.hpp"
using namespace World;

TitleScene::TitleScene() {
    cam.scale = glm::vec2(0.4 * windowPercent, 0.4 * windowPercent);
    LoadWorld("title.txt");

    blur = std::make_shared<OptiObject>();
    blur->SetDrawable(std::make_shared<Util::Image>("../Resources/1px/blur.png"));
    blur->m_Transform.scale = {WINDOW_WIDTH, WINDOW_HEIGHT};
    blur->SetZIndex(91);
    AddChild(blur);

    title = std::make_shared<OptiObject>();
    title->SetDrawable(std::make_shared<Util::Image>("../Resources/logo.png"));
    title->SetZIndex(92);
    title->m_Transform.translation = glm::vec2(0, windowPercent * 550);
    title->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    AddChild(title);

    titleWindow = std::make_shared<OptiObject>();
    titleWindow->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/shapezbackground.png"));
    titleWindow->SetZIndex(92);
    titleWindow->m_Transform.translation = glm::vec2(windowPercent*0, windowPercent*(-100));
    titleWindow->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    AddChild(titleWindow);

    playButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/playButton.png"));
    playButton->text = std::make_shared<Text>("P L A Y", 54, Util::Color::FromRGB(255, 255, 255));
    playButton->text->SetPivot({-5, 0});
    playButton->AddChild(playButton->text);
    playButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/playButtonHovered.png");
    playButton->heldBackground = playButton->hoveredBackground;
    playButton->SetZIndex(93);
    playButton->text->SetZIndex(94);
    playButton->m_Transform.translation = titleWindow->m_Transform.translation;
    playButton->m_Transform.translation.x -= windowPercent * 170;
    playButton->m_Transform.translation.y += windowPercent * 318;
    playButton->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    AddChild(playButton);

    importButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/importButton.png"));
    importButton->text = std::make_shared<Text>("IMPORT", 36, Util::Color::FromRGB(255, 255, 255));
    importButton->text->SetPivot({-5, 0});
    importButton->AddChild(importButton->text);
    importButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/importButtonHovered.png");
    importButton->heldBackground = importButton->hoveredBackground;
    importButton->SetZIndex(93);
    importButton->text->SetZIndex(94);
    importButton->m_Transform.translation = titleWindow->m_Transform.translation;
    importButton->m_Transform.translation.x += windowPercent * 210;
    importButton->m_Transform.translation.y += windowPercent * (318+40);
    importButton->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    AddChild(importButton);

    newGameButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/importButton.png"));
    newGameButton->text = std::make_shared<Text>("NEW GAME", 36, Util::Color::FromRGB(255, 255, 255));
    newGameButton->text->SetPivot({-5, 0});
    newGameButton->AddChild(newGameButton->text);
    newGameButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/importButtonHovered.png");
    newGameButton->heldBackground = newGameButton->hoveredBackground;
    newGameButton->SetZIndex(93);
    newGameButton->text->SetZIndex(94);
    newGameButton->m_Transform.translation = titleWindow->m_Transform.translation;
    newGameButton->m_Transform.translation.x += windowPercent * 210;
    newGameButton->m_Transform.translation.y += windowPercent * (318-40);
    newGameButton->m_Transform.scale = glm::vec2(windowPercent, windowPercent);
    AddChild(newGameButton);
}

std::shared_ptr<Scene> TitleScene::Update() {
    playButton->Update();
    newGameButton->Update();
    importButton->Update();
    if (playButton->released) {
        ClearWorld();
        return std::make_shared<GameScene>();
    }
    return nullptr;
}

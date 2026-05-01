//
// Created by joshb on 2026/3/31.
//
#include "scenes/TitleScene.hpp"
#include "scenes/GameScene.hpp"
#include "World.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <windows.h>
#include <shlobj.h>
using namespace World;

std::string RelativeTimeStr(long long time) {
    long long currentTime = std::chrono::duration_cast<std::chrono::seconds>(
                      std::chrono::system_clock::now().time_since_epoch()).count();
    long long timeDifference = currentTime - time;
    if (timeDifference <= 1) {return std::to_string(timeDifference) + " second ago";}
    if (timeDifference < 60) {return std::to_string(timeDifference) + " seconds ago";}
    timeDifference /= 60;
    if (timeDifference <= 1) {return std::to_string(timeDifference) + " minute ago";}
    if (timeDifference < 60) {return std::to_string(timeDifference) + " minutes ago";}
    timeDifference /= 60;
    if (timeDifference <= 1) {return std::to_string(timeDifference) + " hour ago";}
    if (timeDifference < 60) {return std::to_string(timeDifference) + " hours ago";}
    timeDifference /= 24;
    if (timeDifference <= 1) {return std::to_string(timeDifference) + " day ago";}
    if (timeDifference < 60) {return std::to_string(timeDifference) + " days ago";}
    timeDifference /= 365;
    if (timeDifference <= 1) {return std::to_string(timeDifference) + " year ago";}
    return std::to_string(timeDifference) + " years ago";
}

SaveBlob::SaveBlob(std::string name, int level, long long lastPlayed) {
    this->name = name;
    this->level = level;
    this->lastPlayedAt = lastPlayed;
    m_Transform.scale = {windowPercent, windowPercent};
    SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/saveBlob.png"));
    SetZIndex(93);

    nameTxt = std::make_shared<Text>(name, 40, Util::Color::FromRGB(85, 85, 85));
    nameTxt->m_Transform.scale = m_Transform.scale;
    nameTxt->SetPivot({-0.5f * nameTxt->m_Text->GetSize().x, 0});
    nameTxt->SetZIndex(94);
    AddChild(nameTxt);

    levelTxt = std::make_shared<Text>("Level " + std::to_string(level), 32, Util::Color::FromRGB(172, 172, 172));
    levelTxt->m_Transform.scale = m_Transform.scale;
    levelTxt->SetPivot({-0.5f * levelTxt->m_Text->GetSize().x, 0});
    levelTxt->SetZIndex(94);
    AddChild(levelTxt);

    lastPlayedTxt = std::make_shared<Text>(RelativeTimeStr(lastPlayed), 32, Util::Color::FromRGB(172, 172, 172));
    lastPlayedTxt->m_Transform.scale = m_Transform.scale;
    lastPlayedTxt->SetPivot({-0.5f * lastPlayedTxt->m_Text->GetSize().x, 0});
    lastPlayedTxt->SetZIndex(94);
    AddChild(lastPlayedTxt);

    playButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/startButton.png"));
    playButton->m_Transform.scale = m_Transform.scale;
    playButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/startButtonHovered.png");
    playButton->SetZIndex(94);
    AddChild(playButton);

    renameButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/edit_key.png"));
    renameButton->hoverScale = 0.4f;
    renameButton->heldScale = 0.4f;
    renameButton->idleScale = 0.4f;
    renameButton->selectScale = 0.4f;
    renameButton->SetZIndex(94);
    AddChild(renameButton);

    deleteButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/delete.png"));
    deleteButton->SetZIndex(94);
    deleteButton->hoverScale = 0.6f;
    deleteButton->heldScale = 0.6f;
    deleteButton->idleScale = 0.6f;
    deleteButton->selectScale = 0.6f;
    AddChild(deleteButton);

    downloadButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/download.png"));
    downloadButton->SetZIndex(94);
    downloadButton->hoverScale = 0.6f;
    downloadButton->heldScale = 0.6f;
    downloadButton->idleScale = 0.6f;
    downloadButton->selectScale = 0.6f;
    AddChild(downloadButton);
}

void SaveBlob::Update() {
    nameTxt->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * -350.0f;
    nameTxt->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 20.0f;
    levelTxt->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * -350.0f;
    levelTxt->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -20.0f;
    lastPlayedTxt->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * -100.0f;
    lastPlayedTxt->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -20.0f;
    playButton->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * 312.0f;
    playButton->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 0.0f;
    renameButton->m_Transform.translation.x = nameTxt->m_Transform.translation.x
                                              + nameTxt->m_Transform.scale.x * nameTxt->m_Text->GetSize().x
                                              + m_Transform.scale.x * 10.0f;
    renameButton->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 20.0f;
    deleteButton->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * 230.0f;
    deleteButton->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -25.0f;
    downloadButton->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * 230.0f;
    downloadButton->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 25.0f;
    playButton->Update();
    renameButton->Update();
    deleteButton->Update();
    downloadButton->Update();
}

TitleScene::TitleScene() {
    cam.scale = glm::vec2(0.4 * windowPercent, 0.4 * windowPercent);
    WORLD_NAME = "title";
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

    std::string saveName;
    int saveLevel;
    long long saveTime;
    for (const auto& entry : std::filesystem::directory_iterator("../Saves")) {
        // Skip non-.txt files
        if (entry.path().extension() != ".txt") continue;

        std::ifstream file(entry.path());
        if (!file) continue;

        std::string line;

        // Check first line
        if (!std::getline(file, line) || line != "SHAPEZ_SAVE_FILE") continue;

        saveName = entry.path().stem().string(); // filename without extension

        // Parse LEVEL
        std::string key;
        if (!(file >> key >> saveLevel) || key != "LEVEL") continue;

        // Parse LAST_PLAYED_AT
        if (!(file >> key >> saveTime) || key != "LAST_PLAYED_AT") continue;

        saveBlobs.push_back(std::make_shared<SaveBlob>(saveName, saveLevel, saveTime));
        AddChild(saveBlobs.back());
    }
    std::sort(saveBlobs.begin(), saveBlobs.end(), [](const std::shared_ptr<SaveBlob>& a, const std::shared_ptr<SaveBlob>& b) {
        return a->lastPlayedAt > b->lastPlayedAt;
    });

    if (!saveBlobs.empty()) {playButton->text->m_Text->SetText("CONTINUE");}
}

void openFolder(const std::string& path) {
    ShellExecuteA(nullptr, "explore", path.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

std::string openFileDialog() {
    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = nullptr;
    ofn.lpstrFile   = filename;
    ofn.nMaxFile    = MAX_PATH;
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn))
        return std::string(filename);

    return "";
}

int import() {
    // Open file dialog
    std::string selectedPath = openFileDialog();
    if (selectedPath.empty()) {
        std::cout << "Cancelled." << std::endl;
        return 0;
    }

    // Check if it's a .txt file
    if (std::filesystem::path(selectedPath).extension() != ".txt") {
        std::cout << "Error: Selected file is not a .txt file." << std::endl;
        return 1;
    }

    // Check first line
    std::ifstream inFile(selectedPath);
    if (!inFile) {
        std::cout << "Error: Could not open file." << std::endl;
        return 1;
    }

    std::string firstLine;
    std::getline(inFile, firstLine);
    inFile.close();

    if (firstLine != "SHAPEZ_SAVE_FILE") {
        std::cout << "Error: Not a valid Shapez save file." << std::endl;
        return 1;
    }

    // Ensure ../Saves/ exists
    std::string savesDir = "../Saves";
    std::filesystem::create_directories(savesDir);

    // Copy file, avoiding overwrite
    std::string baseName = std::filesystem::path(selectedPath).stem().string();
    std::string destPath = savesDir + "/" + getUniqueFilename(baseName, ".txt") + ".txt";

    std::filesystem::copy_file(selectedPath, destPath);
    std::cout << "Saved as: " << destPath << std::endl;

    return 0;
}

std::shared_ptr<Scene> TitleScene::Update() {
    playButton->Update();
    newGameButton->Update();
    importButton->Update();
    if (importButton->released) {import();}
    for (int i=0; i<saveBlobs.size(); i++) {
        auto save = saveBlobs[i];
        save->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * (20-100*i);
        save->Update();
        if (save->downloadButton->released) {
            openFolder("../Saves/");
            continue;
        }
        if (!save->playButton->released) {continue;}
        WORLD_NAME = save->name;
        LoadWorld(WORLD_NAME + ".txt");
        return std::make_shared<GameScene>();
    }
    if ((playButton->released) && (!saveBlobs.empty())) {
        ClearWorld();
        WORLD_NAME = saveBlobs[0]->name;
        LoadWorld(WORLD_NAME + ".txt");
        return std::make_shared<GameScene>();
    }
    if ((playButton->released) && (saveBlobs.empty()) || (newGameButton->released)) {
        CreateWorld("Unnamed");
        return std::make_shared<GameScene>();
    }
    return nullptr;
}
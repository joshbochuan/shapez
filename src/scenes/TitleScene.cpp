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
#include "App.hpp"
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

DeleteBlob::DeleteBlob(std::string name, int level) {
    this->name = name;
    this->level = level;

    SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/cancelBlob.png"));
    m_Transform.scale = {windowPercent, windowPercent};
    SetZIndex(97);

    blur = std::make_shared<OptiObject>();
    blur->SetDrawable(std::make_shared<Util::Image>("../Resources/1px/blur.png"));
    blur->m_Transform.scale = {WINDOW_WIDTH, WINDOW_HEIGHT};
    blur->SetZIndex(96);
    AddChild(blur);

    confirmTxt = std::make_shared<Text>("CONFIRM DELETION", 48, Util::Color::FromRGB(85, 85, 85));
    confirmTxt->SetPivot({-0.5f * confirmTxt->m_Drawable->GetSize().x, 0});
    confirmTxt->m_Transform.scale = m_Transform.scale;
    confirmTxt->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * -470.0f;
    confirmTxt->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 190.0f;
    confirmTxt->SetZIndex(98);
    AddChild(confirmTxt);

    uSureTxt = std::make_shared<Text>("Are you sure you want to delete the following game?", 36, Util::Color::FromRGB(85, 85, 85));
    uSureTxt->SetPivot({-0.5f * uSureTxt->m_Drawable->GetSize().x, 0});
    uSureTxt->m_Transform.scale = m_Transform.scale;
    uSureTxt->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * -470.0f;
    uSureTxt->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 110.0f;
    uSureTxt->SetZIndex(98);
    AddChild(uSureTxt);

    levelTxt = std::make_shared<Text>("\'" + name + "\' at level " + std::to_string(level), 36, Util::Color::FromRGB(85, 85, 85));
    levelTxt->SetPivot({-0.5f * levelTxt->m_Drawable->GetSize().x, 0});
    levelTxt->m_Transform.scale = m_Transform.scale;
    levelTxt->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * -470.0f;
    levelTxt->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 20.0f;
    levelTxt->SetZIndex(98);
    AddChild(levelTxt);

    noReturnTxt = std::make_shared<Text>("This can not be undone!", 36, Util::Color::FromRGB(85, 85, 85));
    noReturnTxt->SetPivot({-0.5f * noReturnTxt->m_Drawable->GetSize().x, 0});
    noReturnTxt->m_Transform.scale = m_Transform.scale;
    noReturnTxt->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * -470.0f;
    noReturnTxt->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -70.0f;
    noReturnTxt->SetZIndex(98);
    AddChild(noReturnTxt);

    cancelButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/deleteCancel.png"));
    cancelButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/deleteCancelHovered.png");
    cancelButton->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * 75.0f;
    cancelButton->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -175.0f;
    cancelButton->text = std::make_shared<Text>("CANCEL", 40, Util::Color::FromRGB(255, 255, 255));
    cancelButton->text->SetZIndex(99);
    cancelButton->text->SetPivot({-10, 0});
    cancelButton->AddChild(cancelButton->text);
    cancelButton->SetZIndex(98);
    AddChild(cancelButton);

    deleteButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/deleteConfirm.png"));
    deleteButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/deleteConfirmHovered.png");
    deleteButton->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * 350.0f;
    deleteButton->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -175.0f;
    deleteButton->text = std::make_shared<Text>("DELETE", 40, Util::Color::FromRGB(255, 255, 255));
    deleteButton->text->SetZIndex(99);
    deleteButton->text->SetPivot({-10, 0});
    deleteButton->AddChild(deleteButton->text);
    deleteButton->SetZIndex(98);
    AddChild(deleteButton);

    Update();
}

bool DeleteBlob::Update() {
    cancelButton->Update();
    deleteButton->Update();
    if (cancelButton->released) {return true;}
    if (!deleteButton->released) {return false;}
    std::string path = "../Saves/" + name + ".txt";
    if (std::filesystem::exists(path)) {std::filesystem::remove(path);}
    else {std::cout << "File not found." << std::endl;}
    return true;
}

RenameBlob::RenameBlob(std::string name) {
    this->name = name;

    SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/cancelBlob.png"));
    m_Transform.scale = {windowPercent, windowPercent};
    SetZIndex(97);

    blur = std::make_shared<OptiObject>();
    blur->SetDrawable(std::make_shared<Util::Image>("../Resources/1px/blur.png"));
    blur->m_Transform.scale = {WINDOW_WIDTH, WINDOW_HEIGHT};
    blur->SetZIndex(96);
    AddChild(blur);

    titleTxt = std::make_shared<Text>("RENAME SAVEGAME", 48, Util::Color::FromRGB(85, 85, 85));
    titleTxt->SetPivot({-0.5f * titleTxt->m_Drawable->GetSize().x, 0});
    titleTxt->m_Transform.scale = m_Transform.scale;
    titleTxt->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * -470.0f;
    titleTxt->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 190.0f;
    titleTxt->SetZIndex(98);
    AddChild(titleTxt);

    renameTxt = std::make_shared<Text>("You can rename your savegame here.", 36, Util::Color::FromRGB(85, 85, 85));
    renameTxt->SetPivot({-0.5f * renameTxt->m_Drawable->GetSize().x, 0});
    renameTxt->m_Transform.scale = m_Transform.scale;
    renameTxt->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * -470.0f;
    renameTxt->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 120.0f;
    renameTxt->SetZIndex(98);
    AddChild(renameTxt);

    inputBox = std::make_shared<TextInput>(std::make_shared<Util::Image>("../Resources/ui/blobs/renameDialog.png"), 48, Util::Color::FromRGB(85, 85, 85));
    AddChild(inputBox);
    cancelButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/deleteConfirm.png"));
    cancelButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/deleteConfirmHovered.png");
    cancelButton->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * 75.0f;
    cancelButton->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -175.0f;
    cancelButton->text = std::make_shared<Text>("CANCEL", 40, Util::Color::FromRGB(255, 255, 255));
    cancelButton->text->SetZIndex(99);
    cancelButton->text->SetPivot({-10, 0});
    cancelButton->AddChild(cancelButton->text);
    cancelButton->SetZIndex(98);
    AddChild(cancelButton);

    okButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/deleteCancel.png"));
    okButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/deleteCancelHovered.png");
    okButton->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * 350.0f;
    okButton->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -175.0f;
    okButton->text = std::make_shared<Text>("OK", 40, Util::Color::FromRGB(255, 255, 255));
    okButton->text->SetZIndex(99);
    okButton->text->SetPivot({-10, 0});
    okButton->AddChild(okButton->text);
    okButton->SetZIndex(98);
    AddChild(okButton);

    inputBox = std::make_shared<TextInput>(
        std::make_shared<Util::Image>("../Resources/ui/blobs/renameDialog.png"), 48, Util::Color::FromRGB(85, 85, 85));
    inputBox->SetZIndex(0);
    inputBox->text = name;
    inputBox->background->SetZIndex(98);
    inputBox->textDisplay->SetZIndex(99);
    inputBox->pointer->SetZIndex(99);
    AddChild(inputBox);

    Update();
}

bool validateSaveName(std::string str) {
    if (str == "") {return false;}
    if (str.find('\\') != std::string::npos) {return false;}
    if (str.find('/') != std::string::npos) {return false;}
    if (str.find(':') != std::string::npos) {return false;}
    if (str.find('*') != std::string::npos) {return false;}
    if (str.find('?') != std::string::npos) {return false;}
    if (str.find('\"') != std::string::npos) {return false;}
    if (str.find('<') != std::string::npos) {return false;}
    if (str.find('>') != std::string::npos) {return false;}
    if (str.find('|') != std::string::npos) {return false;}
    return true;
}

bool RenameBlob::Update() {
    inputBox->Update();
    cancelButton->Update();
    okButton->Update();

    bool valid = validateSaveName(inputBox->text);
    if (valid) {inputBox->background->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/renameDialog.png"));}
    else {inputBox->background->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/renameDialogIllegal.png"));}

    if (cancelButton->released) {return true;}
    if (!okButton->released) {return false;}
    if (!valid) {return false;}
    if (inputBox->text == name) {return true;}

    std::filesystem::rename(
        "../Saves/" + name + ".txt",
        "../Saves/" + getUniqueFilename(inputBox->text, ".txt") + ".txt");
    return true;
}

TitleScene::TitleScene() {
    cam.scale = glm::vec2(0.4 * windowPercent, 0.4 * windowPercent);
    cam.translation = glm::vec2(0, 0);
    ClearWorld();
    WORLD_NAME = "title";
    // LoadWorld("../Resources/title.txt");

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

    closeButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/closeButton.png"));
    closeButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/icons/closeButtonHovered.png");
    closeButton->m_Transform.translation.x = static_cast<float>(WINDOW_WIDTH >> 1) - 100.0f * windowPercent;
    closeButton->m_Transform.translation.y = static_cast<float>(WINDOW_HEIGHT >> 1) - 100.0f * windowPercent;
    closeButton->keys.push_back(Util::Keycode::ESCAPE);
    closeButton->SetZIndex(93);
    AddChild(closeButton);

    titleBGM->Play();

    Refresh();
}

void TitleScene::Refresh() {
    for (auto& save: saveBlobs) {RemoveChild(save);}
    saveBlobs.clear();

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
    else {playButton->text->m_Text->SetText("P L A Y");}
}

std::string openImportFileDialog() {
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

void updateLastPlayedAt(const std::string& filePath) {
    // Read all lines
    std::ifstream inFile(filePath);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line))
        lines.push_back(line);
    inFile.close();

    // Replace third line
    if (lines.size() >= 3)
        lines[2] = "LAST_PLAYED_AT " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(
                                          std::chrono::system_clock::now().time_since_epoch()).count());

    // Write back
    std::ofstream outFile(filePath);
    for (const auto& l : lines)
        outFile << l << "\n";
}

int import() {
    std::filesystem::path originalDir = std::filesystem::current_path();
    std::string selectedPath = openImportFileDialog();
    std::filesystem::current_path(originalDir);

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

    std::cout << selectedPath << " " << destPath << std::endl;
    std::filesystem::copy_file(selectedPath, destPath);
    updateLastPlayedAt(destPath);
    std::cout << "Saved as: " << destPath << std::endl;

    return 0;
}

std::string openDownloadFolderDialog() {
    std::string result;

    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    IFileOpenDialog* pDialog = nullptr;
    CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
                     IID_IFileOpenDialog, (void**)&pDialog);

    // Set it to pick folders instead of files
    DWORD options;
    pDialog->GetOptions(&options);
    pDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST);

    pDialog->SetTitle(L"Select destination folder");

    if (pDialog->Show(nullptr) == S_OK) {
        IShellItem* pItem = nullptr;
        if (pDialog->GetResult(&pItem) == S_OK) {
            PWSTR folderPath = nullptr;
            pItem->GetDisplayName(SIGDN_FILESYSPATH, &folderPath);
            // Convert wide string to regular string
            char narrow[MAX_PATH];
            WideCharToMultiByte(CP_UTF8, 0, folderPath, -1, narrow, MAX_PATH, nullptr, nullptr);
            result = std::string(narrow);
            CoTaskMemFree(folderPath);
            pItem->Release();
        }
    }

    pDialog->Release();
    CoUninitialize();

    return result;
}

bool exportFile(const std::string& filePath) {
    std::filesystem::path originalDir = std::filesystem::current_path();

    std::string destDir = openDownloadFolderDialog();

    std::filesystem::current_path(originalDir);

    if (destDir.empty()) {
        std::cout << "Cancelled." << std::endl;
        return false;
    }

    std::filesystem::path src  = filePath;
    std::filesystem::path dest = std::filesystem::path(destDir) / src.filename();

    // Handle duplicate filenames
    if (std::filesystem::exists(dest)) {
        std::string baseName = src.stem().string();
        std::string ext      = src.extension().string();
        int counter = 2;
        do {
            dest = std::filesystem::path(destDir) / (baseName + " (" + std::to_string(counter++) + ")" + ext);
        } while (std::filesystem::exists(dest));
    }

    std::filesystem::copy_file(src, dest);
    std::cout << "Exported to: " << dest << std::endl;
    return true;
}

std::shared_ptr<Scene> TitleScene::Update() {
    if (deleteBlob != nullptr) {
        if (!deleteBlob->Update()) {return shared_from_this();}
        RemoveChild(deleteBlob);
        deleteBlob = nullptr;
        Refresh();
        Update();
        return shared_from_this();
    }
    if (renameBlob != nullptr) {
        if (!renameBlob->Update()) {return shared_from_this();}
        RemoveChild(renameBlob);
        renameBlob = nullptr;
        Refresh();
        Update();
        return shared_from_this();
    }
    playButton->Update();
    newGameButton->Update();
    importButton->Update();
    closeButton->Update();
    if (importButton->released) {
        import();
        Refresh();
        Update();
    }
    for (int i=0; i<saveBlobs.size(); i++) {
        auto save = saveBlobs[i];
        save->m_Transform.translation.y = m_Transform.translation.y + windowPercent * (30-135*i);
        save->Update();
        if (save->downloadButton->released) {exportFile("../Saves/" + save->name + ".txt");}
        if (save->deleteButton->released) {
            deleteBlob = std::make_shared<DeleteBlob>(save->name, save->level);
            AddChild(deleteBlob);
            return shared_from_this();
        }
        if (save->renameButton->released) {
            renameBlob = std::make_shared<RenameBlob>(save->name);
            AddChild(renameBlob);
            return shared_from_this();
        }
        if (!save->playButton->released) {continue;}
        WORLD_NAME = save->name;
        LoadWorld("../Saves/" + WORLD_NAME + ".txt");
        return std::make_shared<GameScene>();
    }
    if ((playButton->released) && (!saveBlobs.empty())) {
        ClearWorld();
        WORLD_NAME = saveBlobs[0]->name;
        LoadWorld("../Saves/" + WORLD_NAME + ".txt");
        return std::make_shared<GameScene>();
    }
    if ((playButton->released) && (saveBlobs.empty()) || (newGameButton->released)) {
        CreateWorld("Unnamed");
        return std::make_shared<GameScene>();
    }
    if (closeButton->released) {
        return nullptr;
    }
    return shared_from_this();
}
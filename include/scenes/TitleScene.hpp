//
// Created by joshb on 2026/3/31.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_TITLESCENE_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_TITLESCENE_HPP
#include "Scene.hpp"
#include "Util/GameObject.hpp"
#include "UIelement/Button.hpp"
#include "../Opti/OptiObject.hpp"
#include "UIelement/TextInput.hpp"
#include "Util/BGM.hpp"
#include "UIelement/Notification.hpp"

class SaveBlob: public OptiObject {
public:
    std::string name;
    int level;
    long long lastPlayedAt;
    std::shared_ptr<Text> nameTxt, levelTxt, lastPlayedTxt;
    std::shared_ptr<Button> playButton, downloadButton, deleteButton, renameButton;
    SaveBlob(std::string name, int level, long long lastPlayed);
    void Update();
};

class DeleteBlob: public OptiObject {
public:
    std::string name;
    int level;
    std::shared_ptr<OptiObject> blur;
    std::shared_ptr<Text> confirmTxt, uSureTxt, levelTxt, noReturnTxt;
    std::shared_ptr<Button> cancelButton, deleteButton;
    DeleteBlob(std::string name, int level);
    bool Update();
};

class RenameBlob: public OptiObject {
public:
    std::string name;
    std::shared_ptr<OptiObject> blur;
    std::shared_ptr<Text> titleTxt, renameTxt;
    std::shared_ptr<TextInput> inputBox;
    std::shared_ptr<Button> cancelButton, okButton;
    int enterCount = 0; // used for counting amount of enter key pressed to enable/disable cheats
    RenameBlob(std::string name);
    bool Update();
};

class TitleScene: public Scene {
private:
    std::shared_ptr<OptiObject> blur;
    std::shared_ptr<OptiObject> title;
    std::shared_ptr<OptiObject> titleWindow; // #E9EAEC
    std::shared_ptr<Button> playButton, importButton, newGameButton; // green and blue buttons
    std::vector<std::shared_ptr<SaveBlob>> saveBlobs;
    std::shared_ptr<DeleteBlob> deleteBlob;
    std::shared_ptr<RenameBlob> renameBlob;
    std::shared_ptr<Button> closeButton;
    std::shared_ptr<Notification> notification = nullptr; // used for display enable/disable cheats
    std::shared_ptr<Text> noSaveTextA, noSaveTextB;

public:
    static inline std::shared_ptr<Util::BGM> titleBGM;
    TitleScene();
    std::shared_ptr<Scene> Update();
    void Refresh();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_TITLESCENE_HPP
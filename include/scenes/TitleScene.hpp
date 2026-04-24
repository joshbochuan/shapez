//
// Created by joshb on 2026/3/31.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_TITLESCENE_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_TITLESCENE_HPP
#include "Scene.hpp"
#include "Util/GameObject.hpp"
#include "UIelement/Button.hpp"
#include "../Opti/OptiObject.hpp"

class TitleScene: public Scene {
private:
    std::shared_ptr<OptiObject> blur;
    std::shared_ptr<OptiObject> title;
    std::shared_ptr<OptiObject> titleWindow; // #E9EAEC
    std::shared_ptr<Button> playButton, importButton, newGameButton; // green and blue buttons
    std::vector<std::shared_ptr<OptiObject>> SaveSlots;

public:
    TitleScene();
    std::shared_ptr<Scene> Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_TITLESCENE_HPP
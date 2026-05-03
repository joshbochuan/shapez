//
// Created by joshb on 2026/5/3.
//

#ifndef SHAPEZ_PAUSESCENE_HPP
#define SHAPEZ_PAUSESCENE_HPP
#include "Scene.hpp"
#include "UIelement/Button.hpp"
#include "UIelement/Text.hpp"

class PauseScene: public Scene {
public:
    PauseScene(int beltCount, int machineCount, long long playTime);
    std::shared_ptr<Scene> Update() override;
    std::shared_ptr<OptiObject> blur;
    std::shared_ptr<Button> resumeButton, exitButton;
    std::shared_ptr<Text> beltCountLabel, machineCountLabel, playTimeLabel;
    std::shared_ptr<Text> beltCountNum, machineCountNum, playTimeNum;
};

#endif //SHAPEZ_PAUSESCENE_HPP
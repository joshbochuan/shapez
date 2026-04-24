//
// Created by joshb on 2026/4/12.
//

#ifndef SHAPEZ_UPGRADESCENE_HPP
#define SHAPEZ_UPGRADESCENE_HPP
#include "UIelement/Button.hpp"
#include "UIelement/ProgressBar.hpp"
#include "UIelement/Text.hpp"
#include "items/Shape.hpp"
#include "Scene.hpp"
#include "World.hpp"

class UpgradeBlob: public OptiObject, public std::enable_shared_from_this<UpgradeBlob> {
public:
    static inline std::shared_ptr<Util::SFX> upgradeSFX;
    static inline std::vector<std::shared_ptr<Util::Image>> tierBackgroundImages;

    World::UpgradeType type;
    std::shared_ptr<Button> upgradeButton;
    std::vector<std::shared_ptr<Item>> itemTargets;
    std::vector<std::shared_ptr<ProgressBar>> progressBars;
    std::vector<int> targetAmounts, progressAmounts;
    std::shared_ptr<Text> title, multiplier;

    std::shared_ptr<OptiObject> tierBackground;
    std::shared_ptr<Text> tierText;
    UpgradeBlob(World::UpgradeType type, std::string title);
    void ClearTarget();
    void AddTarget(std::shared_ptr<Item> item, int target);
    void Update();
};

class UpgradeScene: public Scene {
    std::shared_ptr<OptiObject> background;
    std::shared_ptr<OptiObject> blur;
    std::shared_ptr<Text> title;
    std::shared_ptr<Button> closeButton;
    std::shared_ptr<UpgradeBlob> beltBlob;
    std::shared_ptr<UpgradeBlob> mineBlob;
    std::shared_ptr<UpgradeBlob> processBlob;
    std::shared_ptr<UpgradeBlob> paintBlob;
public:
    UpgradeScene();
    std::shared_ptr<Scene> Update();
};

#endif //SHAPEZ_UPGRADESCENE_HPP
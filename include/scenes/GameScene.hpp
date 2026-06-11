//
// Created by joshb on 2026/3/31.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_USERACTION_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_USERACTION_HPP

#include "../buildings/Belt.hpp"
#include "../buildings/Tunnel.hpp"
#include "../buildings/Rotator.hpp"
#include "scenes/Scene.hpp"
#include "Util/SFX.hpp"
#include "../Opti/OptiObject.hpp"
#include "UIelement/Button.hpp"
#include "UIelement/ItemPin.hpp"
#include "UIelement/Notification.hpp"
#include "Util/BGM.hpp"

class BeltPlannerPreview: public OptiObject {
public:
    int x, y, r;
    BeltType type;
    std::shared_ptr<OptiObject> arrow = nullptr;
    static inline std::shared_ptr<OptiImage> arrowTexture = nullptr; // the arrow
    static inline std::shared_ptr<OptiImage> lineTexture = nullptr;  // 1px line

    BeltPlannerPreview(int x, int y, int r, BeltType type);
};

class PlaceState { // unused currently
public:
    MachineName heldMachine = MachineName::NONE; // the machine user is currently trying to place
    BeltType beltType = BeltType::FORWARD;
    RotatorType rotatorType = RotatorType::ROTATE_CW;
    TunnelType tunnelType = TunnelType::IN;
    int heldR = 0; // rotation of m_MachineHeld
    int heldIdx = 9;
    bool tunnelUpgraded = false;
    bool minerChained = false;
    bool minerCheat = false;
    bool previewMirrored = false;
    std::shared_ptr<OptiObject> heldPreview = nullptr;
};

class GameScene: public Scene {
    BeltType beltType = BeltType::FORWARD;
    RotatorType rotatorType = RotatorType::ROTATE_CW;
    TunnelType tunnelType = TunnelType::IN;
    bool tunnelUpgraded = false;
    bool minerChained = false;
    bool minerCheat = false;
    bool previewMirrored = false;
    int heldIdx = 9;
    MachineName heldMachine = MachineName::NONE; // the machine user is currently trying to place
    int heldR = 0; // rotation of m_MachineHeld
    int beltStartX = 0, beltStartY = 0;
    bool beltPlannerOn = false;
    bool beltPrefersXAxis = true;
    bool beltPreferenceSet = false;
    std::shared_ptr<OptiObject> beltPlannerStart, beltPlannerEnd;

    std::vector<std::shared_ptr<OptiObject>> previewArrows;
    std::shared_ptr<OptiObject> previewBorder;

    std::vector<std::shared_ptr<BeltPlannerPreview>> beltPlannerPreviews;
    std::shared_ptr<OptiObject> heldPreview = nullptr;
    int saveCooldown = 0; // auto saves every 2 minutes
    std::shared_ptr<Notification> notification;

    std::shared_ptr<Util::Image> hoveredTexture;
    std::shared_ptr<Util::Image> selectedTexture;
    std::shared_ptr<Util::Image> lockedTexture;

    glm::vec2 movingMarker = {0, 0};
    int movingMarkerCooldown = 0; // for how many frames the player can't move

    std::vector<std::shared_ptr<ItemPin>> itemPins;

    std::shared_ptr<OptiObject> heldMachineTitleBackground;
    std::shared_ptr<Text> heldMachineTitle, heldMachineSpeedTxt, heldMachineRateTxt;

    void UserMoveCamera();
    void UserSelectMachine();
    void UserSelectVariant();
    void UserPlaceMachine(int mouseX, int mouseY);
    void UserRemoveMachine(int mouseX, int mouseY);
    glm::vec2 lastMousePos = glm::vec2();
public:
    static inline std::shared_ptr<Util::BGM> shapezBGM;
    static inline std::shared_ptr<Util::SFX> placeBuildingSFX, placeBeltSFX, destroyBuildingSFX;
    std::shared_ptr<Button> toolbar;
    std::vector<std::shared_ptr<Button>> buttons;
    std::shared_ptr<Button> pauseButton, saveButton, upgradeButton;
    std::shared_ptr<Scene> subScene = nullptr;
    std::shared_ptr<Text> tickRateText, TickTimeText, fpsText, versionText;
    bool toggleDebug = false; // shows fps
    GameScene();
    std::shared_ptr<Scene> Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_USERACTION_HPP
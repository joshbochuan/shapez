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

class GameScene: public Scene {
    MachineName m_MachineHeld = MachineName::NONE; // the machine user is currently trying to place
    BeltType beltType = BeltType::FORWARD;
    RotatorType rotatorType = RotatorType::ROTATE_CW;
    TunnelType tunnelType = TunnelType::IN;
    int m_MachineHeldR = 0; // rotation of m_MachineHeld
    bool tunnelUpgraded = false;
    bool minerChained = false;
    bool previewMirrored = false;
    std::shared_ptr<Util::GameObject> m_MachineHeldPreview = nullptr;
    void UserMoveCamera();
    void UserSelectMachine();
    void UserSelectVariant();
    void UserPlaceMachine(int mouseX, int mouseY);
    void UserRemoveMachine(int mouseX, int mouseY);
public:
    static inline std::shared_ptr<Util::SFX> placeBuildingSFX, placeBeltSFX, destroyBuildingSFX;
    GameScene();
    std::shared_ptr<Scene> Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_USERACTION_HPP
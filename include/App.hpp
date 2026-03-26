#ifndef APP_HPP
#define APP_HPP

#include <unordered_map>
#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"
#include "Shape.hpp"
#include "Belt.hpp"
#include "Miner.hpp"
#include "Trash.hpp"
#include "Machine.hpp"
#include "Rotator.hpp"
#include "Tunnel.hpp"
#include "Util/SFX.hpp"
#include "Util/BGM.hpp"

inline std::shared_ptr<Util::SFX> placeBuildingSFX, placeBeltSFX, destroyBuildingSFX;

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void OperateMachines();
    void UserMoveCamera();
    void UserSelectMachine();
    void UserSelectVariant();
    void UserPlaceMachine(int mouseX, int mouseY);
    void UserRemoveMachine(int mouseX, int mouseY);

    std::shared_ptr<Util::GameObject> background;
    std::shared_ptr<Util::GameObject> vignette;
    std::vector<std::shared_ptr<Machine>> m_Machines;
    MachineName m_MachineHeld = MachineName::NONE; // the machine user is currently trying to place
    int m_MachineHeldR = 0; // rotation of m_MachineHeld
    // belts, rotators and tunnels have variants
    BeltType beltType = BeltType::FORWARD;
    RotatorType rotatorType = RotatorType::ROTATE_CW;
    TunnelType tunnelType = TunnelType::IN;
    bool tunnelUpgraded = false;
    bool painterMirrored = false;
    std::shared_ptr<Util::GameObject> m_MachineHeldPreview;

    std::shared_ptr<Util::BGM> shapezBGM;

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;

    Util::Renderer m_Root;
};

#endif

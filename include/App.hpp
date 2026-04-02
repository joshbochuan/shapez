#ifndef APP_HPP
#define APP_HPP

#include <unordered_map>
#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"
#include "items/Shape.hpp"
#include "buildings/Machine.hpp"
#include "scenes/GameScene.hpp"
#include "Util/BGM.hpp"
#include "UIelement/Button.hpp"

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

    std::shared_ptr<Scene> scene;
    std::shared_ptr<ShapezObject> background;
    std::shared_ptr<ShapezObject> vignette;
    std::shared_ptr<Util::BGM> shapezBGM;

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;
};

#endif

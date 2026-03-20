#ifndef APP_HPP
#define APP_HPP

#include <unordered_map>
#include "pch.hpp" // IWYU pragma: export
#include "Util/Renderer.hpp"
#include "Shape.hpp"
#include "Belt.hpp"

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

    std::shared_ptr<Belt> m_Belt;
    std::shared_ptr<Shape> m_Shape;
    std::shared_ptr<Quad> m_Quad;

    // std::unordered_map<glm::vec2, std::shared_ptr<Machine>> machines;

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;

    Util::Renderer m_Root;
};

#endif

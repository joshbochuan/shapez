#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Shape.hpp"
#include "Util/Renderer.hpp"

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

    std::shared_ptr<Shape> m_Shape;
    std::shared_ptr<Quad> m_Quad;

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;

    Util::Renderer m_Root;
};

#endif

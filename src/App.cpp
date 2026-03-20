#include "App.hpp"

#include "Shape.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"

void App::Start() {
    LOG_TRACE("Start");

    m_Shape = std::make_shared<Shape>("CuRrRrSg:Rr--Ww--", glm::vec2({0, 0}));
    m_Shape->m_Transform.translation.x = 0;
    m_Shape->m_Transform.translation.y = 0;

    m_Root.AddChild(m_Shape);

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    //TODO: do your things here and delete this line <3

    m_Root.Update();

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

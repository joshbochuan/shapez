#include "App.hpp"

#include "Global.hpp"
#include "Shape.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"

void App::Start() {
    LOG_TRACE("Start");
    /* z-index:
    0 - background
    1 - world ore back color
    2 - world ore sprite
    3 - grid
    4 - belts
    5 - item background
    6 - item first layer
    7 - item second layer
    8 - item third layer
    9 - item forth layer
    10 - machines

    */

    m_Shape = std::make_shared<Shape>("CrCrCcCc:RrRrRrRr:WwWwWwWw:RgSgRgSg");
    m_Shape->m_Transform.scale = glm::vec2(0.3, 0.3);

    m_Belt = std::make_shared<Belt>(0, 0, 3, 0.02, BeltType::RIGHT);
    m_Belt->acceptor->item = m_Shape;
    m_Belt->AddChild(m_Shape);

    m_Root.AddChild(m_Belt);

    cam.scale = glm::vec2(0.5, 0.5);

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    int camSpeed = 5;
    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        cam.translation.y += camSpeed;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
        cam.translation.y -= camSpeed;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        cam.translation.x -= camSpeed;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        cam.translation.x += camSpeed;
    }

    m_Belt->Update();
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

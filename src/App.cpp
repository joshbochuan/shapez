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

    m_Machines.push_back(std::make_shared<Miner>(-1, 0, 3, 0.02, std::make_shared<Shape>("CuCuCuCu")));
    m_Machines.push_back(std::make_shared<Trash>(1, -2, 0.02));
    m_Machines.push_back(std::make_shared<Belt>(0, 0, 3, 0.02, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(1, 0, 3, 0.02, BeltType::RIGHT));
    m_Machines.push_back(std::make_shared<Belt>(1, -1, 2, 0.02, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(2, -1, 0.02));
    m_Machines.push_back(std::make_shared<Miner>(-1, -1, 3, 0.02, nullptr));
    m_Machines.push_back(std::make_shared<Miner>(-1, 1, 0, 0.02, nullptr));
    m_Machines.push_back(std::make_shared<Miner>(-2, 1, 0, 0.02, nullptr));
    m_Machines.push_back(std::make_shared<Miner>(-2, 0, 1, 0.02, nullptr));
    m_Machines.push_back(std::make_shared<Miner>(-2, -1, 1, 0.02, nullptr));

    for (int i=0; i<m_Machines.size(); i++) {
        m_Root.AddChild(m_Machines[i]);
    }

    cam.scale = glm::vec2(0.5, 0.5);

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    float camSpeed = 10;
    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        cam.translation.y += camSpeed / cam.scale.y;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
        cam.translation.y -= camSpeed / cam.scale.y;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        cam.translation.x -= camSpeed / cam.scale.x;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        cam.translation.x += camSpeed / cam.scale.x;
    }

    if (Util::Input::IfScroll()) {
        auto delta = Util::Input::GetScrollDistance();

        // todo: temporary fix, delete when they fix ts scroll wheel
        if (delta.y >= 10) {delta.y = 0;}

        cam.scale.x += delta.y * 0.05;
        cam.scale.y += delta.y * 0.05;
        cam.scale.x = std::clamp(cam.scale.x, 0.1f, 2.0f);
        cam.scale.y = std::clamp(cam.scale.y, 0.1f, 2.0f);

        LOG_DEBUG("Scrolling: x: {}, y: {}", delta.x, delta.y);
    }

    for (int i=0; i<m_Machines.size(); i++) {
        m_Machines[i]->Update();
    }

    // holy shit it worked first try
    // code responsible for moving items from ejectors to acceptors
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejector;
    int dx, dy;
    for (const auto& pair : ejectors) {
        ejector = pair.second;
        if (ejector->item == nullptr) {continue;}
        if (ejector->progress < 1) {continue;}
        switch (ejector->r) {
            case 0: dx = 0; dy = 1; break;
            case 1: dx = -1; dy = 0; break;
            case 2: dx = 0; dy = -1; break;
            case 3: dx = 1; dy = 0; break;
        }
        acceptor = acceptors[{ejector->x+dx, ejector->y+dy, ejector->r}];
        if (acceptor == nullptr) {continue;}
        if (acceptor->item != nullptr) {continue;}
        if ((ejector->item->getType() == ItemType::COLOR) && (!acceptor->takesColor)) {continue;}
        if ((ejector->item->getType() == ItemType::SHAPE) && (!acceptor->takesShape)) {continue;}

        acceptor->item = ejector->item;
        acceptor->progress = ejector->progress-1;
        acceptor->AddChild(acceptor->item);
        ejector->RemoveChild(acceptor->item);
        ejector->item = nullptr;
        ejector->progress = 0;
    }

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

#include "App.hpp"

#include "Global.hpp"
#include "Shape.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include "Rotator.hpp"
#include "Cutter.hpp"
#include "Hub.hpp"
#include "Balancer.hpp"

void App::Start() {
    LOG_TRACE("Start");
    /* z-index:
    0 - background
    1~9 - world
    10~19 - machine below (mainly belts)
    20~39 - shapes
    40~99 - machine above
    100+ - ui
    */
    background = std::make_shared<Util::GameObject>();
    background->SetDrawable(std::make_shared<Util::Image>("../Resources/background.png"));
    background->m_Transform.scale = glm::vec2(256, 256);
    background->SetZIndex(0);
    m_Root.AddChild(background);

    vignette = std::make_shared<Util::GameObject>();
    vignette->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/vignette.lossless.png"));
    vignette->m_Transform.scale = glm::vec2(20.0f/3.0f, 20.0f/3.0f);
    vignette->SetZIndex(100);
    m_Root.AddChild(vignette);

    // balancer test
    m_Machines.push_back(std::make_shared<Miner>(-8, -9, 0, std::make_shared<Shape>("CuCuCuCu")));
    m_Machines.push_back(std::make_shared<Miner>(-7, -9, 0, std::make_shared<Shape>("CuCuCuCu")));
    m_Machines.push_back(std::make_shared<Miner>(-6, -9, 0, std::make_shared<Shape>("CuCuCuCu")));
    m_Machines.push_back(std::make_shared<Miner>(-5, -9, 0, std::make_shared<Shape>("CuCuCuCu")));
    m_Machines.push_back(std::make_shared<Belt>(-8, -8, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-7, -8, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-6, -8, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-5, -8, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Balancer>(-8, -7, 0));
    m_Machines.push_back(std::make_shared<Balancer>(-6, -7, 0));
    m_Machines.push_back(std::make_shared<Balancer>(-7, -6, 0));
    m_Machines.push_back(std::make_shared<Belt>(-7, -5, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(-7, -4));

    // hub test
    m_Machines.push_back(std::make_shared<Hub>());
    m_Machines.push_back(std::make_shared<Belt>(-2, 2, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-1, 2, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(0, 2, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(1, 2, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Miner>(-2, 3, 2, std::make_shared<Shape>("CuCuCuCu")));
    m_Machines.push_back(std::make_shared<Miner>(-1, 3, 2, std::make_shared<Shape>("RrRrRrRr")));
    m_Machines.push_back(std::make_shared<Miner>(0, 3, 2, std::make_shared<Shape>("CuCuCuCu")));
    m_Machines.push_back(std::make_shared<Miner>(1, 3, 2, std::make_shared<Shape>("CrRgSbWw:Cr----Ww:Cr------:--Rg----")));
    // belt, mine, trash test
    m_Machines.push_back(std::make_shared<Miner>(-1, 7, 3, std::make_shared<Shape>("CuCuCuCu")));
    m_Machines.push_back(std::make_shared<Trash>(1, 5));
    m_Machines.push_back(std::make_shared<Belt>(0, 7, 3, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(1, 7, 3, BeltType::RIGHT));
    m_Machines.push_back(std::make_shared<Belt>(1, 6, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(2, 6));
    m_Machines.push_back(std::make_shared<Miner>(-1, 6, 3, nullptr));
    m_Machines.push_back(std::make_shared<Miner>(-1, 8, 0, nullptr));
    m_Machines.push_back(std::make_shared<Miner>(-2, 8, 0, nullptr));
    m_Machines.push_back(std::make_shared<Miner>(-2, 7, 1, nullptr));
    m_Machines.push_back(std::make_shared<Miner>(-2, 6, 1, nullptr));

    // rotator test
    m_Machines.push_back(std::make_shared<Miner>(5, 0, 0, std::make_shared<Shape>("CrRgSbWw:CrRgSbWw:CrRgSbWw:CrRgSbWw")));
    m_Machines.push_back(std::make_shared<Miner>(6, 0, 0, std::make_shared<Shape>("CrRgSbWw:CrRgSbWw:CrRgSbWw:CrRgSbWw")));
    m_Machines.push_back(std::make_shared<Miner>(7, 0, 0, std::make_shared<Shape>("CrRgSbWw:CrRgSbWw:CrRgSbWw:CrRgSbWw")));
    m_Machines.push_back(std::make_shared<Miner>(8, 0, 0, std::make_shared<Shape>("CrRgSbWw:CrRgSbWw:CrRgSbWw:CrRgSbWw")));
    m_Machines.push_back(std::make_shared<Belt>(5, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(6, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(7, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(8, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Rotator>(5, 2, 0, RotatorType::ROTATE_CW));
    m_Machines.push_back(std::make_shared<Rotator>(6, 2, 0, RotatorType::ROTATE_CW));
    m_Machines.push_back(std::make_shared<Rotator>(7, 2, 0, RotatorType::ROTATE_180));
    m_Machines.push_back(std::make_shared<Rotator>(8, 2, 0, RotatorType::ROTATE_CCW));
    m_Machines.push_back(std::make_shared<Rotator>(5, 3, 0, RotatorType::ROTATE_CW));
    m_Machines.push_back(std::make_shared<Belt>(6, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(7, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(8, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(5, 4, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(5, 5));
    m_Machines.push_back(std::make_shared<Trash>(6, 4));
    m_Machines.push_back(std::make_shared<Trash>(7, 4));
    m_Machines.push_back(std::make_shared<Trash>(8, 4));

    // cutter test
    m_Machines.push_back(std::make_shared<Miner>(-5, 0, 0, std::make_shared<Shape>("CrRgSbWw:Cr----Ww:Cr------:--Rg----")));
    m_Machines.push_back(std::make_shared<Belt>(-5, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Cutter>(-5, 2, 0));
    m_Machines.push_back(std::make_shared<Belt>(-5, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-4, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(-5, 4));
    m_Machines.push_back(std::make_shared<Trash>(-4, 4));

    // arknights player behavior
    std::shared_ptr<Belt> belt;
    belt = std::make_shared<Belt>(5, -3, 0, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(6, -3, 3, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(6, -4, 2, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(5, -4, 1, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(4, -2, 0, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(3, -2, 1, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(3, -3, 2, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(4, -3, 3, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);


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

    /* test
    if (Util::Input::IsKeyPressed(Util::Keycode::U)) {cam.scale.x += 0.05; cam.scale.y += 0.05;}
    if (Util::Input::IsKeyPressed(Util::Keycode::I)) {cam.scale.x -= 0.05; cam.scale.y -= 0.05;}
    */

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

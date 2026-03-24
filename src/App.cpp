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
#include "Tunnel.hpp"
#include <iostream>
#include <cmath>

void App::Start() {
    LOG_TRACE("Start");
    /* z-index:
    0 - background
    1~9 - world
    10~19 - machine below (mainly belts)
    20~39 - shapes
    40~99 - machine above
    100 - post-processing
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

    // tunnel test
    m_Machines.push_back(std::make_shared<Miner>(0, -7, 3, std::make_shared<Shape>("CuCuCuCu")));
    m_Machines.push_back(std::make_shared<Belt>(1, -7, 3, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Tunnel>(2, -7, 3, TunnelType::IN, false));
    m_Machines.push_back(std::make_shared<Tunnel>(5, -7, 3, TunnelType::OUT, false));
    m_Machines.push_back(std::make_shared<Belt>(6, -7, 3, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(7, -7));

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
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(6, -3, 3, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(6, -4, 2, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(5, -4, 1, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(4, -2, 0, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(3, -2, 1, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(3, -3, 2, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(4, -3, 3, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = nullptr;


    for (int i=0; i<m_Machines.size(); i++) {
        m_Machines[i]->Init();
        m_Root.AddChild(m_Machines[i]);
    }

    cam.scale = glm::vec2(0.5, 0.5);

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    // user machine placement
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB)) {
        m_MachineHeld = MachineName::NONE;
        m_MachineHeldR = 0;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        m_MachineHeld = MachineName::BELT;
        beltType = BeltType::FORWARD;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
        m_MachineHeld = MachineName::BALANCER;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
        m_MachineHeld = MachineName::TUNNEL;
        tunnelType = TunnelType::IN;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_4)) {
        m_MachineHeld = MachineName::MINER;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_5)) {
        m_MachineHeld = MachineName::CUTTER;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_6)) {
        m_MachineHeld = MachineName::ROTATOR;
        rotatorType = RotatorType::ROTATE_CW;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_7)) {
        m_MachineHeld = MachineName::STACKER;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_8)) {
        m_MachineHeld = MachineName::MIXER;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_9)) {
        m_MachineHeld = MachineName::PAINTER;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_0)) {
        m_MachineHeld = MachineName::TRASH;
    }
    if (Util::Input::IsKeyDown(Util::Keycode::R) && m_MachineHeld != MachineName::NONE) {
        m_MachineHeldR = (m_MachineHeldR + 3) % 4;
    }

    // handle variants
    if (Util::Input::IsKeyDown(Util::Keycode::T)) {
        if (m_MachineHeld == MachineName::BELT) {
            if (beltType == BeltType::FORWARD) {beltType = BeltType::LEFT;}
            else if (beltType == BeltType::LEFT) {beltType = BeltType::RIGHT;}
            else if (beltType == BeltType::RIGHT) {beltType = BeltType::FORWARD;}
        }
        else if (m_MachineHeld == MachineName::ROTATOR) {
            if (rotatorType == RotatorType::ROTATE_CW) {rotatorType = RotatorType::ROTATE_180;}
            else if (rotatorType == RotatorType::ROTATE_180) {rotatorType = RotatorType::ROTATE_CCW;}
            else if (rotatorType == RotatorType::ROTATE_CCW) {rotatorType = RotatorType::ROTATE_CW;}
        }
        else if (m_MachineHeld == MachineName::TUNNEL) {
            if (tunnelType == TunnelType::IN) {tunnelType = TunnelType::OUT;}
            else {tunnelType = TunnelType::IN;}
        }
    }

    int mouseX = std::floor((((Util::Input::GetCursorPosition().x / cam.scale.x) + cam.translation.x))/192.0f);
    int mouseY = std::floor((((Util::Input::GetCursorPosition().y / cam.scale.y) + cam.translation.y))/192.0f);

    std::shared_ptr<Machine> MachineToAdd = nullptr;
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        if (m_MachineHeld == MachineName::BELT) {
            try {MachineToAdd = std::make_shared<Belt>(mouseX, mouseY, m_MachineHeldR, beltType);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (m_MachineHeld == MachineName::BALANCER) {
            try {MachineToAdd = std::make_shared<Balancer>(mouseX, mouseY, m_MachineHeldR);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (m_MachineHeld == MachineName::TUNNEL) {
            try {MachineToAdd = std::make_shared<Tunnel>(mouseX, mouseY, m_MachineHeldR, tunnelType, false);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (m_MachineHeld == MachineName::MINER) {
            try {MachineToAdd = std::make_shared<Miner>(mouseX, mouseY, m_MachineHeldR, std::make_shared<Shape>("CuCuCuCu"));}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (m_MachineHeld == MachineName::CUTTER) {
            try {MachineToAdd = std::make_shared<Cutter>(mouseX, mouseY, m_MachineHeldR);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (m_MachineHeld == MachineName::ROTATOR) {
            try {MachineToAdd = std::make_shared<Rotator>(mouseX, mouseY, m_MachineHeldR, rotatorType);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (m_MachineHeld == MachineName::STACKER) {
            /*
            try {MachineToAdd = std::make_shared<Stacker>(mouseX, mouseY, m_MachineHeldR, BeltType::FORWARD);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
            */
        }
        if (m_MachineHeld == MachineName::MIXER) {
            /*
            try {MachineToAdd = std::make_shared<Belt>(mouseX, mouseY, m_MachineHeldR, BeltType::FORWARD);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
            */
        }
        if (m_MachineHeld == MachineName::PAINTER) {
            /*
            try {MachineToAdd = std::make_shared<Belt>(mouseX, mouseY, m_MachineHeldR, BeltType::FORWARD);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
            */
        }
        if (m_MachineHeld == MachineName::TRASH) {
            try {MachineToAdd = std::make_shared<Trash>(mouseX, mouseY);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (MachineToAdd != nullptr) {
            MachineToAdd->Init();
            m_Machines.push_back(MachineToAdd);
            m_Root.AddChild(MachineToAdd);
            MachineToAdd = nullptr;
        }
    }

    std::shared_ptr<Machine> MachineToRemove = MapMachines[{mouseX, mouseY}];
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_RB)
        && MachineToRemove != nullptr
        && MachineToRemove->getName() != MachineName::HUB) {
            m_Machines.erase(std::remove(m_Machines.begin(), m_Machines.end(), MachineToRemove), m_Machines.end());
            m_Root.RemoveChild(MachineToRemove);
            MachineToRemove->Delete();
    }
    MachineToRemove = nullptr;

    // camera movement
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

        // fix: change PTSD/src/Util/Input.cpp
        // if (delta.y >= 10) {delta.y = 0;}
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
    for (const auto& pair : MapEjectors) {
        ejector = pair.second;

        if (ejector == nullptr) {continue;}
        if (ejector->item == nullptr) {continue;}
        if (ejector->progress < 1) {continue;}

        acceptor = ejector->next;

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

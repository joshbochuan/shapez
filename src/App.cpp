#include "App.hpp"

#include "Test.hpp"
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
#include "Color.hpp"
#include "Stacker.hpp"
#include "Painter.hpp"
#include "Mixer.hpp"
#include <iostream>
#include <cmath>

void loadTextures() {
    shapeTexture = std::make_shared<Util::Image>("../Resources/shapes/shape.png");

    std::string quadChoice="CRSW", colorChoice="bcgpruwy", code;
    for (int i=0; i<4; i++) {for (int j=0; j<8; j++) {for (int k=0; k<4; k++) {
        code = quadChoice[i];
        code += colorChoice[j];
        code += std::to_string(k);
        quadTextures[code] = std::make_shared<Util::Image>("../Resources/shapes/" + code + ".png");
    }}}

    colorTextures["Color-b"] = std::make_shared<Util::Image>("../Resources/sprites/colors/blue.png");
    colorTextures["Color-c"] = std::make_shared<Util::Image>("../Resources/sprites/colors/cyan.png");
    colorTextures["Color-g"] = std::make_shared<Util::Image>("../Resources/sprites/colors/green.png");
    colorTextures["Color-p"] = std::make_shared<Util::Image>("../Resources/sprites/colors/purple.png");
    colorTextures["Color-r"] = std::make_shared<Util::Image>("../Resources/sprites/colors/red.png");
    colorTextures["Color-u"] = std::make_shared<Util::Image>("../Resources/sprites/colors/uncolored.png");
    colorTextures["Color-w"] = std::make_shared<Util::Image>("../Resources/sprites/colors/white.png");
    colorTextures["Color-y"] = std::make_shared<Util::Image>("../Resources/sprites/colors/yellow.png");

    beltForwardTexture.clear();
    beltLeftTexture.clear();
    beltRightTexture.clear();
    balancerInTextures.clear();
    balancerOutTextures.clear();
    for (int i=0; i<=13; i++) {
        beltForwardTexture.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/forward_" + std::to_string(i) +".png"));
        beltLeftTexture.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/left_" + std::to_string(i) +".png"));
        beltRightTexture.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/right_" + std::to_string(i) +".png"));
        balancerInTextures.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/forward_" + std::to_string(i) + "_bottom.png"));
        balancerOutTextures.push_back(std::make_shared<Util::Image>(
            "../Resources/sprites/belt/built/forward_" + std::to_string(i) + "_top.png"));
    }

    balancerTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/balancer.png");
    cutterTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/cutter.png");
    minerTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/miner.png");
    minerCoverTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/miner-cover.png");
    mixerTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/mixer.png");
    rotatorCWTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/rotater.png");
    rotator180Texture = std::make_shared<Util::Image>("../Resources/sprites/buildings/rotater-rotate180.png");
    rotatorCCWTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/rotater-ccw.png");
    stackerTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/stacker.png");
    trashTexture = std::make_shared<Util::Image>("../Resources/sprites/buildings/trash.png");

    painterTextures.clear();
    painterTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/painter.png"));
    painterTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/painter-mirrored.png"));

    tunnelInTextures.clear();
    tunnelOutTextures.clear();
    tunnelInTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/underground_belt_entry.png"));
    tunnelInTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/underground_belt_entry-tier2.png"));
    tunnelOutTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/underground_belt_exit.png"));
    tunnelOutTextures.push_back(std::make_shared<Util::Image>("../Resources/sprites/buildings/underground_belt_exit-tier2.png"));
}

void loadAudio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    placeBeltSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/place_belt.wav");
    placeBuildingSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/place_building.wav");
    destroyBuildingSFX = std::make_shared<Util::SFX>("../Resources/sounds/sfx/destroy_building.wav");
}

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
    loadAudio();
    loadTextures();

    // 41 minutes of ABSOLUTE BANGER
    shapezBGM = std::make_shared<Util::BGM>("../Resources/sounds/music/theme-full.mp3");
    shapezBGM->Play();

    m_MachineHeldPreview = std::make_shared<Util::GameObject>();
    m_MachineHeldPreview->SetVisible(false);
    m_MachineHeldPreview->SetZIndex(99);
    m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/belt_top.png"));
    m_Root.AddChild(m_MachineHeldPreview);

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

    std::vector<std::shared_ptr<Machine>> vec2;
    m_Machines = AddColorTest();
    vec2 = AddTunnelTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddHubTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddBalancerTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddBeltTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddCutterTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddRotatorTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    vec2 = AddPenguinTest();
    m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());

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
        m_MachineHeldPreview->SetVisible(false);
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_1)) {
        m_MachineHeld = MachineName::BELT;
        beltType = BeltType::FORWARD;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/belt_top.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_2)) {
        m_MachineHeld = MachineName::BALANCER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/balancer.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_3)) {
        m_MachineHeld = MachineName::TUNNEL;
        tunnelType = TunnelType::IN;
        tunnelUpgraded = false;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/underground_belt_entry.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_4)) {
        m_MachineHeld = MachineName::MINER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/miner.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_5)) {
        m_MachineHeld = MachineName::CUTTER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/cutter.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_6)) {
        m_MachineHeld = MachineName::ROTATOR;
        rotatorType = RotatorType::ROTATE_CW;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/rotater.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_7)) {
        m_MachineHeld = MachineName::STACKER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/stacker.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_8)) {
        m_MachineHeld = MachineName::MIXER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/mixer.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_9)) {
        painterMirrored = false;
        m_MachineHeld = MachineName::PAINTER;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/painter.png"));
        m_MachineHeldPreview->SetPivot({-84, 0});
    }
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_0)) {
        m_MachineHeld = MachineName::TRASH;
        m_MachineHeldPreview->SetVisible(true);
        m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
            "../Resources/sprites/blueprints/trash.png"));
        m_MachineHeldPreview->SetPivot({0, 0});
    }
    m_MachineHeldPreview->m_Transform.translation = Util::Input::GetCursorPosition();
    m_MachineHeldPreview->m_Transform.rotation = 0.5 * M_PI * m_MachineHeldR;
    m_MachineHeldPreview->m_Transform.scale = cam.scale;
    if (Util::Input::IsKeyDown(Util::Keycode::R) && m_MachineHeld != MachineName::NONE) {
        m_MachineHeldR = (m_MachineHeldR + 3) % 4;
    }

    // handle variants
    if (Util::Input::IsKeyDown(Util::Keycode::T)) {
        if (m_MachineHeld == MachineName::BELT) {
            if (beltType == BeltType::FORWARD) {
                beltType = BeltType::LEFT;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/belt_left.png"));
            }
            else if (beltType == BeltType::LEFT) {
                beltType = BeltType::RIGHT;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/belt_right.png"));
            }
            else if (beltType == BeltType::RIGHT) {
                beltType = BeltType::FORWARD;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/belt_top.png"));
            }
        }
        else if (m_MachineHeld == MachineName::ROTATOR) {
            if (rotatorType == RotatorType::ROTATE_CW) {
                rotatorType = RotatorType::ROTATE_180;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/rotater-rotate180.png"));
            }
            else if (rotatorType == RotatorType::ROTATE_180) {
                rotatorType = RotatorType::ROTATE_CCW;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/rotater-ccw.png"));
            }
            else if (rotatorType == RotatorType::ROTATE_CCW) {
                rotatorType = RotatorType::ROTATE_CW;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/rotater.png"));
            }
        }
        else if (m_MachineHeld == MachineName::TUNNEL) {
            if (tunnelType == TunnelType::IN && !tunnelUpgraded) {
                tunnelType = TunnelType::OUT;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/underground_belt_exit.png"));
            }
            else if (tunnelType == TunnelType::OUT && !tunnelUpgraded) {
                tunnelType = TunnelType::IN;
                tunnelUpgraded = true;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/underground_belt_entry-tier2.png"));
            }
            else if (tunnelType == TunnelType::IN && tunnelUpgraded) {
                tunnelType = TunnelType::OUT;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/underground_belt_exit-tier2.png"));
            }
            else if (tunnelType == TunnelType::OUT && tunnelUpgraded) {
                tunnelType = TunnelType::IN;
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                "../Resources/sprites/blueprints/underground_belt_entry.png"));
            }
        }
        else if (m_MachineHeld == MachineName::PAINTER) {
            if (!painterMirrored) {
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                    "../Resources/sprites/blueprints/painter-mirrored.png"));
            }
            else {
                m_MachineHeldPreview->SetDrawable(std::make_shared<Util::Image>(
                    "../Resources/sprites/blueprints/painter.png"));
            }
            painterMirrored = !painterMirrored;
        }
    }

    int mouseX = std::floor((((Util::Input::GetCursorPosition().x / cam.scale.x) + cam.translation.x))/192.0f);
    int mouseY = std::floor((((Util::Input::GetCursorPosition().y / cam.scale.y) + cam.translation.y))/192.0f);

    std::shared_ptr<Machine> MachineToAdd = nullptr;
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        if (m_MachineHeld == MachineName::BELT) {
            try {MachineToAdd = std::make_shared<Belt>(mouseX, mouseY, m_MachineHeldR, beltType);}
            catch (const std::invalid_argument& e) {}
        }
        if (m_MachineHeld == MachineName::BALANCER) {
            try {MachineToAdd = std::make_shared<Balancer>(mouseX, mouseY, m_MachineHeldR);}
            catch (const std::invalid_argument& e) {}
        }
        if (m_MachineHeld == MachineName::TUNNEL) {
            try {MachineToAdd = std::make_shared<Tunnel>(mouseX, mouseY, m_MachineHeldR, tunnelType, tunnelUpgraded);}
            catch (const std::invalid_argument& e) {}
        }
        if (m_MachineHeld == MachineName::MINER) {
            try {MachineToAdd = std::make_shared<Miner>(mouseX, mouseY, m_MachineHeldR, std::make_shared<Shape>("CuCuCuCu"));}
            catch (const std::invalid_argument& e) {}
        }
        if (m_MachineHeld == MachineName::CUTTER) {
            try {MachineToAdd = std::make_shared<Cutter>(mouseX, mouseY, m_MachineHeldR);}
            catch (const std::invalid_argument& e) {}
        }
        if (m_MachineHeld == MachineName::ROTATOR) {
            try {MachineToAdd = std::make_shared<Rotator>(mouseX, mouseY, m_MachineHeldR, rotatorType);}
            catch (const std::invalid_argument& e) {}
        }
        if (m_MachineHeld == MachineName::STACKER) {
            try {MachineToAdd = std::make_shared<Stacker>(mouseX, mouseY, m_MachineHeldR);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (m_MachineHeld == MachineName::MIXER) {
            try {MachineToAdd = std::make_shared<Mixer>(mouseX, mouseY, m_MachineHeldR);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (m_MachineHeld == MachineName::PAINTER) {
            try {MachineToAdd = std::make_shared<Painter>(mouseX, mouseY, m_MachineHeldR, painterMirrored);}
            catch (const std::invalid_argument& e) {std::cerr << e.what() << std::endl;}
        }
        if (m_MachineHeld == MachineName::TRASH) {
            try {MachineToAdd = std::make_shared<Trash>(mouseX, mouseY);}
            catch (const std::invalid_argument& e) {}
        }
        if (MachineToAdd != nullptr) {
            if (m_MachineHeld == MachineName::BELT) {placeBeltSFX->Play();} // play place_belt.wav
            else {placeBuildingSFX->Play();} // play place_building.wav
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
        destroyBuildingSFX->Play();
        m_Machines.erase(std::remove(m_Machines.begin(), m_Machines.end(), MachineToRemove), m_Machines.end());
        m_Root.RemoveChild(MachineToRemove);
        MachineToRemove->Delete();
        if (MachineToRemove.use_count() != 1) {
            throw std::invalid_argument("machine not properly removed");
        }
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

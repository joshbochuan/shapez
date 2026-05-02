//
// Created by joshb on 2026/5/2.
//

#include "UIelement/TextInput.hpp"
#include "World.hpp"
#include "Util/Input.hpp"
#include "Util/Time.hpp"
using namespace World;

TextInput::TextInput(std::shared_ptr<Util::Image> box, int textSize, Util::Color color) {
    this->textSize = textSize;

    background = std::make_shared<Button>(box);
    background->lockedBackground = box;
    background->locked = true;
    AddChild(background);

    textDisplay = std::make_shared<Text>(" ", textSize, color);
    AddChild(textDisplay);

    pointer = std::make_shared<OptiObject>();
    pointer->SetDrawable(std::make_shared<Util::Image>("../Resources/1px/333438.png"));
    pointer->m_Transform.scale = {1, 1.5f * windowPercent * textSize};
    AddChild(pointer);
}

void edit(std::string& str) {
    if (Util::Input::IsKeyDown(Util::Keycode::BACKSPACE)) {str = str.substr(0, str.length() - 1);}

    std::string toAdd;
    if (Util::Input::IsKeyDown(Util::Keycode::A)) {toAdd += "A";}
    if (Util::Input::IsKeyDown(Util::Keycode::B)) {toAdd += "B";}
    if (Util::Input::IsKeyDown(Util::Keycode::C)) {toAdd += "C";}
    if (Util::Input::IsKeyDown(Util::Keycode::D)) {toAdd += "D";}
    if (Util::Input::IsKeyDown(Util::Keycode::E)) {toAdd += "E";}
    if (Util::Input::IsKeyDown(Util::Keycode::F)) {toAdd += "F";}
    if (Util::Input::IsKeyDown(Util::Keycode::G)) {toAdd += "G";}
    if (Util::Input::IsKeyDown(Util::Keycode::H)) {toAdd += "H";}
    if (Util::Input::IsKeyDown(Util::Keycode::I)) {toAdd += "I";}
    if (Util::Input::IsKeyDown(Util::Keycode::J)) {toAdd += "J";}
    if (Util::Input::IsKeyDown(Util::Keycode::K)) {toAdd += "K";}
    if (Util::Input::IsKeyDown(Util::Keycode::L)) {toAdd += "L";}
    if (Util::Input::IsKeyDown(Util::Keycode::M)) {toAdd += "M";}
    if (Util::Input::IsKeyDown(Util::Keycode::N)) {toAdd += "N";}
    if (Util::Input::IsKeyDown(Util::Keycode::O)) {toAdd += "O";}
    if (Util::Input::IsKeyDown(Util::Keycode::P)) {toAdd += "P";}
    if (Util::Input::IsKeyDown(Util::Keycode::Q)) {toAdd += "Q";}
    if (Util::Input::IsKeyDown(Util::Keycode::R)) {toAdd += "R";}
    if (Util::Input::IsKeyDown(Util::Keycode::S)) {toAdd += "S";}
    if (Util::Input::IsKeyDown(Util::Keycode::T)) {toAdd += "T";}
    if (Util::Input::IsKeyDown(Util::Keycode::U)) {toAdd += "U";}
    if (Util::Input::IsKeyDown(Util::Keycode::V)) {toAdd += "V";}
    if (Util::Input::IsKeyDown(Util::Keycode::W)) {toAdd += "W";}
    if (Util::Input::IsKeyDown(Util::Keycode::X)) {toAdd += "X";}
    if (Util::Input::IsKeyDown(Util::Keycode::Y)) {toAdd += "Y";}
    if (Util::Input::IsKeyDown(Util::Keycode::Z)) {toAdd += "Z";}
    if (!((Util::Input::IsKeyPressed(Util::Keycode::LSHIFT)) || (Util::Input::IsKeyPressed(Util::Keycode::RSHIFT)))) {
        std::transform(toAdd.begin(), toAdd.end(), toAdd.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    }
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {toAdd += " ";}
    if (Util::Input::IsKeyDown(Util::Keycode::LEFTBRACKET)) {toAdd += "(";}
    if (Util::Input::IsKeyDown(Util::Keycode::RIGHTBRACKET)) {toAdd += ")";}
    str += toAdd;
}

void TextInput::Update() {
    m_Transform.scale = {windowPercent, windowPercent};

    background->m_Transform = m_Transform;
    background->Update();
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {selected = background->hovered;}

    if (selected) {
        edit(text);
    }

    if (text != "") {
        textDisplay->SetVisible(true);
        textDisplay->m_Text->SetText(text);
    }
    else {textDisplay->SetVisible(false);}
    textDisplay->SetPivot({-0.5f * textDisplay->m_Text->GetSize().x, 0});
    textDisplay->m_Transform = m_Transform;
    textDisplay->m_Transform.translation.x -= 0.5f * m_Transform.scale.x * background->dimension.x;
    textDisplay->m_Transform.translation.x += 0.75f * m_Transform.scale.x * textSize;

    pointer->SetVisible(selected && (std::fmod(Util::Time::GetElapsedTimeMs(), 1000)<=500));
    pointer->m_Transform.translation.x = textDisplay->m_Transform.translation.x;
    if (text != "") {
        pointer->m_Transform.translation.x += textDisplay->m_Transform.scale.x * textDisplay->m_Text->GetSize().x;
        pointer->m_Transform.translation.x -= textDisplay->m_Transform.scale.x * textSize * 0.25f;
    }

    pointer->m_Transform.translation.y = m_Transform.translation.y;
}
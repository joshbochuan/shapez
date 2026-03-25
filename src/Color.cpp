//
// Created by joshb on 2026/3/25.
//

#include "Color.hpp"

Color::Color(int color)
    : Item(ItemType::COLOR) {
    this->color = color;
    SetDrawable(colorTextures[Color::getCode()]);
    SetZIndex(20);
}

Color::Color(std::string code)
    : Item(ItemType::COLOR) {
    if (code == "Color-u") {color = 0;} // 000
    else if (code == "Color-b") {color = 1;} // 001
    else if (code == "Color-g") {color = 2;} // 010
    else if (code == "Color-c") {color = 3;} // 011
    else if (code == "Color-r") {color = 4;} // 100
    else if (code == "Color-p") {color = 5;} // 101
    else if (code == "Color-y") {color = 6;} // 110
    else if (code == "Color-w") {color = 7;} // 111
    else {throw std::invalid_argument("Invalid color " + code);}
    SetDrawable(colorTextures[code]);
    SetZIndex(20);
}

void Color::Update() {}

std::shared_ptr<Item> Color::copy() {
    return std::make_shared<Color>(color);
}

std::string Color::getCode() {
    if (color == 0) {return "Color-u";}
    if (color == 1) {return "Color-b";}
    if (color == 2) {return "Color-g";}
    if (color == 3) {return "Color-c";}
    if (color == 4) {return "Color-r";}
    if (color == 5) {return "Color-p";}
    if (color == 6) {return "Color-y";}
    if (color == 7) {return "Color-w";}
    throw std::invalid_argument("Invalid color " + std::to_string(color));
}

int Color::getColor() {
    return color;
}

bool Color::operator==(std::shared_ptr<Item> rhs) {
    if (rhs->getType() != ItemType::COLOR) {return false;}
    std::shared_ptr<Color> tmp = std::dynamic_pointer_cast<Color>(rhs);
    return tmp->getColor() == color;
}

void Color::MachineItemZIndex(float index) {
    SetZIndex(index);
}

void Color::SetItemSize(glm::vec2 s) {
    m_Transform.scale.x = s.x;
    m_Transform.scale.y = s.y;
}

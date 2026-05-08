//
// Created by joshb on 2026/3/21.
//
#include "buildings/Rotator.hpp"
#include "World.hpp"
#include "../../include/items/Shape.hpp"
#include "Util/Image.hpp"
#include <algorithm>
#include <iostream>
using namespace World;

Rotator::Rotator(int x, int y, int r, RotatorType type)
    : Machine(x, y, r, ROTATE_RATE, MachineName::ROTATOR) {
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }
    this->type = type;
    this->cooldown = 0;

    switch (this->type) {
        case RotatorType::ROTATE_CW: this->SetDrawable(rotatorCWTexture); break;
        case RotatorType::ROTATE_180: this->SetDrawable(rotator180Texture); break;
        case RotatorType::ROTATE_CCW: this->SetDrawable(rotatorCCWTexture); break;
        default: throw std::invalid_argument("Invalid rotator type");
    }
    this->SetZIndex(44 + (x+y)%2);
    this->m_Transform.rotation = M_PI * 0.5 * r;
}

std::shared_ptr<Shape> RotateCW(std::shared_ptr<Shape> shape) {
    // make the code for new shape
    std::string orig = shape->getCode();
    std::string code = "";
    int layerCnt = std::count(orig.begin(), orig.end(), ':')+1;
    for (int i=0; i<layerCnt; i++) {
        code += orig[6 + 9*i];
        code += orig[7 + 9*i];
        code += orig[0 + 9*i];
        code += orig[1 + 9*i];
        code += orig[2 + 9*i];
        code += orig[3 + 9*i];
        code += orig[4 + 9*i];
        code += orig[5 + 9*i];
        if (i < layerCnt-1) {code += ':';}
    }
    return std::make_shared<Shape>(code);
}

std::shared_ptr<Shape> Rotate180(std::shared_ptr<Shape> shape) {
    // make the code for new shape
    std::string orig = shape->getCode();
    std::string code = "";
    int layerCnt = std::count(orig.begin(), orig.end(), ':')+1;
    for (int i=0; i<layerCnt; i++) {
        code += orig[4 + 9*i];
        code += orig[5 + 9*i];
        code += orig[6 + 9*i];
        code += orig[7 + 9*i];
        code += orig[0 + 9*i];
        code += orig[1 + 9*i];
        code += orig[2 + 9*i];
        code += orig[3 + 9*i];
        if (i < layerCnt-1) {code += ':';}
    }
    return std::make_shared<Shape>(code);
}

std::shared_ptr<Shape> RotateCCW(std::shared_ptr<Shape> shape) {
    // make the code for new shape
    std::string orig = shape->getCode();
    std::string code = "";
    int layerCnt = std::count(orig.begin(), orig.end(), ':')+1;
    for (int i=0; i<layerCnt; i++) {
        code += orig[2 + 9*i];
        code += orig[3 + 9*i];
        code += orig[4 + 9*i];
        code += orig[5 + 9*i];
        code += orig[6 + 9*i];
        code += orig[7 + 9*i];
        code += orig[0 + 9*i];
        code += orig[1 + 9*i];
        if (i < layerCnt-1) {code += ':';}
    }
    return std::make_shared<Shape>(code);
}

std::string Rotator::getSaveString() {
    std::string res = "ROTATOR ";
    res += std::to_string(x) + " ";
    res += std::to_string(y) + " ";
    res += std::to_string(r) + " ";
    switch (type) {
        case RotatorType::ROTATE_180: res += "ROTATE_180"; break;
        case RotatorType::ROTATE_CCW: res += "ROTATE_CCW"; break;
        default: res += "ROTATE_CW"; break;
    }
    return res;
}

std::shared_ptr<Machine> Rotator::fromSaveString(std::vector<std::string> prop) {
    int x = std::stoi(prop[1]);
    int y = std::stoi(prop[2]);
    int r = std::stoi(prop[3]);
    RotatorType type = RotatorType::ROTATE_CW;
    if (prop[4] == "ROTATE_180") {type = RotatorType::ROTATE_180;}
    else if (prop[4] == "ROTATE_CCW") {type = RotatorType::ROTATE_CCW;}
    return std::make_shared<Rotator>(x, y, r, type);
}

void Rotator::Init() {
    this->acceptor = std::make_shared<ItemAcceptor>(this->x, this->y, this->r);
    this->acceptor->takesColor = false;
    this->AddChild(this->acceptor);
    this->ejector = std::make_shared<ItemEjector>(this->x, this->y, this->r, shared_from_this());
    this->AddChild(this->ejector);
    acceptor->Init();
    ejector->Init();

    MapMachines[{x, y}] = shared_from_this();

    MACHINE_COUNT++;
}

void Rotator::Delete() {
    MapMachines.erase({x, y});
    acceptor->Delete();
    ejector->Delete();
    MACHINE_COUNT--;
}

void Rotator::Update() {
    restored = false;

    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;
    m_Visible = ((std::abs(m_Transform.translation.x)-cam.scale.x*192 < WINDOW_WIDTH>>1)
        && (std::abs(m_Transform.translation.y)-cam.scale.y*192 < WINDOW_HEIGHT>>1));

    this->acceptor->Update();
    this->ejector->Update();
    cooldown += rate;
    backupItem = nullptr;
    if ((cooldown >= 1)
        && (this->acceptor->item != nullptr)
        && (this->acceptor->progress > 1)
        && (ejector->item == nullptr)) {
        cooldown -= 1;
        backupItem = acceptor->item;
        if (type == RotatorType::ROTATE_CW) {
            this->ejector->prep = RotateCW(std::dynamic_pointer_cast<Shape>(acceptor->item));
        }
        if (type == RotatorType::ROTATE_180) {
            this->ejector->prep = Rotate180(std::dynamic_pointer_cast<Shape>(acceptor->item));
        }
        if (type == RotatorType::ROTATE_CCW) {
            this->ejector->prep = RotateCCW(std::dynamic_pointer_cast<Shape>(acceptor->item));
        }
        this->ejector->prepProgress = this->acceptor->progress;
        acceptor->RemoveItem();
        this->acceptor->progress = 0;
    }
    if (cooldown > 1) {cooldown = 1;}
}

void Rotator::Restore(int arg, std::shared_ptr<ItemEjector> from) {
    std::cout << "called rotator restore\n";
    if (restored) {return;}
    restored = true;
    if (backupItem != nullptr) {
        cooldown += 1;
        acceptor->progress = 1;
        acceptor->Restore(1);
        acceptor->SetItem(backupItem);
    }
    ejector->prep = nullptr;
}

void Rotator::Promote() {
    acceptor->Promote();
    ejector->Promote();
}
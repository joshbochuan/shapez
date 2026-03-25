//
// Created by joshb on 2026/3/21.
//
#include "Rotator.hpp"
#include "Global.hpp"
#include "Shape.hpp"
#include "Util/Image.hpp"
#include <algorithm>
#include <iostream>

Rotator::Rotator(int x, int y, int r, RotatorType type)
    : Machine(x, y, r, ROTATE_RATE, MachineName::ROTATOR) {
    if (MapMachines[{x, y}] != nullptr) {
        throw std::invalid_argument("an machine is already at " + std::to_string(x) + ", " + std::to_string(y));
    }
    this->type = type;
    this->cooldown = 0;
    this->acceptor = std::make_shared<ItemAcceptor>(this->x, this->y, this->r);
    this->acceptor->takesColor = false;
    this->AddChild(this->acceptor);
    this->ejector = std::make_shared<ItemEjector>(this->x, this->y, this->r);
    this->AddChild(this->ejector);
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

void Rotator::Init() {
    MapMachines[{x, y}] = shared_from_this();
    acceptor->Init();
    ejector->Init();
}

void Rotator::Delete() {
    MapMachines.erase({x, y});
    acceptor->Delete();
    ejector->Delete();
}

void Rotator::Update() {
    this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
    this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;

    cooldown += rate;
    if ((cooldown >= 1)
        && (this->acceptor->item != nullptr)
        && (this->ejector->item == nullptr)
        && (this->acceptor->progress >= 1)) {
        cooldown -= 1;
        if (type == RotatorType::ROTATE_CW) {
            this->ejector->item = RotateCW(std::dynamic_pointer_cast<Shape>(acceptor->item));
        }
        if (type == RotatorType::ROTATE_180) {
            this->ejector->item = Rotate180(std::dynamic_pointer_cast<Shape>(acceptor->item));
        }
        if (type == RotatorType::ROTATE_CCW) {
            this->ejector->item = RotateCCW(std::dynamic_pointer_cast<Shape>(acceptor->item));
        }
        if (type == RotatorType::ROTATE_CCW) {}
        this->ejector->AddChild(this->ejector->item);
        this->ejector->progress = this->acceptor->progress-1;
        this->acceptor->RemoveChild(this->acceptor->item);
        this->acceptor->item = nullptr;
        this->acceptor->progress = 0;
    }
    if (cooldown > 1) {cooldown = 1;}

    this->acceptor->Update();
    this->ejector->Update();
}


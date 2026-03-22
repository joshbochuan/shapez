//
// Created by joshb on 2026/3/22.
//
#include "Balancer.hpp"
#include "Global.hpp"
#include "Util/Image.hpp"

Balancer::Balancer(int x, int y, int r)
    : Machine(x, y, r, BELT_RATE) {
    this->m_Transform.rotation = M_PI * 0.5 * static_cast<float>(r);
    this->SetDrawable(std::make_shared<Util::Image>("../Resources/Sprites/buildings/balancer.png"));
    this->SetZIndex(60 + (x+y)%2);

    this->acceptorA = std::make_shared<ItemAcceptor>(x, y, r);
    this->ejectorA = std::make_shared<ItemEjector>(x, y, r);

    switch (r) {
        case 0:
            this->acceptorB = std::make_shared<ItemAcceptor>(x+1, y, r);
            this->ejectorB = std::make_shared<ItemEjector>(x+1, y, r);
            break;
        case 1:
            this->acceptorB = std::make_shared<ItemAcceptor>(x, y-1, r);
            this->ejectorB = std::make_shared<ItemEjector>(x, y-1, r);
            break;
        case 2:
            this->acceptorB = std::make_shared<ItemAcceptor>(x-1, y, r);
            this->ejectorB = std::make_shared<ItemEjector>(x-1, y, r);
            break;
        case 3:
            this->acceptorB = std::make_shared<ItemAcceptor>(x, y+1, r);
            this->ejectorB = std::make_shared<ItemEjector>(x, y+1, r);
            break;
        default: throw std::invalid_argument("illegal balancer rotation " + std::to_string(r));
    }

    this->AddChild(this->acceptorA);
    this->AddChild(this->acceptorB);
    this->AddChild(this->ejectorA);
    this->AddChild(this->ejectorB);
    acceptors[std::make_tuple(acceptorA->x, acceptorA->y, acceptorA->r)] = acceptorA;
    acceptors[std::make_tuple(acceptorB->x, acceptorB->y, acceptorB->r)] = acceptorB;
    ejectors[std::make_tuple(ejectorA->x, ejectorA->y, ejectorA->r)] = ejectorA;
    ejectors[std::make_tuple(ejectorB->x, ejectorB->y, ejectorB->r)] = ejectorB;

    acceptPriority = 0;
    ejectPriority = 0;
}

void Balancer::Update() {
    switch (r) {
        case 0:
            this->m_Transform.translation.x = std::round(((192.0*(1+x)) - cam.translation.x) * cam.scale.x);
            this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
            break;
        case 1:
            this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
            this->m_Transform.translation.y = std::round(((192.0*(1+y)) - cam.translation.y) * cam.scale.y);
            break;
        case 2:
            this->m_Transform.translation.x = std::round(((192.0*(x)) - cam.translation.x) * cam.scale.x);
            this->m_Transform.translation.y = std::round(((192.0*(0.5+y)) - cam.translation.y) * cam.scale.y);
            break;
        case 3:
            this->m_Transform.translation.x = std::round(((192.0*(0.5+x)) - cam.translation.x) * cam.scale.x);
            this->m_Transform.translation.y = std::round(((192.0*(y)) - cam.translation.y) * cam.scale.y);
            break;
        default:
            throw std::invalid_argument("illegal cutter rotation" + std::to_string(r));
    }

    this->m_Transform.scale.x = cam.scale.x * 1.1;
    this->m_Transform.scale.y = cam.scale.y * 1.1;

    // 0 in or 0 out: don't do anything
    // 1 in 1 out: input goes to output, set priority to opposite
    // 1 in 2 out: input goes to prioritized output
    // 2 in 1 out: takes input from prioritized input
    // 2 in 2 out: same side input goes to same side output

    std::shared_ptr<Item> item;
    float progress;
    if ((acceptorA->item!=nullptr && acceptorA->progress>=1)
        && (acceptorB->item!=nullptr && acceptorB->progress>=1)
        && (ejectorA->item==nullptr)
        && (ejectorB->item==nullptr)) { // 2 in 2 out
        ejectorA->item = acceptorA->item;
        ejectorA->AddChild(ejectorA->item);
        ejectorA->progress = acceptorA->progress-1;
        acceptorA->RemoveChild(acceptorA->item);
        acceptorA->item = nullptr;
        acceptorA->progress = 0;
        ejectorB->item = acceptorB->item;
        ejectorB->AddChild(ejectorB->item);
        ejectorB->progress = acceptorB->progress-1;
        acceptorB->RemoveChild(acceptorB->item);
        acceptorB->item = nullptr;
        acceptorB->progress = 0;
    }
    else if (((acceptorA->item!=nullptr && acceptorA->progress>=1)
        ^ (acceptorB->item!=nullptr && acceptorB->progress>=1))
        && ((ejectorA->item==nullptr)
        && (ejectorB->item==nullptr))) {
        // 1 in 2 out
        if (acceptorA->item!=nullptr && acceptorA->progress>=1) {
            item = acceptorA->item;
            progress = acceptorA->progress;
            acceptPriority = 1;
            acceptorA->RemoveChild(acceptorA->item);
            acceptorA->item = nullptr;
            acceptorA->progress = 0;
        }
        else {
            item = acceptorB->item;
            progress = acceptorB->progress;
            acceptPriority = 0;
            acceptorB->RemoveChild(acceptorB->item);
            acceptorB->item = nullptr;
            acceptorB->progress = 0;
        }
        if (ejectPriority) { // outputs to prioritized one (B)
            ejectorB->item = item;
            ejectorB->progress = progress-1;
            ejectorB->AddChild(item);
            ejectPriority = 0;
        }
        else {
            ejectorA->item = item;
            ejectorA->progress = progress-1;
            ejectorA->AddChild(item);
            ejectPriority = 1;
        }
    }
    else if (((acceptorA->item!=nullptr && acceptorA->progress>=1)
        && (acceptorB->item!=nullptr && acceptorB->progress>=1))
        && ((ejectorA->item==nullptr)
        ^ (ejectorB->item==nullptr))) {
        // 2 in 1 out
        if (acceptPriority) { // B
            item = acceptorB->item;
            progress = acceptorB->progress;
            acceptPriority = 0;
            acceptorB->RemoveChild(acceptorB->item);
            acceptorB->item = nullptr;
            acceptorB->progress = 0;
        }
        else {
            item = acceptorA->item;
            progress = acceptorA->progress;
            acceptPriority = 1;
            acceptorA->RemoveChild(acceptorA->item);
            acceptorA->item = nullptr;
            acceptorA->progress = 0;
        }
        if (ejectorA->item==nullptr) {
            ejectorA->item = item;
            ejectorA->progress = progress-1;
            ejectorA->AddChild(item);
            ejectPriority = 1;
        }
        else {
            ejectorB->item = item;
            ejectorB->progress = progress-1;
            ejectorB->AddChild(item);
            ejectPriority = 0;
        }
    }
    else if (((acceptorA->item!=nullptr && acceptorA->progress>=1)
        ^ (acceptorB->item!=nullptr && acceptorB->progress>=1))
        && ((ejectorA->item==nullptr)
        ^ (ejectorB->item==nullptr))) { // 1 in 1 out
        if (acceptorA->item!=nullptr && acceptorA->progress>=1) {
            item = acceptorA->item;
            progress = acceptorA->progress;
            acceptPriority = 1;
            acceptorA->RemoveChild(acceptorA->item);
            acceptorA->item = nullptr;
            acceptorA->progress = 0;
        }
        else {
            item = acceptorB->item;
            progress = acceptorB->progress;
            acceptPriority = 0;
            acceptorB->RemoveChild(acceptorB->item);
            acceptorB->item = nullptr;
            acceptorB->progress = 0;
        }
        if (ejectorA->item==nullptr) {
            ejectorA->item = item;
            ejectorA->progress = progress-1;
            ejectorA->AddChild(item);
            ejectPriority = 1;
        }
        else {
            ejectorB->item = item;
            ejectorB->progress = progress-1;
            ejectorB->AddChild(item);
            ejectPriority = 0;
        }
    }
    acceptorA->Update();
    acceptorB->Update();
    ejectorA->Update();
    ejectorB->Update();
}
//
// Created by joshb on 2026/3/22.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_BALANCER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_BALANCER_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"

class Balancer: public Machine {
private:
    int acceptPriority, ejectPriority;
public:
    std::shared_ptr<ItemAcceptor> acceptorA, acceptorB;
    std::shared_ptr<ItemEjector> ejectorA, ejectorB;
    Balancer(int x, int y, int r);
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BALANCER_HPP
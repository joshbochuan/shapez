//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_CUTTER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_CUTTER_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"

class Cutter: public Machine {
private:
    float cooldown;
public:
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejectorA;
    std::shared_ptr<ItemEjector> ejectorB;
    Cutter(int x, int y, int r);
    void Init();
    void Update();
    void Delete();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_CUTTER_HPP
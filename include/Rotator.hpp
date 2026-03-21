//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ROTATOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ROTATOR_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"

enum class RotatorType {
    ROTATE_CW,
    ROTATE_CCW,
    ROTATE_180
};

class Rotator: public Machine {
private:
    RotatorType type;
    float cooldown;
public:
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejector;
    Rotator(int x, int y, int r, RotatorType type);
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ROTATOR_HPP
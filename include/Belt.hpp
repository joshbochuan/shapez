//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
#include "Machine.hpp"
#include "Item.hpp"

class Belt: public Machine {
public:
    Belt(int x, int y, int r);
    std::shared_ptr<Item> acceptSlot, ejectSlot;
    float acceptProgress, ejectProgress;
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
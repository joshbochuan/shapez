//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_MACHINE_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_MACHINE_HPP
#include "Util/GameObject.hpp"

class Machine: public Util::GameObject {
public:
    int x, y, r; // world x, y coordinates and rotation
    Machine(int x, int y, int r);
    virtual void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_MACHINE_HPP
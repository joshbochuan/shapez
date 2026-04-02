//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_MACHINE_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_MACHINE_HPP
#include "Util/GameObject.hpp"
#include "ShapezObject.hpp"

enum class MachineName {
    NONE,
    BALANCER,
    SPLITTER,
    MERGER,
    BELT,
    CUTTER,
    HUB,
    MINER,
    MIXER,
    PAINTER,
    ROTATOR,
    STACKER,
    TRASH,
    TUNNEL
};

class Machine: public ShapezObject, public std::enable_shared_from_this<Machine> {
private:
    MachineName name;
public:
    int x, y, r; // world x, y coordinates and rotation
    float& rate; // rate the machine would run in
    Machine(int x, int y, int r, float& rate, MachineName name);
    virtual void Init() = 0; // called right after construction using shared_ptr, add global hashmap keys
    virtual void Update() = 0;
    virtual void Delete() = 0; // called right before deconstruction, erase global hashmap keys
    MachineName getName();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_MACHINE_HPP
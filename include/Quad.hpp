//
// Created by User on 2026/3/20.
//

#ifndef SHAPEZ_QUAD_HPP
#define SHAPEZ_QUAD_HPP

#include "Util/GameObject.hpp"
#include <string>

class Quad: public Util::GameObject {
public:
    std::string code;
    int r; // rotation, 0-3 in corresponding places
    int layer; // layer, 0 is bottom, 3 is top
    Quad(std::string code, int r, int layer);
};

#endif //SHAPEZ_QUAD_HPP
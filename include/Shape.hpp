//
// Created by User on 2026/3/20.
//

#ifndef SHAPEZ_SHAPE_HPP
#define SHAPEZ_SHAPE_HPP
#include "Quad.hpp"
#include "Item.hpp"
#include "Util/GameObject.hpp"
#include <string>

class Shape: public Item {
public:
    std::vector<std::shared_ptr<Quad>> quads;
    Shape(std::string code);
    void Update();
};


#endif //SHAPEZ_SHAPE_HPP
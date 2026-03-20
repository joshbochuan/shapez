//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEM_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEM_HPP
#include "Util/GameObject.hpp"

class Item: public Util::GameObject {
public:
    glm::vec2 pos;
    Item(glm::vec2 pos);
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEM_HPP
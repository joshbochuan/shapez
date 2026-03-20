//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEM_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEM_HPP
#include "Util/GameObject.hpp"

class Item: public Util::GameObject {
public:

    enum class ItemType {
        SHAPE,
        COLOR
    };

    glm::vec2 pos;
    ItemType type;
    Item(glm::vec2 pos, ItemType type);
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEM_HPP
//
// Created by User on 2026/3/20.
//

#include "../../include/items/Item.hpp"
#include "Util/GameObject.hpp"

Item::Item(ItemType type) {
    this->type = type;
};

ItemType Item::getType() {
    return this->type;
}
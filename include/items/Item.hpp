//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEM_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEM_HPP
#include "Util/GameObject.hpp"

enum class ItemType {
    SHAPE,
    COLOR
};

class Item: public Util::GameObject {
private:
    ItemType type;
public:
    Item(ItemType type);
    ItemType getType();
    virtual void Update() = 0;
    virtual std::string getCode() = 0;
    virtual std::shared_ptr<Item> copy() = 0;
    virtual bool operator==(std::shared_ptr<Item> rhs) = 0;
    virtual void MachineItemZIndex(float index) = 0;
    virtual void SetItemSize(glm::vec2 s) = 0;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEM_HPP
//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
#include "Item.hpp"
#include "Machine.hpp"
#include "ItemAcceptor.hpp"

class ItemEjector {
public:
    std::shared_ptr<Item> item;
    float progress;
    float rate;
    ItemEjector(float rate);
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
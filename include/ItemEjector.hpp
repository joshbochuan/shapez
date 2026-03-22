//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
#include "Item.hpp"
#include "Machine.hpp"
#include "ItemAcceptor.hpp"

class ItemAcceptor;

class ItemEjector: public Machine {
public:
    std::shared_ptr<Item> item;
    float progress;
    std::weak_ptr<ItemAcceptor> prev;
    ItemEjector(int x, int y, int r);
    // ~ItemEjector();
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEMACCEPTOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEMACCEPTOR_HPP
#include "Item.hpp"
#include "ItemEjector.hpp"

class ItemAcceptor {
public:
    std::shared_ptr<Item> item;
    float progress;
    float rate;
    ItemAcceptor(float rate);
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEMACCEPTOR_HPP
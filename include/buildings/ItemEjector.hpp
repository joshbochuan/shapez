//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
#include "../items/Item.hpp"
#include "Util/GameObject.hpp"
#include "ItemAcceptor.hpp"

class ItemAcceptor;

class ItemEjector: public Util::GameObject, public std::enable_shared_from_this<ItemEjector> {
private:
    bool initialized = false;
public:
    int x, y, r;
    float& rate;
    std::shared_ptr<Item> item;
    float progress;
    std::shared_ptr<ItemAcceptor> next;
    ItemEjector(int x, int y, int r);
    void Init();
    void Update();
    void Delete();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
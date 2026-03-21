//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEMACCEPTOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEMACCEPTOR_HPP
#include "Item.hpp"
#include "ItemEjector.hpp"

class ItemAcceptor: public Machine {
public:
    std::shared_ptr<Item> item;
    float progress;
    bool takesShape, takesColor;
    ItemAcceptor(int x, int y, int r);
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEMACCEPTOR_HPP
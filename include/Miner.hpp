//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
#include "Global.hpp"
#include "Item.hpp"
#include "Machine.hpp"

class Miner: public Machine {
private:
    float cooldown;
public:
    std::shared_ptr<Item> product;
    Miner(int x, int y, int r, std::shared_ptr<Item> product);
    void Update();
    std::shared_ptr<ItemEjector> ejector;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
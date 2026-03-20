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
    std::shared_ptr<Item> product;
public:
    Miner(int x, int y, int r, float rate, std::shared_ptr<Item> product);
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
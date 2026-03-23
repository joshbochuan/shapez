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
    std::shared_ptr<Util::GameObject> cover;
public:
    std::shared_ptr<Item> product;
    Miner(int x, int y, int r, std::shared_ptr<Item> product);
    void Init();
    void Update();
    void Delete();
    std::shared_ptr<ItemEjector> ejector;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
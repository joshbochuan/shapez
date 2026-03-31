//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
#include "World.hpp"
#include "items/Item.hpp"
#include "Machine.hpp"
#include "Util/Image.hpp"

class Miner: public Machine {
private:
    bool chained = false; // whether it's a normal miner or chained miner
    int chainLen = 1; // how many of them are chained, maxed out at 5
    float cooldown;
    std::shared_ptr<Util::GameObject> cover;

public:
    static inline std::shared_ptr<Util::Image> minerTexture;
    static inline std::shared_ptr<Util::Image> chainedMinerTexture;
    static inline std::shared_ptr<Util::Image> minerCoverTexture;
    std::shared_ptr<Miner> next = nullptr;
    std::vector<std::shared_ptr<Miner>> prev;
    std::shared_ptr<Item> product;
    Miner(int x, int y, int r, std::shared_ptr<Item> product, bool chained);
    void Init();
    void Update();
    void Delete();
    bool isChained();

    std::shared_ptr<Miner> FindRoot();
    int UpdateChainLen();

    std::shared_ptr<ItemEjector> ejector;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
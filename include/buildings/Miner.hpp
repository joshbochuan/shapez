//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_MINER_HPP
#include "World.hpp"
#include "items/Item.hpp"
#include "Machine.hpp"
#include "Util/Image.hpp"
#include "Opti/OptiImage.hpp"

class Miner: public Machine {
private:
    bool chained = false; // whether it's a normal miner or chained miner
    int chainLen = 1; // how many of them are chained, maxed out at 5
    float cooldown;
    std::shared_ptr<OptiObject> cover;

public:
    static inline std::shared_ptr<OptiImage> minerTexture;
    static inline std::shared_ptr<OptiImage> chainedMinerTexture;
    static inline std::shared_ptr<OptiImage> minerCoverTextureGray;
    static inline std::shared_ptr<OptiImage> minerCoverTextureRed;
    static inline std::shared_ptr<OptiImage> minerCoverTextureGreen;
    static inline std::shared_ptr<OptiImage> minerCoverTextureBlue;
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
//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
#include "Util/Transform.hpp"
#include <unordered_map>
#include <memory>
#include <utility>

#include "Chunk.hpp"
#include "config.hpp"
#include "buildings/Hub.hpp"
#include "Opti/OptiRenderer.hpp"
#include "buildings/ItemAcceptor.hpp"
#include "buildings/ItemEjector.hpp"
#include "buildings/Machine.hpp"
#include "Util/Renderer.hpp"
#include "buildings/Hub.hpp"
#include "config.hpp"

struct PairHash {
    std::size_t operator()(const std::pair<int,int>& t) const {
        auto [a, b] = t;
        std::size_t h1 = std::hash<int>{}(a);
        std::size_t h2 = std::hash<int>{}(b);
        return h1 ^ (h2 << 1);
    }
};

struct TriHash {
    std::size_t operator()(const std::tuple<int,int,int>& t) const {
        auto [a, b, c] = t;
        std::size_t h1 = std::hash<int>{}(a);
        std::size_t h2 = std::hash<int>{}(b);
        std::size_t h3 = std::hash<int>{}(c);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class Hub;

namespace World {
    inline OptiRenderer m_Root;

    inline Util::Transform cam;
    inline float windowPercent = std::min(
        static_cast<float>(WINDOW_WIDTH)/2560.0f,
        static_cast<float>(WINDOW_HEIGHT)/1440.0f
        );

    inline std::unordered_map<std::pair<int, int>, std::shared_ptr<Chunk>, PairHash> chunks;
    inline std::unordered_map<std::string, int> warehouse; // key: item code, value: amount
    inline std::unordered_map<std::tuple<int, int, int>, std::shared_ptr<ItemAcceptor>, TriHash> MapAcceptors;
    inline std::unordered_map<std::tuple<int, int, int>, std::shared_ptr<ItemEjector>, TriHash> MapEjectors;
    inline std::unordered_map<std::pair<int, int>, std::shared_ptr<Machine>, PairHash> MapMachines;
    inline std::vector<std::shared_ptr<Machine>> LstMachines;
    inline std::vector<std::shared_ptr<ItemEjector>> LstEjectors;

    inline std::shared_ptr<Hub> hub;

    inline int SEED = 67;
    inline int LEVEL = 1;
    inline int PROGRESS = 0;
    inline long long CREATION_TIME = 0;

    inline int UPGRADE_BELT = 1;
    inline int UPGRADE_PROCESS = 1;
    inline int UPGRADE_MINE = 1;
    inline int UPGRADE_PAINT = 1;

    inline float MULTIPLIER_BELT = 1;
    inline float MULTIPLIER_PROCESS = 1;
    inline float MULTIPLIER_MINE = 1;
    inline float MULTIPLIER_PAINT = 1;

    inline int BELT_LEVEL = 1;
    inline int MINER_LEVEL = 1;
    inline int CUTTER_LEVEL = 2;
    inline int TRASH_LEVEL = 2;
    inline int UPGRADE_LEVEL = 3;
    inline int BALANCER_LEVEL = 4;
    inline int ROTATOR_LEVEL = 5;
    inline int TUNNEL_LEVEL = 6;
    inline int PAINTER_LEVEL = 7;
    inline int ROTATOR_CW_LEVEL = 8;
    inline int MIXER_LEVEL = 9;
    inline int MERGER_LEVEL = 10;
    inline int STACKER_LEVEL = 11;
    inline int CHAIN_MINER_LEVEL = 12;
    inline int BLUEPRINT_LEVEL = 13;
    inline int TUNNEL_T2_LEVEL = 14;
    inline int READER_LEVEL = 15;
    inline int STORAGE_LEVEL = 16;
    inline int QUAD_CUTTER_LEVEL = 17;
    inline int DOUBLE_PAINTER_LEVEL = 18;
    inline int ROTATOR_180_LEVEL = 19;
    inline int SPLITTER_LEVEL = 20;

    inline float BELT_RATE = 2.0/FPS_CAP;
    inline float MINE_RATE = 0.4/FPS_CAP;
    inline float CUT_RATE = 0.5/FPS_CAP;
    inline float ROTATE_RATE = 2.0/FPS_CAP;
    inline float STACK_RATE = 0.25/FPS_CAP;
    inline float PAINT_RATE = 0.334/FPS_CAP;
    inline float MIX_RATE = 0.4/FPS_CAP;

    enum class UpgradeType {
        BELT,
        PROCESS,
        MINE,
        PAINT
    };

    float getMultiplierByLevel(int level);

    void OperateMachines();
    void UpdateWorld();

    void ClearWorld();
    std::string SaveWorld(std::string save);
    void LoadWorld(std::string save);
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
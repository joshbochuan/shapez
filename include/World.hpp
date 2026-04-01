//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
#include "Util/Transform.hpp"
#include <unordered_map>
#include <memory>
#include <utility>

#include "config.hpp"
#include "ShapezRenderer.hpp"
#include "buildings/ItemAcceptor.hpp"
#include "buildings/ItemEjector.hpp"
#include "buildings/Machine.hpp"
#include "Util/Renderer.hpp"

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

namespace World {
    inline ShapezRenderer m_Root;

    inline Util::Transform cam;
    inline std::unordered_map<std::tuple<int, int, int>, std::shared_ptr<ItemAcceptor>, TriHash> MapAcceptors;
    inline std::unordered_map<std::tuple<int, int, int>, std::shared_ptr<ItemEjector>, TriHash> MapEjectors;
    inline std::unordered_map<std::pair<int, int>, std::shared_ptr<Machine>, PairHash> MapMachines;
    inline std::vector<std::shared_ptr<Machine>> LstMachines;

    inline int SEED = 67;

    inline float BELT_RATE = 2.0/FPS_CAP;
    inline float MINE_RATE = 0.4/FPS_CAP;
    inline float CUT_RATE = 0.5/FPS_CAP;
    inline float ROTATE_RATE = 0.5/FPS_CAP;
    inline float STACK_RATE = 0.5/FPS_CAP;
    inline float PAINT_RATE = 0.5/FPS_CAP;
    inline float MIX_RATE = 0.5/FPS_CAP;

    void OperateMachines();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
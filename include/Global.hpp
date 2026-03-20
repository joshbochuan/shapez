//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
#include "Util/Transform.hpp"
#include <unordered_map>
#include <memory>
#include <utility>
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        std::size_t h1 = std::hash<int>{}(p.first);
        std::size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

inline Util::Transform cam;
inline std::unordered_map<std::pair<int, int>, std::shared_ptr<ItemAcceptor>, PairHash> acceptors;
inline std::unordered_map<std::pair<int, int>, std::shared_ptr<ItemEjector>, PairHash> ejectors;

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
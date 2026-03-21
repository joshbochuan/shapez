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

struct TriHash {
    std::size_t operator()(const std::tuple<int,int,int>& t) const {
        auto [a, b, c] = t;
        std::size_t h1 = std::hash<int>{}(a);
        std::size_t h2 = std::hash<int>{}(b);
        std::size_t h3 = std::hash<int>{}(c);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

inline Util::Transform cam;
inline std::unordered_map<std::tuple<int, int, int>, std::shared_ptr<ItemAcceptor>, TriHash> acceptors;
inline std::unordered_map<std::tuple<int, int, int>, std::shared_ptr<ItemEjector>, TriHash> ejectors;

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_CAM_HPP
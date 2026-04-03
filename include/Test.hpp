//
// Created by joshb on 2026/3/25.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_TEST_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_TEST_HPP
#include <vector>
#include <memory>
#include "buildings/Machine.hpp"
#include "buildings/Miner.hpp"
#include "buildings/Belt.hpp"
#include "buildings/Tunnel.hpp"
#include "buildings/Trash.hpp"
#include "buildings/Balancer.hpp"
#include "buildings/Hub.hpp"
#include "buildings/Cutter.hpp"
#include "buildings/Mixer.hpp"
#include "buildings/Painter.hpp"
#include "buildings/Rotator.hpp"
#include "buildings/Stacker.hpp"
#include "items/Shape.hpp"
#include "items/Color.hpp"

inline std::vector<std::shared_ptr<Machine>> AddMachineBenchmark(int x, int y) {
    std::vector<std::shared_ptr<Machine>> m_Machines;
    // 40 machines
    m_Machines.push_back(std::make_shared<Belt>(x-10, y+13, 3, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(x-2, y+10, 3, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(x-2, y+13, 1, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(x-11, y+10, 3, BeltType::FORWARD));

    m_Machines.push_back(std::make_shared<Belt>(x-1, y+10, 3, BeltType::LEFT));
    m_Machines.push_back(std::make_shared<Belt>(x-1, y+13, 0, BeltType::LEFT));
    m_Machines.push_back(std::make_shared<Belt>(x-12, y+10, 2, BeltType::LEFT));
    m_Machines.push_back(std::make_shared<Belt>(x-3, y+11, 3, BeltType::LEFT));
    m_Machines.push_back(std::make_shared<Belt>(x-4, y+13, 1, BeltType::LEFT));
    m_Machines.push_back(std::make_shared<Belt>(x-5, y+13, 0, BeltType::LEFT));
    m_Machines.push_back(std::make_shared<Belt>(x-6, y+13, 1, BeltType::LEFT));

    m_Machines.push_back(std::make_shared<Belt>(x-9, y+13, 3, BeltType::RIGHT));
    m_Machines.push_back(std::make_shared<Belt>(x-9, y+12, 2, BeltType::RIGHT));
    m_Machines.push_back(std::make_shared<Belt>(x-4, y+12, 2, BeltType::RIGHT));
    m_Machines.push_back(std::make_shared<Belt>(x-5, y+12, 1, BeltType::RIGHT));

    m_Machines.push_back(std::make_shared<Trash>(x-10, y+12));
    m_Machines.push_back(std::make_shared<Trash>(x-6, y+12));

    m_Machines.push_back(std::make_shared<Balancer>(x-11, y+12, 3));
    m_Machines.push_back(std::make_shared<Stacker>(x-10, y+11, 3));
    m_Machines.push_back(std::make_shared<Cutter>(x-9, y+11, 3));
    m_Machines.push_back(std::make_shared<Mixer>(x-7, y+12, 2));

    m_Machines.push_back(std::make_shared<Miner>(x-11, y+13, 3, std::make_shared<Shape>("CuCuCuCu"), 0));
    m_Machines.push_back(std::make_shared<Miner>(x-12, y+12, 2, std::make_shared<Shape>("CuCuCuCu"), 1));
    m_Machines.push_back(std::make_shared<Miner>(x-12, y+13, 2, std::make_shared<Shape>("CuCuCuCu"), 1));
    m_Machines.push_back(std::make_shared<Miner>(x-8, y+13, 2, std::make_shared<Color>("Color-r"), 1));
    m_Machines.push_back(std::make_shared<Miner>(x-7, y+13, 2, std::make_shared<Color>("Color-g"), 1));
    m_Machines.push_back(std::make_shared<Miner>(x-4, y+10, 0, std::make_shared<Color>("Color-b"), 1));

    m_Machines.push_back(std::make_shared<Rotator>(x-8, y+10, 3, RotatorType::ROTATE_CW));
    m_Machines.push_back(std::make_shared<Rotator>(x-7, y+10, 3, RotatorType::ROTATE_180));
    m_Machines.push_back(std::make_shared<Rotator>(x-3, y+12, 0, RotatorType::ROTATE_CCW));

    m_Machines.push_back(std::make_shared<Painter>(x-8, y+11, 0, false));
    m_Machines.push_back(std::make_shared<Painter>(x-5, y+11, 0, true));

    m_Machines.push_back(std::make_shared<Splitter>(x-12, y+11, 2, true));
    m_Machines.push_back(std::make_shared<Splitter>(x-6, y+11, 3, false));
    m_Machines.push_back(std::make_shared<Merger>(x-6, y+10, 3, true));
    m_Machines.push_back(std::make_shared<Merger>(x-3, y+13, 1, true));

    m_Machines.push_back(std::make_shared<Tunnel>(x-5, y+10, 3, TunnelType::IN, false));
    m_Machines.push_back(std::make_shared<Tunnel>(x-3, y+10, 3, TunnelType::OUT, false));
    m_Machines.push_back(std::make_shared<Tunnel>(x-1, y+11, 0, TunnelType::IN, true));
    m_Machines.push_back(std::make_shared<Tunnel>(x-1, y+12, 0, TunnelType::OUT, true));

    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddMachineBenchmark10k() {
    std::vector<std::shared_ptr<Machine>> m_Machines, vec2;
    int n=250;
    for (int i=0; i<n; i++) {
        vec2 = AddMachineBenchmark(-10, -625 + 5 * i);
        m_Machines.insert(m_Machines.end(), vec2.begin(), vec2.end());
    }
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddDrawableBenchmark10k() {
    std::vector<std::shared_ptr<Machine>> m_Machines;
    int width = 36, height = 2;
    for (int i=0; i<width; i++) {
        m_Machines.push_back(std::make_shared<Miner>(i, 9, 0, std::make_shared<Shape>("CuCuCuCu:CuCuCuCu:CuCuCuCu:CuCuCuCu"), true));
        m_Machines.push_back(std::make_shared<Miner>(i, 8, 0, std::make_shared<Shape>("CuCuCuCu:CuCuCuCu:CuCuCuCu:CuCuCuCu"), true));
        m_Machines.push_back(std::make_shared<Miner>(i, 7, 0, std::make_shared<Shape>("CuCuCuCu:CuCuCuCu:CuCuCuCu:CuCuCuCu"), true));
        m_Machines.push_back(std::make_shared<Miner>(i, 6, 0, std::make_shared<Shape>("CuCuCuCu:CuCuCuCu:CuCuCuCu:CuCuCuCu"), true));
        m_Machines.push_back(std::make_shared<Miner>(i, 5, 0, std::make_shared<Shape>("CuCuCuCu:CuCuCuCu:CuCuCuCu:CuCuCuCu"), true));
        for (int j=10; j<10+height; j++) {
            m_Machines.push_back(std::make_shared<Belt>(i, j, 0, BeltType::FORWARD));
        }
        m_Machines.push_back(std::make_shared<Trash>(i, 10+height));
    }
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddChainMinerTest() {
    // contains all miners having uncolored whole shapes and rgb colors
    std::vector<std::shared_ptr<Machine>> m_Machines;
    for (int i=-2; i<=2; i++) {
        m_Machines.push_back(std::make_shared<Miner>(-10, i, 0, std::make_shared<Shape>("CuCuCuCu"), true));
        m_Machines.push_back(std::make_shared<Miner>(-9, i, 0, std::make_shared<Shape>("RuRuRuRu"), true));
        m_Machines.push_back(std::make_shared<Miner>(-8, i, 0, std::make_shared<Shape>("SuSuSuSu"), true));
        m_Machines.push_back(std::make_shared<Miner>(-7, i, 0, std::make_shared<Shape>("WuWuWuWu"), true));
        m_Machines.push_back(std::make_shared<Miner>(7, i, 0, std::make_shared<Color>("Color-r"), true));
        m_Machines.push_back(std::make_shared<Miner>(8, i, 0, std::make_shared<Color>("Color-g"), true));
        m_Machines.push_back(std::make_shared<Miner>(9, i, 0, std::make_shared<Color>("Color-b"), true));
    }
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddColorTest() {
    // tunnel test
    std::vector<std::shared_ptr<Machine>> m_Machines;
    m_Machines.push_back(std::make_shared<Miner>(-5, 10, 0, std::make_shared<Color>("Color-r"), false));
    m_Machines.push_back(std::make_shared<Miner>(-4, 10, 0, std::make_shared<Color>("Color-r"), false));
    m_Machines.push_back(std::make_shared<Miner>(-3, 10, 0, std::make_shared<Color>("Color-r"), false));
    m_Machines.push_back(std::make_shared<Miner>(-2, 10, 0, std::make_shared<Color>("Color-r"), false));
    m_Machines.push_back(std::make_shared<Miner>(-1, 10, 0, std::make_shared<Color>("Color-g"), false));
    m_Machines.push_back(std::make_shared<Miner>(0, 10, 0, std::make_shared<Color>("Color-g"), false));
    m_Machines.push_back(std::make_shared<Miner>(1, 10, 0, std::make_shared<Color>("Color-g"), false));
    m_Machines.push_back(std::make_shared<Miner>(2, 10, 0, std::make_shared<Color>("Color-g"), false));
    m_Machines.push_back(std::make_shared<Miner>(3, 10, 0, std::make_shared<Color>("Color-b"), false));
    m_Machines.push_back(std::make_shared<Miner>(4, 10, 0, std::make_shared<Color>("Color-b"), false));
    m_Machines.push_back(std::make_shared<Miner>(5, 10, 0, std::make_shared<Color>("Color-b"), false));
    m_Machines.push_back(std::make_shared<Miner>(6, 10, 0, std::make_shared<Color>("Color-b"), false));
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddTunnelTest() {
    // tunnel test
    std::vector<std::shared_ptr<Machine>> m_Machines;
    m_Machines.push_back(std::make_shared<Miner>(0, -7, 3, std::make_shared<Shape>("CuCuCuCu"), false));
    m_Machines.push_back(std::make_shared<Belt>(1, -7, 3, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Tunnel>(2, -7, 3, TunnelType::IN, false));
    m_Machines.push_back(std::make_shared<Tunnel>(5, -7, 3, TunnelType::OUT, false));
    m_Machines.push_back(std::make_shared<Belt>(6, -7, 3, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(7, -7));
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddBalancerTest() {
    std::vector<std::shared_ptr<Machine>> m_Machines;
    m_Machines.push_back(std::make_shared<Miner>(-8, -9, 0, std::make_shared<Shape>("CuCuCuCu"), false));
    m_Machines.push_back(std::make_shared<Miner>(-7, -9, 0, std::make_shared<Shape>("CuCuCuCu"), false));
    m_Machines.push_back(std::make_shared<Miner>(-6, -9, 0, std::make_shared<Shape>("CuCuCuCu"), false));
    m_Machines.push_back(std::make_shared<Miner>(-5, -9, 0, std::make_shared<Shape>("CuCuCuCu"), false));
    m_Machines.push_back(std::make_shared<Belt>(-8, -8, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-7, -8, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-6, -8, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-5, -8, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Balancer>(-8, -7, 0));
    m_Machines.push_back(std::make_shared<Balancer>(-6, -7, 0));
    m_Machines.push_back(std::make_shared<Balancer>(-7, -6, 0));
    m_Machines.push_back(std::make_shared<Belt>(-7, -5, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(-7, -4));
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddHubTest() {
    std::vector<std::shared_ptr<Machine>> m_Machines;
    m_Machines.push_back(std::make_shared<Belt>(-2, 2, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-1, 2, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(0, 2, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(1, 2, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Miner>(-2, 3, 2, std::make_shared<Shape>("CuCuCuCu"), false));
    m_Machines.push_back(std::make_shared<Miner>(-1, 3, 2, std::make_shared<Shape>("RrRrRrRr"), false));
    m_Machines.push_back(std::make_shared<Miner>(0, 3, 2, std::make_shared<Shape>("CuCuCuCu"), false));
    m_Machines.push_back(std::make_shared<Miner>(1, 3, 2, std::make_shared<Shape>("CrRgSbWw:Cr----Ww:Cr------:--Rg----"), false));
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddBeltTest() {
    std::vector<std::shared_ptr<Machine>> m_Machines;
    // belt, mine, trash test
    m_Machines.push_back(std::make_shared<Miner>(-1, 7, 3, std::make_shared<Shape>("CuCuCuCu"), false));
    m_Machines.push_back(std::make_shared<Trash>(1, 5));
    m_Machines.push_back(std::make_shared<Belt>(0, 7, 3, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(1, 7, 3, BeltType::RIGHT));
    m_Machines.push_back(std::make_shared<Belt>(1, 6, 2, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(2, 6));
    m_Machines.push_back(std::make_shared<Miner>(-1, 6, 3, std::make_shared<Shape>("RuRuRuRu"), false));
    m_Machines.push_back(std::make_shared<Miner>(-1, 8, 0, nullptr, false));
    m_Machines.push_back(std::make_shared<Miner>(-2, 8, 0, std::make_shared<Shape>("RuRuRuRu"), false));
    m_Machines.push_back(std::make_shared<Miner>(-2, 7, 1, std::make_shared<Shape>("RuRuRuRu"), false));
    m_Machines.push_back(std::make_shared<Miner>(-2, 6, 1, std::make_shared<Shape>("RuRuRuRu"), false));
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddRotatorTest() {
    std::vector<std::shared_ptr<Machine>> m_Machines;
    // rotator test
    m_Machines.push_back(std::make_shared<Miner>(5, 0, 0, std::make_shared<Shape>("CrRgSbWw:CrRgSbWw:CrRgSbWw:CrRgSbWw"), false));
    m_Machines.push_back(std::make_shared<Miner>(6, 0, 0, std::make_shared<Shape>("CrRgSbWw:CrRgSbWw:CrRgSbWw:CrRgSbWw"), false));
    m_Machines.push_back(std::make_shared<Miner>(7, 0, 0, std::make_shared<Shape>("CrRgSbWw:CrRgSbWw:CrRgSbWw:CrRgSbWw"), false));
    m_Machines.push_back(std::make_shared<Miner>(8, 0, 0, std::make_shared<Shape>("CrRgSbWw:CrRgSbWw:CrRgSbWw:CrRgSbWw"), false));
    m_Machines.push_back(std::make_shared<Belt>(5, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(6, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(7, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(8, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Rotator>(5, 2, 0, RotatorType::ROTATE_CW));
    m_Machines.push_back(std::make_shared<Rotator>(6, 2, 0, RotatorType::ROTATE_CW));
    m_Machines.push_back(std::make_shared<Rotator>(7, 2, 0, RotatorType::ROTATE_180));
    m_Machines.push_back(std::make_shared<Rotator>(8, 2, 0, RotatorType::ROTATE_CCW));
    m_Machines.push_back(std::make_shared<Rotator>(5, 3, 0, RotatorType::ROTATE_CW));
    m_Machines.push_back(std::make_shared<Belt>(6, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(7, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(8, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(5, 4, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(5, 5));
    m_Machines.push_back(std::make_shared<Trash>(6, 4));
    m_Machines.push_back(std::make_shared<Trash>(7, 4));
    m_Machines.push_back(std::make_shared<Trash>(8, 4));
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddCutterTest() {
    std::vector<std::shared_ptr<Machine>> m_Machines;
    // cutter test
    m_Machines.push_back(std::make_shared<Miner>(-5, 0, 0, std::make_shared<Shape>("CrRgSbWw:Cr----Ww:Cr------:--Rg----"), false));
    m_Machines.push_back(std::make_shared<Belt>(-5, 1, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Cutter>(-5, 2, 0));
    m_Machines.push_back(std::make_shared<Belt>(-5, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Belt>(-4, 3, 0, BeltType::FORWARD));
    m_Machines.push_back(std::make_shared<Trash>(-5, 4));
    m_Machines.push_back(std::make_shared<Trash>(-4, 4));
    return m_Machines;
}

inline std::vector<std::shared_ptr<Machine>> AddPenguinTest() {
    std::vector<std::shared_ptr<Machine>> m_Machines;
    // arknights player behavior
    std::shared_ptr<Belt> belt;
    belt = std::make_shared<Belt>(5, -3, 0, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(6, -3, 3, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(6, -4, 2, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(5, -4, 1, BeltType::RIGHT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RyCyRyCy");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(4, -2, 0, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(3, -2, 1, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(3, -3, 2, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = std::make_shared<Belt>(4, -3, 3, BeltType::LEFT);
    belt->acceptor->item = std::make_shared<Shape>("RuRuRuRu:RbCbRbCb");
    belt->acceptor->AddChild(belt->acceptor->item);
    m_Machines.push_back(belt);
    belt = nullptr;
    return m_Machines;
}

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_TEST_HPP
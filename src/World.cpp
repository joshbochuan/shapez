//
// Created by joshb on 2026/3/31.
//

#include "World.hpp"
#include <execution>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "buildings/Balancer.hpp"
#include "buildings/Belt.hpp"
#include "buildings/Cutter.hpp"
#include "buildings/Hub.hpp"
#include "buildings/Miner.hpp"
#include "buildings/Mixer.hpp"
#include "buildings/Painter.hpp"
#include "buildings/Rotator.hpp"
#include "buildings/Stacker.hpp"
#include "buildings/Trash.hpp"
#include "buildings/Tunnel.hpp"
#include "items/Color.hpp"
#include "items/Shape.hpp"
#include <thread>
#include "ThreadPool.hpp"
#include "WaitGroup.hpp"
using namespace World;

void UpdateMachines(
    ThreadPool& pool,
    std::vector<std::shared_ptr<Machine>>& LstMachines)
{
    int total = (int)LstMachines.size();
    int numThreads = pool.size();
    int chunkSize = (total + numThreads - 1) / numThreads;

    WaitGroup wg;

    for (int t = 0; t < numThreads; ++t) {
        int start = t * chunkSize;
        int end = std::min(start + chunkSize, total);
        if (start >= total) break;

        wg.Add(1);

        pool.Enqueue([start, end, &LstMachines, &wg]() {
            try {
                for (int i = start; i < end; ++i) {
                    if (LstMachines[i]) {
                        LstMachines[i]->Update();
                    }
                }
            } catch (...) {
                // log if needed
            }

            wg.Done();
        });
    }

    wg.Wait();
}

void UpdateTransferrers(
    ThreadPool& pool,
    std::vector<std::shared_ptr<ItemEjector>>& LstEjectors)
{
    int total = (int)LstEjectors.size();
    int numThreads = pool.size();
    int chunkSize = (total + numThreads - 1) / numThreads;

    WaitGroup wg;

    for (int t = 0; t < numThreads; ++t) {
        int start = t * chunkSize;
        int end = std::min(start + chunkSize, total);
        if (start >= total) break;

        wg.Add(1);

        pool.Enqueue([start, end, &LstEjectors, &wg]() {
            try {
                for (int i = start; i < end; ++i) {
                    auto& obj = LstEjectors[i];
                    if (obj) {
                        obj->Transfer();
                    }
                }
            } catch (...) {}

            wg.Done();
        });
    }

    wg.Wait();
}

void World::UpdateWorld() {
    int chunkStartX = static_cast<int>(std::floor((cam.translation.x - (WINDOW_WIDTH * 0.5f)/cam.scale.x)/(192.0f ))) >> 4;
    int chunkStartY = static_cast<int>(std::floor((cam.translation.y - (WINDOW_HEIGHT * 0.5f)/cam.scale.y)/(192.0f))) >> 4;
    int chunkEndX = static_cast<int>(std::ceil((cam.translation.x + (WINDOW_WIDTH * 0.5f)/cam.scale.x)/(192.0f))) >> 4;
    int chunkEndY = static_cast<int>(std::ceil((cam.translation.y + (WINDOW_HEIGHT * 0.5f)/cam.scale.y)/(192.0f))) >> 4;
    for (int i=chunkStartX-1; i<=chunkEndX+1; i++) {
        for (int j=chunkStartY-1; j<=chunkEndY+1; j++) {
            if (chunks[{i, j}] != nullptr) {continue;}
            chunks[{i, j}] = std::make_shared<Chunk>(i, j);
            m_Root.AddChild(chunks[{i, j}]);
        }
    }
    for (auto& [t, chunk] : chunks) {
        if (chunk == nullptr) {continue;}
        chunk->Update();
    }
    hub->Update();
    UpdateMachines(pool, LstMachines);
    UpdateTransferrers(pool, LstEjectors); // moving items from ejectors to acceptors
}

std::vector<std::vector<std::string>> parse2D(const std::string& input) {
    std::vector<std::vector<std::string>> result;
    std::stringstream ss(input);
    std::string line;
    // Split by '\n'
    while (std::getline(ss, line)) {
        std::stringstream lineStream(line);
        std::string word;
        std::vector<std::string> row;
        // Split each line by ' '
        while (lineStream >> word) {row.push_back(word);}
        result.push_back(row);
    }
    return result;
}

void World::ClearWorld() {
    for (int i = LstMachines.size()-1; i >= 0; i--) {
        LstMachines[i]->Delete();
        m_Root.RemoveChild(LstMachines[i]);
    }
    LstMachines.clear();
    MapMachines.clear();
    MapAcceptors.clear();
    MapEjectors.clear();
    warehouse.clear();
    for (auto& [t, chunk]: chunks) {
        m_Root.RemoveChild(chunk);
    }
    chunks.clear();

    SEED = 0;
    LEVEL = 1;
    PROGRESS = 0;
    UPGRADE_BELT = 1;
    UPGRADE_PROCESS = 1;
    UPGRADE_MINE = 1;
    UPGRADE_PAINT = 1;
    MULTIPLIER_BELT = 1;
    MULTIPLIER_PROCESS = 1;
    MULTIPLIER_MINE = 1;
    MULTIPLIER_PAINT = 1;

    hub->Init();
    hub->LoadState();
}

std::string World::SaveWorld(std::string save) {
    std::string res;
    res += "LEVEL " + std::to_string(LEVEL) + "\n";
    long long time = std::chrono::duration_cast<std::chrono::seconds>(
                      std::chrono::system_clock::now().time_since_epoch()
                  ).count();
    res += "LAST_PLAYED_AT " + std::to_string(time) + "\n";
    res += "CREATED_AT " + std::to_string(CREATION_TIME) + "\n";
    res += "SEED " + std::to_string(SEED) + "\n";
    res += "PROGRESS " + std::to_string(PROGRESS) + "\n";
    res += "CHEATS " + std::to_string(CHEATS) + "\n";
    res += "UPGRADE_BELT " + std::to_string(UPGRADE_BELT) + "\n";
    res += "UPGRADE_PROCESS " + std::to_string(UPGRADE_PROCESS) + "\n";
    res += "UPGRADE_MINE " + std::to_string(UPGRADE_MINE) + "\n";
    res += "UPGRADE_PAINT " + std::to_string(UPGRADE_PAINT) + "\n";

    for (auto& [item, cnt] : warehouse) {
        if (cnt == 0) {continue;}
        res += "WAREHOUSE " + item + " " + std::to_string(cnt) + "\n";
    }
    for (auto& machine: LstMachines) {res += machine->getSaveString() + "\n";}
    for (auto& [pos, ac] : MapAcceptors) {
        if (ac->item == nullptr) {continue;}
        res += ac->getSaveString() + "\n";
    }
    for (auto& [pos, ej] : MapEjectors) {
        if (ej->item == nullptr) {continue;}
        res += ej->getSaveString() + "\n";
    }

    std::ofstream file("../Saves/" + save);  // open file
    if (!file) {throw std::invalid_argument("couldn't open file");}
    file << res;
    file.close(); // optional (auto-closes when out of scope)
    return res;
}

void World::LoadWorld(std::string save) {
    ClearWorld();

    std::vector<std::vector<std::string>> MachinesToAdd;
    std::ifstream file("../Saves/" + save);
    if (file.is_open()) {
        std::ostringstream ss;
        ss << file.rdbuf(); // Read the entire file buffer into the stream
        MachinesToAdd = parse2D(ss.str());
    }
    else {return;}

    for (auto& prop : MachinesToAdd) {
        if (prop[0] == "LEVEL") {LEVEL = std::stoi(prop[1]);}
        if (prop[0] == "CREATED_AT") {CREATION_TIME = std::stoi(prop[1]);}
        if (prop[0] == "SEED") {SEED = std::stoi(prop[1]);}
        if (prop[0] == "PROGRESS") {PROGRESS = std::stoi(prop[1]);}
        if (prop[0] == "CHEATS") {CHEATS = std::stoi(prop[1]);}
        if (prop[0] == "UPGRADE_BELT") {UPGRADE_BELT = std::stoi(prop[1]);}
        if (prop[0] == "UPGRADE_PROCESS") {UPGRADE_PROCESS = std::stoi(prop[1]);}
        if (prop[0] == "UPGRADE_MINE") {UPGRADE_MINE = std::stoi(prop[1]);}
        if (prop[0] == "UPGRADE_PAINT") {UPGRADE_PAINT = std::stoi(prop[1]);}
    }

    hub->Init();
    hub->LoadState();
    MULTIPLIER_BELT = getMultiplierByLevel(UPGRADE_BELT);
    MULTIPLIER_PROCESS = getMultiplierByLevel(UPGRADE_PROCESS);
    MULTIPLIER_MINE = getMultiplierByLevel(UPGRADE_MINE);
    MULTIPLIER_PAINT = getMultiplierByLevel(UPGRADE_PAINT);

    for (auto& prop : MachinesToAdd) {
        if (prop[0] == "WAREHOUSE") {warehouse[prop[1]] = std::stoi(prop[2]);}
        else if (prop[0] == "BALANCER") {LstMachines.push_back(Balancer::fromSaveString(prop));}
        else if (prop[0] == "SPLITTER") {LstMachines.push_back(Splitter::fromSaveString(prop));}
        else if (prop[0] == "MERGER") {LstMachines.push_back(Merger::fromSaveString(prop));}
        else if (prop[0] == "BELT") {LstMachines.push_back(Belt::fromSaveString(prop));}
        else if (prop[0] == "CUTTER") {LstMachines.push_back(Cutter::fromSaveString(prop));}
        else if (prop[0] == "MINER") {LstMachines.push_back(Miner::fromSaveString(prop));}
        else if (prop[0] == "MIXER") {LstMachines.push_back(Mixer::fromSaveString(prop));}
        else if (prop[0] == "PAINTER") {LstMachines.push_back(Painter::fromSaveString(prop));}
        else if (prop[0] == "ROTATOR") {LstMachines.push_back(Rotator::fromSaveString(prop));}
        else if (prop[0] == "STACKER") {LstMachines.push_back(Stacker::fromSaveString(prop));}
        else if (prop[0] == "TRASH") {LstMachines.push_back(Trash::fromSaveString(prop));}
        else if (prop[0] == "TUNNEL") {LstMachines.push_back(Tunnel::fromSaveString(prop));}
        else if (prop[0] == "ACCEPTOR") {break;}
        else if (prop[0] == "EJECTOR") {break;}
    }
    for (auto& machine : LstMachines) {
        m_Root.AddChild(machine);
        machine->Init();
    }
    for (auto& prop : MachinesToAdd) {
        if ((prop[0] != "ACCEPTOR") && (prop[0] != "EJECTOR")) {continue;}
        int x = std::stoi(prop[1]);
        int y = std::stoi(prop[2]);
        int r = std::stoi(prop[3]);
        std::shared_ptr<Item> item = nullptr;
        if (prop[4].substr(0, 5) == "Color") {item = std::make_shared<Color>(prop[4]);}
        else if (prop[4] != "NULL") {item = std::make_shared<Shape>(prop[4]);}
        float progress = std::stof(prop[5]);
        if (prop[0] == "ACCEPTOR") {
            if (MapAcceptors[{x, y, r}] == nullptr) {
                throw std::invalid_argument("acceptor doesn't exist at " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(r));
            }
            MapAcceptors[{x, y, r}]->item = item;
            MapAcceptors[{x, y, r}]->progress = progress;
            MapAcceptors[{x, y, r}]->AddChild(item);
        }
        else if (prop[0] == "EJECTOR") {
            if (MapEjectors[{x, y, r}] == nullptr) {
                throw std::invalid_argument("ejector doesn't exist at " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(r));
            }
            MapEjectors[{x, y, r}]->item = item;
            MapEjectors[{x, y, r}]->progress = progress;
            MapEjectors[{x, y, r}]->AddChild(item);
        }
    }
}

float World::getMultiplierByLevel(int level) {
    switch (level) {
        case 1: return 1;
        case 2: return 1.5;
        case 3: return 2;
        case 4: return 3;
        case 5: return 4;
        case 6: return 6;
        case 7: return 7;
        case 8: return 8;
        default: return 8;
    }
}

int World::pseudo_random(int seed, int x, int y) {
    int n = seed;

    // Mix in x and y
    n ^= x * 0x9E3779B9;
    n ^= y * 0x85EBCA6B;

    // Bit mixing (hash-style scrambling)
    n ^= (n >> 16);
    n *= 0x7FEB352D;
    n ^= (n >> 15);
    n *= 0x846CA68B;
    n ^= (n >> 16);

    return n;
}
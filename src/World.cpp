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
using namespace World;

void World::OperateMachines() {
    std::for_each(std::execution::par, LstMachines.begin(), LstMachines.end(),
        [](const auto& machine) {
            machine->Update();
        });

    // moving items from ejectors to acceptors
    std::for_each(std::execution::par, MapEjectors.begin(), MapEjectors.end(),
        [](const auto& pair) {
        auto ejector = pair.second;
        if (ejector == nullptr) {return;}
        if (ejector->item == nullptr) {return;}
        if (ejector->progress < 1) {return;}

        auto acceptor = ejector->next;
        if (acceptor == nullptr) {return;}
        if (acceptor->item != nullptr) {return;}
        if ((ejector->item->getType() == ItemType::COLOR) && (!acceptor->takesColor)) {return;}
        if ((ejector->item->getType() == ItemType::SHAPE) && (!acceptor->takesShape)) {return;}

        acceptor->item = ejector->item;
        acceptor->progress = ejector->progress-1;
        acceptor->AddChild(acceptor->item);
        ejector->RemoveChild(acceptor->item);
        ejector->item = nullptr;
        ejector->progress = 0;
    });
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

std::string World::SaveWorld(std::string save) {
    std::string res;
    res += std::to_string(SEED) + " " + std::to_string(LEVEL) + " " + std::to_string(PROGRESS) + "\n";
    for (auto& machine: LstMachines) {
        if (machine->getName() == MachineName::BALANCER) {
            res += "BALANCER "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + "\n";
        }
        else if (machine->getName() == MachineName::SPLITTER) {
            res += "SPLITTER "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + " ";
            auto ptr = std::dynamic_pointer_cast<Splitter>(machine);
            res += std::to_string(ptr->mirrored) + "\n";
        }
        else if (machine->getName() == MachineName::BELT) {
            res += "BELT "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + " ";
            auto ptr = std::dynamic_pointer_cast<Belt>(machine);
            if (ptr->type == BeltType::FORWARD) {res += "FORWARD\n";}
            else if (ptr->type == BeltType::LEFT) {res += "LEFT\n";}
            else if (ptr->type == BeltType::RIGHT) {res += "RIGHT\n";}
        }
        else if (machine->getName() == MachineName::CUTTER) {
            res += "CUTTER "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + "\n";
        }
        else if (machine->getName() == MachineName::HUB) {
            res += "HUB\n";
        }
        else if (machine->getName() == MachineName::MERGER) {
            res += "MERGER "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + " ";
            auto ptr = std::dynamic_pointer_cast<Merger>(machine);
            res += std::to_string(ptr->mirrored) + "\n";
        }
        else if (machine->getName() == MachineName::MINER) {
            res += "MINER "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + " ";
            auto ptr = std::dynamic_pointer_cast<Miner>(machine);
            if (ptr->product == nullptr) {res += "NULL ";}
            else {res += ptr->product->getCode() + " ";}
            res += std::to_string(ptr->isChained()) + "\n";
        }
        else if (machine->getName() == MachineName::MIXER) {
            res += "MIXER "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + "\n";
        }
        else if (machine->getName() == MachineName::PAINTER) {
            res += "PAINTER "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + " ";
            auto ptr = std::dynamic_pointer_cast<Painter>(machine);
            res += std::to_string(ptr->mirrored) + "\n";
        }
        else if (machine->getName() == MachineName::ROTATOR) {
            res += "ROTATOR "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + " ";
            auto ptr = std::dynamic_pointer_cast<Rotator>(machine);
            if (ptr->type == RotatorType::ROTATE_CW) {res += "ROTATE_CW\n";}
            else if (ptr->type == RotatorType::ROTATE_180) {res += "ROTATE_180\n";}
            else if (ptr->type == RotatorType::ROTATE_CCW) {res += "ROTATE_CCW\n";}
        }
        else if (machine->getName() == MachineName::STACKER) {
            res += "STACKER "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + "\n";
        }
        else if (machine->getName() == MachineName::TRASH) {
            res += "TRASH "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + "\n";
        }
        else if (machine->getName() == MachineName::TUNNEL) {
            res += "TUNNEL "
            + std::to_string(machine->x) + " "
            + std::to_string(machine->y) + " "
            + std::to_string(machine->r) + " ";
            auto ptr = std::dynamic_pointer_cast<Tunnel>(machine);
            if (ptr->type == TunnelType::IN) {res += "IN ";}
            else if (ptr->type == TunnelType::OUT) {res += "OUT ";}
            res += std::to_string(ptr->upgraded) + "\n";
        }
    }
    for (auto& [pos, ac] : MapAcceptors) {
        if (ac->item == nullptr) {continue;}
        res += "ACCEPTOR "
            + std::to_string(ac->x) + " "
            + std::to_string(ac->y) + " "
            + std::to_string(ac->r) + " "
            + ac->item->getCode() + " "
            + std::to_string(ac->progress) + "\n";
    }
    for (auto& [pos, ej] : MapEjectors) {
        if (ej->item == nullptr) {continue;}
        res += "EJECTOR "
            + std::to_string(ej->x) + " "
            + std::to_string(ej->y) + " "
            + std::to_string(ej->r) + " "
            + ej->item->getCode() + " "
            + std::to_string(ej->progress) + "\n";
    }

    std::ofstream file("../Saves/" + save);  // open file
    if (!file) {throw std::invalid_argument("couldn't open file");}
    file << res;
    file.close(); // optional (auto-closes when out of scope)
    return res;
}

void World::LoadWorld(std::string save) {
    std::vector<std::vector<std::string>> MachinesToAdd;
    std::ifstream file("../Saves/" + save);
    if (file.is_open()) {
        std::ostringstream ss;
        ss << file.rdbuf(); // Read the entire file buffer into the stream
        MachinesToAdd = parse2D(ss.str());
    }
    else {return;}

    for (auto& machine : LstMachines) {
        machine->Delete();
        m_Root.RemoveChild(machine);
    }
    LstMachines.clear();
    MapMachines.clear();
    MapAcceptors.clear();
    MapEjectors.clear();

    SEED = std::stoi(MachinesToAdd[0][0]);
    LEVEL = std::stoi(MachinesToAdd[0][1]);
    PROGRESS = std::stoi(MachinesToAdd[0][2]);

    for (auto& prop : MachinesToAdd) {
        if (prop[0] == "BALANCER") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            LstMachines.push_back(std::make_shared<Balancer>(x, y, r));
        }
        else if (prop[0] == "SPLITTER") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            bool mirrored = std::stoi(prop[4]);
            LstMachines.push_back(std::make_shared<Splitter>(x, y, r, mirrored));
        }
        else if (prop[0] == "MERGER") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            bool mirrored = std::stoi(prop[4]);
            LstMachines.push_back(std::make_shared<Merger>(x, y, r, mirrored));
        }
        else if (prop[0] == "BELT") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            BeltType type = BeltType::FORWARD;
            if (prop[4] == "LEFT") {type = BeltType::LEFT;}
            else if (prop[4] == "RIGHT") {type = BeltType::RIGHT;}
            LstMachines.push_back(std::make_shared<Belt>(x, y, r, type));
        }
        else if (prop[0] == "CUTTER") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            LstMachines.push_back(std::make_shared<Cutter>(x, y, r));
        }
        else if (prop[0] == "HUB") {
            LstMachines.push_back(std::make_shared<Hub>());
        }
        else if (prop[0] == "MINER") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            std::shared_ptr<Item> item = nullptr;
            if (prop[4].substr(0, 5) == "Color") {item = std::make_shared<Color>(prop[4]);}
            else if (prop[4] != "NULL") {item = std::make_shared<Shape>(prop[4]);}
            bool chained = std::stoi(prop[5]);
            LstMachines.push_back(std::make_shared<Miner>(x, y, r, item, chained));
            item = nullptr;
        }
        else if (prop[0] == "MIXER") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            LstMachines.push_back(std::make_shared<Mixer>(x, y, r));
        }
        else if (prop[0] == "PAINTER") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            bool mirrored = std::stoi(prop[4]);
            LstMachines.push_back(std::make_shared<Painter>(x, y, r, mirrored));
        }
        else if (prop[0] == "ROTATOR") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            RotatorType type = RotatorType::ROTATE_CW;
            if (prop[4] == "ROTATE_180") {type = RotatorType::ROTATE_180;}
            else if (prop[4] == "ROTATE_CCW") {type = RotatorType::ROTATE_CCW;}
            LstMachines.push_back(std::make_shared<Rotator>(x, y, r, type));
        }
        else if (prop[0] == "STACKER") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            LstMachines.push_back(std::make_shared<Stacker>(x, y, r));
        }
        else if (prop[0] == "TRASH") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            LstMachines.push_back(std::make_shared<Trash>(x, y));
        }
        else if (prop[0] == "TUNNEL") {
            int x = std::stoi(prop[1]);
            int y = std::stoi(prop[2]);
            int r = std::stoi(prop[3]);
            TunnelType type = TunnelType::IN;
            if (prop[4] == "OUT") {type = TunnelType::OUT;}
            bool upgraded = std::stoi(prop[5]);
            LstMachines.push_back(std::make_shared<Tunnel>(x, y, r, type, upgraded));
        }
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
            MapAcceptors[{x, y, r}]->item = item;
            MapAcceptors[{x, y, r}]->progress = progress;
            MapAcceptors[{x, y, r}]->AddChild(item);
        }
        else if (prop[0] == "EJECTOR") {
            MapEjectors[{x, y, r}]->item = item;
            MapEjectors[{x, y, r}]->progress = progress;
            MapEjectors[{x, y, r}]->AddChild(item);
        }
    }
}
//
// Created by joshb on 2026/3/31.
//

#include "World.hpp"
#include <execution>
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
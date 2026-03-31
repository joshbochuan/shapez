//
// Created by joshb on 2026/3/31.
//

#include "World.hpp"
using namespace World;

void World::OperateMachines() {
    for (const auto& machine: LstMachines) {machine->Update();}

    // moving items from ejectors to acceptors
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejector;
    for (const auto& pair : MapEjectors) {
        ejector = pair.second;
        if (ejector == nullptr) {continue;}
        if (ejector->item == nullptr) {continue;}
        if (ejector->progress < 1) {continue;}

        acceptor = ejector->next;
        if (acceptor == nullptr) {continue;}
        if (acceptor->item != nullptr) {continue;}
        if ((ejector->item->getType() == ItemType::COLOR) && (!acceptor->takesColor)) {continue;}
        if ((ejector->item->getType() == ItemType::SHAPE) && (!acceptor->takesShape)) {continue;}

        acceptor->item = ejector->item;
        acceptor->progress = ejector->progress-1;
        acceptor->AddChild(acceptor->item);
        ejector->RemoveChild(acceptor->item);
        ejector->item = nullptr;
        ejector->progress = 0;
    }
}
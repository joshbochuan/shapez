//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
#include "Machine.hpp"
#include "Item.hpp"
#include "Util/Animation.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"

enum class BeltType {
    LEFT,
    RIGHT,
    FORWARD
};

class Belt: public Machine {
private:
    BeltType type;
    std::shared_ptr<Util::Animation> m_Animation;
public:
    Belt(int x, int y, int r, BeltType type);
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejector;
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
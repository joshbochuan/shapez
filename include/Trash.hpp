//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_TRASH_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_TRASH_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"

class Trash: public Machine {
public:
    std::vector<std::shared_ptr<ItemAcceptor>> m_Acceptors;
    Trash(int x, int y, float rate);
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_TRASH_HPP
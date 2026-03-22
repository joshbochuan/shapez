//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_HUB_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_HUB_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "Global.hpp"
#include "Item.hpp"
#include "Text.hpp"

class Hub: public Machine {
public:
    int level;
    std::shared_ptr<Item> targetItem;
    int targetAmount;
    int progress; // how much of the item is thrown into the hub already
    std::vector<std::shared_ptr<ItemAcceptor>> m_Acceptors;
    Hub();
    void Update();
    std::shared_ptr<Text> levelTxt;
    std::shared_ptr<Text> levelNumTxt;
    std::shared_ptr<Text> progressTxt;
    std::shared_ptr<Text> targetTxt;
    std::shared_ptr<Text> deliverTxt;
    std::shared_ptr<Text> toUnlockTxt;
    std::shared_ptr<Text> lockedItemTxt;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_HUB_HPP
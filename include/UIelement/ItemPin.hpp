//
// Created by User on 2026/5/29.
//

#ifndef SHAPEZ_ITEMPIN_HPP
#define SHAPEZ_ITEMPIN_HPP
#include "items/Item.hpp"
#include "Button.hpp"
#include "Text.hpp"

class ItemPin: public OptiObject {
public:
    ItemPin(std::shared_ptr<Item> item, float targetAmount, bool isCurrentGoal);
    void Update();

    std::shared_ptr<Item> item = nullptr;
    float targetAmount = 0;

    bool isCurrentGoal = false; // whether this tracks the current hub goal
    int lastUpdateLevel = 0;
    std::shared_ptr<Text> progressText, targetText;
    std::shared_ptr<Button> unpinButton, infoButton;
};

#endif //SHAPEZ_ITEMPIN_HPP

//
// Created by User on 2026/5/29.
//
#include "UIelement/ItemPin.hpp"

#include <sstream>

#include "World.hpp"
#include "BigNumStr.hpp"
using namespace World;

ItemPin::ItemPin(std::shared_ptr<Item> item, float targetAmount, bool isCurrentGoal) {
    this->item = item;
    this->targetAmount = targetAmount;
    this->isCurrentGoal = isCurrentGoal;

    item->MachineItemZIndex(81);
    item->SetItemSize({0.75f * windowPercent, 0.75f * windowPercent});
    item->Update();
    AddChild(item);

    if (isCurrentGoal) {
        unpinButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/current_goal_marker.png"));
        unpinButton->lockedBackground = unpinButton->idleBackground;
        unpinButton->idleScale = 0.75f;
    }
    else {
        unpinButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/unpin_shape.png"));
        unpinButton->idleScale = 0.375f;
    }
    unpinButton->heldScale = unpinButton->idleScale;
    unpinButton->hoverScale = unpinButton->idleScale;
    unpinButton->selectScale = unpinButton->idleScale;
    unpinButton->locked = isCurrentGoal;
    unpinButton->SetZIndex(81);
    AddChild(unpinButton);

    infoButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/info_button.png"));
    infoButton->idleScale = 0.75f;
    infoButton->heldScale = infoButton->idleScale;
    infoButton->hoverScale = infoButton->idleScale;
    infoButton->selectScale = infoButton->idleScale;
    infoButton->SetZIndex(81);
    AddChild(infoButton);

    progressText = std::make_shared<Text>("0", 32, Util::Color::FromRGB(51, 52, 56));
    progressText->m_Transform.scale = {windowPercent, windowPercent};
    progressText->SetZIndex(81);
    AddChild(progressText);

    targetText = std::make_shared<Text>("/" + BigNumStr(this->targetAmount), 24, Util::Color::FromRGB(51, 52, 56));
    targetText->m_Transform.scale = {windowPercent, windowPercent};
    targetText->SetZIndex(81);
    AddChild(targetText);

    Update();
}

void ItemPin::Update() {
    if (isCurrentGoal && (lastUpdateLevel != LEVEL)) {
        lastUpdateLevel = LEVEL;
        RemoveChild(item);
        item = hub->targetItem->copy();
        item->SetItemSize({0.75f * windowPercent, 0.75f * windowPercent});
        item->MachineItemZIndex(81);
        AddChild(item);
        targetAmount = hub->targetAmount;
        if (targetAmount >= 0) {targetText->m_Text->SetText("/" + BigNumStr(targetAmount));}
        else {
            std::ostringstream oss;
            oss << -targetAmount;
            targetText->m_Text->SetText(oss.str() + " / s");
        }
    }

    item->m_Transform.translation = m_Transform.translation;
    item->Update();

    unpinButton->m_Transform.translation = m_Transform.translation;
    unpinButton->m_Transform.translation.x -= 50.0f * windowPercent;
    unpinButton->m_Transform.translation.y += 15.0f * windowPercent;
    unpinButton->Update();

    infoButton->m_Transform.translation = m_Transform.translation;
    infoButton->m_Transform.translation.x -= 50.0f * windowPercent;
    infoButton->m_Transform.translation.y -= 15.0f * windowPercent;
    infoButton->Update();

    progressText->m_Transform.translation = m_Transform.translation;
    progressText->m_Transform.translation.x += 50.0f * windowPercent;
    progressText->m_Transform.translation.y += 15.0f * windowPercent;
    progressText->m_Text->SetText(BigNumStr(warehouse[item->getCode()]));
    progressText->SetPivot({-0.5f * progressText->m_Text->GetSize().x, 0});
    progressText->SetVisible(targetAmount >= 0);

    if (targetAmount >= 0) {
        targetText->m_Transform.translation = m_Transform.translation;
        targetText->m_Transform.translation.x += 50.0f * windowPercent;
        targetText->m_Transform.translation.y -= 15.0f * windowPercent;
        targetText->SetPivot({-0.5f * targetText->m_Text->GetSize().x, 0});
    }
    else {
        targetText->m_Transform.translation = m_Transform.translation;
        targetText->m_Transform.translation.x += 50.0f * windowPercent;
        targetText->SetPivot({-0.5f * targetText->m_Text->GetSize().x, 0});
    }
}
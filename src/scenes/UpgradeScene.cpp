//
// Created by joshb on 2026/4/12.
//
#include "scenes/UpgradeScene.hpp"

#include "BigNumStr.hpp"
#include "World.hpp"

using namespace World;

UpgradeBlob::UpgradeBlob(std::string title) {
    SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeBlob.png"));
    this->title = std::make_shared<Text>(title, 36, Util::Color::FromRGB(0, 0, 0));
    this->title->SetZIndex(93);
    this->title->SetPivot({-(this->title->m_Drawable->GetSize().x/2.0), 0});
    AddChild(this->title);
    this->tierBackground = std::make_shared<OptiObject>();
    this->tierBackground->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeTier1.png"));
    this->tierBackground->m_Transform.scale = {windowPercent, windowPercent};
    this->tierBackground->SetZIndex(93);
    AddChild(this->tierBackground);
    this->tierText = std::make_shared<Text>("TIER I", 36, Util::Color::FromRGB(255, 255, 255));
    this->tierText->SetZIndex(94);
    AddChild(this->tierText);
    upgradeButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeButtonLocked.png"));
    upgradeButton->text = std::make_shared<Text>("UPGRADE", 36, Util::Color::FromRGB(255, 255, 255));
    upgradeButton->text->SetPivot({-7.5, 0});
    upgradeButton->SetZIndex(93);
    upgradeButton->text->SetZIndex(94);
    upgradeButton->AddChild(upgradeButton->text);
    AddChild(upgradeButton);
    multiplier = std::make_shared<Text>("Speed x1.00 -> x1.50", 36, Util::Color::FromRGB(172, 172, 172));
    multiplier->SetPivot({multiplier->m_Text->GetSize().x/2.0f, 0});
    multiplier->SetZIndex(93);
    AddChild(multiplier);
}

void UpgradeBlob::Update() {
    title->m_Transform.scale = m_Transform.scale;
    title->m_Transform.translation.x = m_Transform.translation.x - windowPercent * 480;
    title->m_Transform.translation.y = m_Transform.translation.y + windowPercent * 90;
    tierBackground->m_Transform.translation.x = -575*m_Transform.scale.x + m_Transform.translation.x;
    tierBackground->m_Transform.translation.y = 90*m_Transform.scale.y + m_Transform.translation.y;
    tierText->m_Transform = tierBackground->m_Transform;
    upgradeButton->m_Transform.translation.x = 550*m_Transform.scale.x + m_Transform.translation.x;
    upgradeButton->m_Transform.translation.y = -25*m_Transform.scale.y + m_Transform.translation.y;
    upgradeButton->Update();
    multiplier->m_Transform.scale = m_Transform.scale;
    multiplier->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * 675;
    multiplier->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 90;
    for (int i=0; i<itemTargets.size(); i++) {
        itemTargets[i]->SetItemSize(m_Transform.scale);
        itemTargets[i]->m_Transform.translation.x = m_Transform.scale.x * static_cast<float>(-575 + (212.5*i));
        itemTargets[i]->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 0.0f;
        itemTargets[i]->Update();
        progressBars[i]->m_Transform.scale = m_Transform.scale;
        progressBars[i]->m_Transform.translation.x = m_Transform.scale.x * static_cast<float>(-575 + (212.5*i));
        progressBars[i]->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -90.0f;
        progressBars[i]->progress = progresses[i];
        progressBars[i]->text->m_Text->SetText(BigNumStr(progresses[i]) + " / " + BigNumStr(static_cast<int>(progressBars[i]->target)));
        progressBars[i]->Update();
    }
}

void UpgradeBlob::ClearTarget() {
    for (auto& item: itemTargets) {RemoveChild(item);}
    itemTargets.clear();
    for (auto& bar: progressBars) {RemoveChild(bar);}
    progressBars.clear();
    progresses.clear();
}

void UpgradeBlob::AddTarget(std::shared_ptr<Item> item, int &progress, int target) {
    item->MachineItemZIndex(93);
    itemTargets.push_back(item);
    item->Update();
    AddChild(item);
    progressBars.push_back(std::make_shared<ProgressBar>(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeProgressBar.png")));
    progressBars.back()->SetBackground(std::make_shared<Util::Image>("../Resources/1px/E9EAEC.png"));
    progressBars.back()->SetBar(std::make_shared<Util::Image>("../Resources/1px/E3E7EA.png"));
    progressBars.back()->SetBarComplete(std::make_shared<Util::Image>("../Resources/1px/58B55C.png"));
    progressBars.back()->progress = progress;
    progressBars.back()->target = target;
    progressBars.back()->SetBarZIndex(93);
    progressBars.back()->text = std::make_shared<Text>("0 / " + BigNumStr(target), 24, Util::Color::FromRGB(0, 0, 0));
    progressBars.back()->text->SetZIndex(94);
    progressBars.back()->AddChild(progressBars.back()->text);
    AddChild(progressBars.back());
    progresses.push_back(progress);
}

UpgradeScene::UpgradeScene() {
    title = std::make_shared<Text>("UPGRADES", 64, Util::Color::FromRGB(0, 0, 0));
    title->SetZIndex(92);
    title->m_Transform.scale = {windowPercent, windowPercent};
    title->m_Transform.translation = {windowPercent * (-520), windowPercent * 540};
    AddChild(title);

    background = std::make_shared<OptiObject>();
    background->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradebackground.png"));
    background->m_Transform.scale = {windowPercent, windowPercent};
    background->SetZIndex(91);
    AddChild(background);

    closeButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/close.png"));
    closeButton->m_Transform.scale = {windowPercent, windowPercent};
    closeButton->SetZIndex(92);
    closeButton->m_Transform.translation = {windowPercent*650, windowPercent*550};
    AddChild(closeButton);

    beltBlob = std::make_shared<UpgradeBlob>("Belts, Distributor & Tunnels");
    beltBlob->m_Transform.scale = {windowPercent, windowPercent};
    beltBlob->m_Transform.translation = {windowPercent * (-12), windowPercent * 360};
    beltBlob->SetZIndex(92);
    beltBlob->AddTarget(std::make_shared<Shape>("CuCuCuCu"), PROGRESS, 20);
    beltBlob->AddTarget(std::make_shared<Shape>("RuRuRuRu"), PROGRESS, 30);
    AddChild(beltBlob);

    mineBlob = std::make_shared<UpgradeBlob>("Extraction");
    mineBlob->m_Transform.scale = {windowPercent, windowPercent};
    mineBlob->m_Transform.translation = {windowPercent * (-12), windowPercent * 90};
    mineBlob->SetZIndex(92);
    AddChild(mineBlob);

    processBlob = std::make_shared<UpgradeBlob>("Cutting, Rotating & Stacking");
    processBlob->m_Transform.scale = {windowPercent, windowPercent};
    processBlob->m_Transform.translation = {windowPercent * (-12), windowPercent * -180};
    processBlob->SetZIndex(92);
    AddChild(processBlob);

    paintBlob = std::make_shared<UpgradeBlob>("Mixing & Painting");
    paintBlob->m_Transform.scale = {windowPercent, windowPercent};
    paintBlob->m_Transform.translation = {windowPercent * (-12), windowPercent * -450};
    paintBlob->SetZIndex(92);
    AddChild(paintBlob);
}

std::shared_ptr<Scene> UpgradeScene::Update() {
    beltBlob->Update();
    processBlob->Update();
    mineBlob->Update();
    paintBlob->Update();
    closeButton->Update();
    if (closeButton->released) {
        return nullptr;
    }
    return shared_from_this();
}

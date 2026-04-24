//
// Created by joshb on 2026/4/12.
//
#include "scenes/UpgradeScene.hpp"

#include "BigNumStr.hpp"
#include "World.hpp"
#include <string>
#include <iomanip>
#include <sstream>

#include "spdlog/fmt/bundled/color.h"

using namespace World;

void AddTargetsByLevel(std::shared_ptr<UpgradeBlob> blob, int level) {
    blob->ClearTarget();
    UpgradeType type = blob->type;
    if (level == 6) {
        blob->AddTarget(std::make_shared<Shape>("CpRpCp--:SwSwSwSw"), 25000);
    }
    else if (level == 7) {
        blob->AddTarget(std::make_shared<Shape>("CpRpCp--:SwSwSwSw"), 25000);
        blob->AddTarget(std::make_shared<Shape>("RuCw--Cw:----Ru--"), 50000);
    }
    else if (level >= 8) {
        blob->AddTarget(std::make_shared<Shape>("CpRpCp--:SwSwSwSw"), 30000);
        blob->AddTarget(std::make_shared<Shape>("RuCw--Cw:----Ru--"), 20000);
        blob->AddTarget(std::make_shared<Shape>("CbCuCbCu:Sr------:--CrSrCr:CwCwCwCw"), 20000);
    }
    else if (type == UpgradeType::BELT) {
        if (level == 1) {
            blob->AddTarget(std::make_shared<Shape>("CuCuCuCu"), 30);
        }
        else if (level == 2) {
            blob->AddTarget(std::make_shared<Shape>("CuCuCuCu"), 75);
            blob->AddTarget(std::make_shared<Shape>("--CuCu--"), 500);
        }
        else if (level == 3) {
            blob->AddTarget(std::make_shared<Shape>("CuCuCuCu"), 180);
            blob->AddTarget(std::make_shared<Shape>("--CuCu--"), 1200);
            blob->AddTarget(std::make_shared<Shape>("CpCpCpCp"), 1000);
        }
        else if (level == 4) {
            blob->AddTarget(std::make_shared<Shape>("CuCuCuCu"), 440);
            blob->AddTarget(std::make_shared<Shape>("--CuCu--"), 3000);
            blob->AddTarget(std::make_shared<Shape>("CpCpCpCp"), 2500);
            blob->AddTarget(std::make_shared<Shape>("SrSrSrSr:CyCyCyCy"), 6000);
        }
        else if (level == 5) {
            blob->AddTarget(std::make_shared<Shape>("CuCuCuCu"), 1100);
            blob->AddTarget(std::make_shared<Shape>("--CuCu--"), 7000);
            blob->AddTarget(std::make_shared<Shape>("CpCpCpCp"), 6000);
            blob->AddTarget(std::make_shared<Shape>("SrSrSrSr:CyCyCyCy"), 15000);
            blob->AddTarget(std::make_shared<Shape>("SrSrSrSr:CyCyCyCy:SwSwSwSw"), 25000);
        }
    }
    else if (type == UpgradeType::PROCESS) {
        if (level == 1) {
            blob->AddTarget(std::make_shared<Shape>("SuSuSuSu"), 500);
        }
        else if (level == 2) {
            blob->AddTarget(std::make_shared<Shape>("SuSuSuSu"), 1200);
            blob->AddTarget(std::make_shared<Shape>("RuRu----"), 600);
        }
        else if (level == 3) {
            blob->AddTarget(std::make_shared<Shape>("SuSuSuSu"), 3000);
            blob->AddTarget(std::make_shared<Shape>("RuRu----"), 1500);
            blob->AddTarget(std::make_shared<Shape>("CgScScCg"), 3500);
        }
        else if (level == 4) {
            blob->AddTarget(std::make_shared<Shape>("SuSuSuSu"), 7000);
            blob->AddTarget(std::make_shared<Shape>("RuRu----"), 3500);
            blob->AddTarget(std::make_shared<Shape>("CgScScCg"), 8000);
            blob->AddTarget(std::make_shared<Shape>("CwCrCwCr:SgSgSgSg"), 25000);
        }
        else if (level == 5) {
            blob->AddTarget(std::make_shared<Shape>("SuSuSuSu"), 17000);
            blob->AddTarget(std::make_shared<Shape>("RuRu----"), 800);
            blob->AddTarget(std::make_shared<Shape>("CgScScCg"), 20000);
            blob->AddTarget(std::make_shared<Shape>("CwCrCwCr:SgSgSgSg"), 60000);
            blob->AddTarget(std::make_shared<Shape>("WrRgWrRg:CwCrCwCr:SgSgSgSg"), 50000);
        }
    }
    else if (type == UpgradeType::MINE) {
        if (level == 1) {
            blob->AddTarget(std::make_shared<Shape>("RuRuRuRu"), 300);
        }
        else if (level == 2) {
            blob->AddTarget(std::make_shared<Shape>("RuRuRuRu"), 740);
            blob->AddTarget(std::make_shared<Shape>("Cu------"), 800);
        }
        else if (level == 3) {
            blob->AddTarget(std::make_shared<Shape>("RuRuRuRu"), 1800);
            blob->AddTarget(std::make_shared<Shape>("Cu------"), 2000);
            blob->AddTarget(std::make_shared<Shape>("ScScScSc"), 3500);
        }
        else if (level == 4) {
            blob->AddTarget(std::make_shared<Shape>("RuRuRuRu"), 4500);
            blob->AddTarget(std::make_shared<Shape>("Cu------"), 5000);
            blob->AddTarget(std::make_shared<Shape>("ScScScSc"), 8000);
            blob->AddTarget(std::make_shared<Shape>("CwCwCwCw:WbWbWbWb"), 23000);
        }
        else if (level == 5) {
            blob->AddTarget(std::make_shared<Shape>("RuRuRuRu"), 11000);
            blob->AddTarget(std::make_shared<Shape>("Cu------"), 12000);
            blob->AddTarget(std::make_shared<Shape>("ScScScSc"), 20000);
            blob->AddTarget(std::make_shared<Shape>("CwCwCwCw:WbWbWbWb"), 50000);
            blob->AddTarget(std::make_shared<Shape>("CbRbRbCb:CwCwCwCw:WbWbWbWb"), 50000);
        }
    }
    else if (type == UpgradeType::PAINT) {
        if (level == 1) {
            blob->AddTarget(std::make_shared<Shape>("RbRb----"), 600);
        }
        else if (level == 2) {
            blob->AddTarget(std::make_shared<Shape>("RbRb----"), 1500);
            blob->AddTarget(std::make_shared<Shape>("WrWrWrWr"), 3800);
        }
        else if (level == 3) {
            blob->AddTarget(std::make_shared<Shape>("RbRb----"), 3500);
            blob->AddTarget(std::make_shared<Shape>("WrWrWrWr"), 9000);
            blob->AddTarget(std::make_shared<Shape>("RpRpRpRp:CwCwCwCw"), 6500);
        }
        else if (level == 4) {
            blob->AddTarget(std::make_shared<Shape>("RbRb----"), 8000);
            blob->AddTarget(std::make_shared<Shape>("WrWrWrWr"), 20000);
            blob->AddTarget(std::make_shared<Shape>("RpRpRpRp:CwCwCwCw"), 16000);
            blob->AddTarget(std::make_shared<Shape>("WpWpWpWp:CwCwCwCw:WpWpWpWp"), 25000);
        }
        else if (level == 5) {
            blob->AddTarget(std::make_shared<Shape>("RbRb----"), 20000);
            blob->AddTarget(std::make_shared<Shape>("WrWrWrWr"), 50000);
            blob->AddTarget(std::make_shared<Shape>("RpRpRpRp:CwCwCwCw"), 40000);
            blob->AddTarget(std::make_shared<Shape>("WpWpWpWp:CwCwCwCw:WpWpWpWp"), 60000);
            blob->AddTarget(std::make_shared<Shape>("WpWpWpWp:CwCwCwCw:WpWpWpWp:CwCwCwCw"), 50000);
        }
    }
}

std::string intToRoman(int num) {
    // Value-symbol pairs in descending order
    int values[] =    {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    std::string symbols[] ={"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

    std::string result = "";

    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            result += symbols[i];
            num -= values[i];
        }
    }

    return result;
}

UpgradeBlob::UpgradeBlob(UpgradeType type, std::string title) {
    this->type = type;
    SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeBlob.png"));
    this->title = std::make_shared<Text>(title, 36, Util::Color::FromRGB(94, 94, 94));
    this->title->SetZIndex(94);
    this->title->SetPivot({-(this->title->m_Drawable->GetSize().x/2.0), 0});
    AddChild(this->title);
    this->tierBackground = std::make_shared<OptiObject>();
    this->tierBackground->SetDrawable(tierBackgroundImages[0]);
    this->tierBackground->m_Transform.scale = {windowPercent, windowPercent};
    this->tierBackground->SetZIndex(94);
    AddChild(this->tierBackground);
    this->tierText = std::make_shared<Text>("TIER I", 36, Util::Color::FromRGB(255, 255, 255));
    this->tierText->SetZIndex(95);
    this->tierText->SetPivot({-5, 0});
    AddChild(this->tierText);
    upgradeButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeButton.png"));
    upgradeButton->hoveredBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeButtonHovered.png");
    upgradeButton->lockedBackground = std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeButtonLocked.png");
    upgradeButton->text = std::make_shared<Text>("UPGRADE", 36, Util::Color::FromRGB(255, 255, 255));
    upgradeButton->text->SetPivot({-7.5, 0});
    upgradeButton->SetZIndex(94);
    upgradeButton->text->SetZIndex(95);
    upgradeButton->AddChild(upgradeButton->text);
    AddChild(upgradeButton);
    multiplier = std::make_shared<Text>("Speed x1.00 -> x1.50", 36, Util::Color::FromRGB(172, 172, 172));
    multiplier->SetPivot({multiplier->m_Text->GetSize().x/2.0f, 0});
    multiplier->SetZIndex(94);
    AddChild(multiplier);
    Update();
}

std::string formatSpeed(float from, float to) {
    std::ostringstream oss;
    oss << "Speed x"
        << std::fixed << std::setprecision(2) << from
        << " -> x"
        << std::fixed << std::setprecision(2) << to;
    return oss.str();
}

void UpgradeBlob::Update() {
    int* contextLevel;
    switch (type) {
        case UpgradeType::BELT: contextLevel = &UPGRADE_BELT; break;
        case UpgradeType::MINE: contextLevel = &UPGRADE_MINE; break;
        case UpgradeType::PROCESS: contextLevel = &UPGRADE_PROCESS; break;
        case UpgradeType::PAINT: contextLevel = &UPGRADE_PAINT; break;
        default: contextLevel = nullptr; break;
    }
    if (contextLevel == nullptr) {throw std::invalid_argument("invalid upgrade blob type");}

    title->m_Transform.scale = m_Transform.scale;
    title->m_Transform.translation.x = m_Transform.translation.x - windowPercent * 480;
    title->m_Transform.translation.y = m_Transform.translation.y + windowPercent * 90;
    tierBackground->m_Transform.translation.x = -575*m_Transform.scale.x + m_Transform.translation.x;
    tierBackground->m_Transform.translation.y = 90*m_Transform.scale.y + m_Transform.translation.y;
    if (*contextLevel >= 8) {tierBackground->SetDrawable(tierBackgroundImages[7]);}
    else {tierBackground->SetDrawable(tierBackgroundImages[(*contextLevel)-1]);}

    tierText->m_Transform = tierBackground->m_Transform;
    tierText->m_Text->SetText("TIER " + intToRoman(*contextLevel));

    multiplier->m_Transform.scale = m_Transform.scale;
    multiplier->m_Transform.translation.x = m_Transform.translation.x + m_Transform.scale.x * 675;
    multiplier->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 90;
    multiplier->m_Text->SetText(formatSpeed(getMultiplierByLevel(*contextLevel), getMultiplierByLevel((*contextLevel)+1)));

    bool upgradeAvailable = true;
    for (int i=0; i<itemTargets.size(); i++) {
        itemTargets[i]->SetItemSize(m_Transform.scale);
        itemTargets[i]->m_Transform.translation.x = m_Transform.scale.x * static_cast<float>(-575 + (212.5*i));
        itemTargets[i]->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * 0.0f;
        itemTargets[i]->Update();
        progressBars[i]->m_Transform.scale = m_Transform.scale;
        progressBars[i]->m_Transform.translation.x = m_Transform.scale.x * static_cast<float>(-575 + (212.5*i));
        progressBars[i]->m_Transform.translation.y = m_Transform.translation.y + m_Transform.scale.y * -90.0f;
        progressBars[i]->progress = warehouse[itemTargets[i]->getCode()];
        if (progressBars[i]->progress < progressBars[i]->target) {upgradeAvailable = false;}
        progressBars[i]->text->m_Text->SetText(BigNumStr(warehouse[itemTargets[i]->getCode()]) + " / " + BigNumStr(static_cast<int>(progressBars[i]->target)));
        progressBars[i]->Update();
    }
    upgradeButton->locked = !upgradeAvailable;
    upgradeButton->m_Transform.translation.x = 550*m_Transform.scale.x + m_Transform.translation.x;
    upgradeButton->m_Transform.translation.y = -25*m_Transform.scale.y + m_Transform.translation.y;
    upgradeButton->Update();
    if (!upgradeButton->released) {return;}


    (*contextLevel)++;
    AddTargetsByLevel(shared_from_this(), *contextLevel);
    for (int i=0; i<itemTargets.size(); i++) {warehouse[itemTargets[i]->getCode()] -= progressBars[i]->target;}
    switch (type) {
        case UpgradeType::BELT: MULTIPLIER_BELT = getMultiplierByLevel(*contextLevel); break;
        case UpgradeType::MINE: MULTIPLIER_MINE = getMultiplierByLevel(*contextLevel); break;
        case UpgradeType::PROCESS: MULTIPLIER_PROCESS = getMultiplierByLevel(*contextLevel); break;
        case UpgradeType::PAINT: MULTIPLIER_PAINT = getMultiplierByLevel(*contextLevel); break;
        default: contextLevel = nullptr; break;
    }
    upgradeSFX->Play();
    Update();
}

void UpgradeBlob::ClearTarget() {
    for (auto& item: itemTargets) {RemoveChild(item);}
    itemTargets.clear();
    for (auto& bar: progressBars) {RemoveChild(bar);}
    progressBars.clear();
}

void UpgradeBlob::AddTarget(std::shared_ptr<Item> item, int target) {
    item->MachineItemZIndex(94);
    itemTargets.push_back(item);
    item->Update();
    AddChild(item);
    progressBars.push_back(std::make_shared<ProgressBar>(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradeProgressBar.png")));
    progressBars.back()->SetBackground(std::make_shared<Util::Image>("../Resources/1px/E2E4E6.png"));
    progressBars.back()->SetBar(std::make_shared<Util::Image>("../Resources/1px/BDBFCA.png"));
    progressBars.back()->SetBarComplete(std::make_shared<Util::Image>("../Resources/1px/58B55C.png"));
    progressBars.back()->progress = warehouse[item->getCode()];
    progressBars.back()->target = target;
    progressBars.back()->SetBarZIndex(94);
    progressBars.back()->text = std::make_shared<Text>("0 / " + BigNumStr(target), 24, Util::Color::FromRGB(94, 94, 94));
    progressBars.back()->text->SetZIndex(95);
    progressBars.back()->AddChild(progressBars.back()->text);
    progressBars.back()->text->SetPivot({-5, 0});
    AddChild(progressBars.back());
}

UpgradeScene::UpgradeScene() {
    title = std::make_shared<Text>("UPGRADES", 64, Util::Color::FromRGB(94, 94, 94));
    title->SetZIndex(93);
    title->m_Transform.scale = {windowPercent, windowPercent};
    title->m_Transform.translation = {windowPercent * (-520), windowPercent * 540};
    AddChild(title);

    background = std::make_shared<OptiObject>();
    background->SetDrawable(std::make_shared<Util::Image>("../Resources/ui/blobs/upgradebackground.png"));
    background->m_Transform.scale = {windowPercent, windowPercent};
    background->SetZIndex(92);
    AddChild(background);

    blur = std::make_shared<OptiObject>();
    blur->SetDrawable(std::make_shared<Util::Image>("../Resources/1px/blur.png"));
    blur->m_Transform.scale = {WINDOW_WIDTH, WINDOW_HEIGHT};
    blur->SetZIndex(91);
    AddChild(blur);

    closeButton = std::make_shared<Button>(std::make_shared<Util::Image>("../Resources/ui/icons/close.png"));
    closeButton->m_Transform.scale = {windowPercent, windowPercent};
    closeButton->SetZIndex(93);
    closeButton->m_Transform.translation = {windowPercent*650, windowPercent*550};
    AddChild(closeButton);

    beltBlob = std::make_shared<UpgradeBlob>(UpgradeType::BELT, "Belts, Distributor & Tunnels");
    beltBlob->m_Transform.scale = {windowPercent, windowPercent};
    beltBlob->m_Transform.translation = {windowPercent * (-12), windowPercent * 360};
    beltBlob->SetZIndex(93);
    AddTargetsByLevel(beltBlob, UPGRADE_BELT);
    AddChild(beltBlob);
    beltBlob->Update();

    mineBlob = std::make_shared<UpgradeBlob>(UpgradeType::MINE, "Extraction");
    mineBlob->m_Transform.scale = {windowPercent, windowPercent};
    mineBlob->m_Transform.translation = {windowPercent * (-12), windowPercent * 90};
    mineBlob->SetZIndex(93);
    AddTargetsByLevel(mineBlob, UPGRADE_MINE);
    AddChild(mineBlob);
    mineBlob->Update();

    processBlob = std::make_shared<UpgradeBlob>(UpgradeType::PROCESS, "Cutting, Rotating & Stacking");
    processBlob->m_Transform.scale = {windowPercent, windowPercent};
    processBlob->m_Transform.translation = {windowPercent * (-12), windowPercent * -180};
    processBlob->SetZIndex(93);
    AddTargetsByLevel(processBlob, UPGRADE_PROCESS);
    AddChild(processBlob);
    processBlob->Update();

    paintBlob = std::make_shared<UpgradeBlob>(UpgradeType::PAINT, "Mixing & Painting");
    paintBlob->m_Transform.scale = {windowPercent, windowPercent};
    paintBlob->m_Transform.translation = {windowPercent * (-12), windowPercent * -450};
    paintBlob->SetZIndex(93);
    AddTargetsByLevel(paintBlob, UPGRADE_PAINT);
    AddChild(paintBlob);
    paintBlob->Update();
}

std::shared_ptr<Scene> UpgradeScene::Update() {
    beltBlob->Update();
    processBlob->Update();
    mineBlob->Update();
    paintBlob->Update();
    closeButton->Update();
    if (closeButton->released || Util::Input::IsKeyUp(Util::Keycode::F)) {
        return nullptr;
    }
    return shared_from_this();
}

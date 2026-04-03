//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_HUB_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_HUB_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "World.hpp"
#include "../items/Item.hpp"
#include "../UIelement/Text.hpp"
#include "Util/SFX.hpp"
#include "Opti/OptiImage.hpp"

class Hub: public Machine {
public:
    std::shared_ptr<Util::SFX> levelUpSFX = nullptr;
    int level;
    std::shared_ptr<Item> targetItem;
    int targetAmount;
    int progress; // how much of the item is thrown into the hub already
    std::vector<std::shared_ptr<ItemAcceptor>> m_Acceptors;

    // og are
    std::vector<std::tuple<std::string, int, std::string>> levelTargets = {
        {"CuCuCuCu",                            30, "Cutter and Trash"}, // og1
        {"----CuCu",                            40, "Upgrades"}, // og2
        {"RuRuRuRu",                            70, "Balancer"}, // og3
        {"RuRu----",                            125, "Rotator (CW)"}, // og4
        {"Cu----Cu",                            170, "Tunnel"}, // level 5, og5
        {"Cu------",                            270, "Painter"}, // og6
        {"CrCrCrCr",                            300, "Splitter"}, // og7
        {"RbRbRbRb",                            480, "Color Mixer"}, // og8
        {"CpCpCpCp",                            600, "Merger"}, // og9
        {"ScScScSc",                            800, "Stacker"}, // level 10, og10
        {"CbCbCbRb:CwCwCwCw",                   1000, "Blueprint"}, // bp, og12
        {"RpRpRpRp:CwCwCwCw",                   1000, "Next Level"}, // og13
        {"SrSrSrSr:CyCyCyCy:SwSwSwSw",          2500, "Rotator (CCW)"}, // og16
        {"CbRbRbCb:CwCwCwCw:WbWbWbWb",          3800, "Rotator (180)"}, // og17
        {"Sg----Sg:CgCgCgCg:--CyCy--",          6000, "Chaining Miner"}, // level 15, og18
        {"Cg----Cr:Cw----Cw:Sy------:Cy----Cy", 20000, "Next Level"}, // og22
        {"CcRcCcRc:RwCwRwCw:Sr--Sw--:CyCyCyCy", 20000, "Tunnel Tier II"}, // og24
        {"Rg--Rg--:CwRwCwRw:--Rg--Rg"         , 25000, "Double Painter"}, // og25
        {"CbCuCbCu:Sr------:--CrSrCr:CwCwCwCw", 25000, "Quad Cutter"}, // og26
        {"RuCw--Cw:----Ru--",                   50000, "Freeplay"} // level 20, og20
    };

    Hub();
    void Init();
    void Update();
    void Delete();
    std::shared_ptr<Text> levelTxt;
    std::shared_ptr<Text> levelNumTxt;
    std::shared_ptr<Text> progressTxt;
    std::shared_ptr<Text> targetTxt;
    std::shared_ptr<Text> deliverTxt;
    std::shared_ptr<Text> toUnlockTxt;
    std::shared_ptr<Text> lockedItemTxt;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_HUB_HPP
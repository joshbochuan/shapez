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
    std::shared_ptr<Item> targetItem;
    int targetAmount;
    std::vector<std::shared_ptr<ItemAcceptor>> m_Acceptors;

    // og are
    std::vector<std::tuple<std::string, int, std::string>> levelTargets = {
        {"CuCuCuCu",                            30, "Cutter and Trash"}, // og1
        {"----CuCu",                            40, "Upgrades"}, // og2
        {"RuRuRuRu",                            70, "Balancer"}, // og3
        {"RuRu----",                            125, "Rotator (CW)"}, // og4
        {"Cu----Cu",                            170, "Tunnel"}, // level 5, og5
        {"Cu------",                            270, "Painter"}, // og6
        {"CrCrCrCr",                            300, "Rotator (CCW)"}, // og7
        {"RbRbRbRb",                            480, "Color Mixer"}, // og8
        {"CpCpCpCp",                            600, "Compact Merger"}, // og9
        {"ScScScSc",                            800, "Stacker"}, // level 10, og10
        {"CgScScCg",                            1000, "Chaining Extractor"}, // bp, og11
        {"CbCbCbRb:CwCwCwCw",                   1000, "Blueprints"}, // og12
        {"SrSrSrSr:CyCyCyCy:SwSwSwSw",          2500, "Tunnel Tier II"}, // og16
        {"CbRbRbCb:CwCwCwCw:WbWbWbWb",          3800, "Belt Reader"}, // og17
        {"Sg----Sg:CgCgCgCg:--CyCy--",          6000, "Storage"}, // level 15, og18
        {"Cg----Cr:Cw----Cw:Sy------:Cy----Cy", 20000, "Quad Cutter"}, // og22
        {"CcRcCcRc:RwCwRwCw:Sr--Sw--:CyCyCyCy", 20000, "Double Painter"}, // og24
        {"Rg--Rg--:CwRwCwRw:--Rg--Rg"         , 25000, "Rotator (180)"}, // og25
        {"CbCuCbCu:Sr------:--CrSrCr:CwCwCwCw", 25000, "Compact Splitter"}, // og26
        {"RuCw--Cw:----Ru--",                   50000, "Freeplay"} // level 20, og20
    };

    Hub();
    void Init();
    void Update();
    void Delete();
    void LoadState();
    std::shared_ptr<Text> levelTxt;
    std::shared_ptr<Text> levelNumTxt;
    std::shared_ptr<Text> progressTxt;
    std::shared_ptr<Text> targetTxt;
    std::shared_ptr<Text> deliverTxt;
    std::shared_ptr<Text> toUnlockTxt;
    std::shared_ptr<Text> lockedItemTxt;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_HUB_HPP
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
#include "Util/SFX.hpp"

class Hub: public Machine {
public:
    std::shared_ptr<Util::SFX> levelUpSFX = nullptr;
    int level;
    std::shared_ptr<Item> targetItem;
    int targetAmount;
    int progress; // how much of the item is thrown into the hub already
    std::vector<std::shared_ptr<ItemAcceptor>> m_Acceptors;

    // og are
    std::vector<std::pair<std::string, int>> levelTargets = {
        {"CuCuCuCu",                            30}, // og1
        {"----CuCu",                            40}, // og2
        {"RuRuRuRu",                            70}, // og3
        {"RuRu----",                            125}, // og4
        {"Cu----Cu",                            170}, // level 5, og5
        {"Cu------",                            270}, // og6
        {"CrCrCrCr",                            300}, // og7
        {"RbRbRbRb",                            480}, // og8
        {"CpCpCpCp",                            600}, // og9
        {"ScScScSc",                            800}, // level 10, og10
        {"CbCbCbRb:CwCwCwCw",                   1000}, // bp, og12
        {"RpRpRpRp:CwCwCwCw",                   1000}, // og13
        {"SrSrSrSr:CyCyCyCy:SwSwSwSw",          2500}, // og16
        {"CbRbRbCb:CwCwCwCw:WbWbWbWb",          3800}, // og17
        {"Sg----Sg:CgCgCgCg:--CyCy--",          6000}, // level 15, og18
        {"Cg----Cr:Cw----Cw:Sy------:Cy----Cy", 20000}, // og22
        {"CcRcCcRc:RwCwRwCw:Sr--Sw--:CyCyCyCy", 20000}, // og24
        {"Rg--Rg--:CwRwCwRw:--Rg--Rg"         , 25000}, // og25
        {"CbCuCbCu:Sr------:--CrSrCr:CwCwCwCw", 25000}, // og26
        {"RuCw--Cw:----Ru--",                   50000} // level 20, og20
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
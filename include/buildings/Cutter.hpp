//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_CUTTER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_CUTTER_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"
#include "Util/Image.hpp"
#include "Opti/OptiImage.hpp"

class Cutter: public Machine {
private:
    float cooldown;
public:
    static inline std::shared_ptr<OptiImage> cutterTexture;
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejectorA;
    std::shared_ptr<ItemEjector> ejectorB;
    std::shared_ptr<Item> backupItem;
    bool pushA = false, pushB = false;
    Cutter(int x, int y, int r);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    void Init();
    void Update();
    void Delete();
    void Restore(int arg, std::shared_ptr<ItemEjector> from);
    void Promote();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_CUTTER_HPP
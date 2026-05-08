//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
#include "../items/Item.hpp"
#include "Util/GameObject.hpp"
#include "ItemAcceptor.hpp"
#include "Machine.hpp"
#include "../Opti/OptiObject.hpp"
#include "Machine.hpp"

class ItemAcceptor;
class Machine;

class ItemEjector: public OptiObject, public std::enable_shared_from_this<ItemEjector> {
private:
    bool initialized = false;
public:
    int x, y, r;
    float& rate;
    std::shared_ptr<Item> item, prep;
    float progress, prepProgress;
    std::shared_ptr<ItemAcceptor> next;
    std::shared_ptr<Machine> master;
    bool restored = false;
    std::vector<std::shared_ptr<OptiObject>> empty;
    ItemEjector(int x, int y, int r, std::shared_ptr<Machine> master);
    std::string getSaveString();
    void Init();
    void Update();
    void Delete();
    void Transfer();
    bool CheckConflict();
    void StartRestore();
    void Restore(int arg);
    void Promote();
    void SetItem(std::shared_ptr<Item> item);
    void RemoveItem();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEMEJECTOR_HPP
//
// Created by joshb on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ITEMACCEPTOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ITEMACCEPTOR_HPP
#include "../items/Item.hpp"
#include "Util/GameObject.hpp"
#include "ItemEjector.hpp"
#include "../Opti/OptiObject.hpp"

class ItemEjector;

class ItemAcceptor: public OptiObject, public std::enable_shared_from_this<ItemAcceptor> {
private:
    bool initialized = false;
public:
    int x, y, r;
    float& rate;
    std::shared_ptr<Item> item, prep;
    float progress, prepProgress;
    bool takesShape, takesColor;
    std::shared_ptr<ItemEjector> prev;
    bool restored = false;
    std::vector<std::shared_ptr<OptiObject>> empty;
    ItemAcceptor(int x, int y, int r);
    std::string getSaveString();
    void Init();
    void Update();
    void Delete();
    bool CheckConflict();
    void StartRestore();
    void Restore(int arg);
    void Promote();
    void SetItem(std::shared_ptr<Item> item);
    void RemoveItem();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ITEMACCEPTOR_HPP
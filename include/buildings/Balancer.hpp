//
// Created by joshb on 2026/3/22.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_BALANCER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_BALANCER_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"
#include "Util/Image.hpp"
#include "Opti/OptiImage.hpp"

class Balancer: public Machine {
private:
    int acceptPriority, ejectPriority;
public:
    static inline std::shared_ptr<OptiImage> balancerTexture;
    std::shared_ptr<ItemAcceptor> acceptorA, acceptorB;
    std::shared_ptr<ItemEjector> ejectorA, ejectorB;
    std::shared_ptr<Item> backupItemA, backupItemB;
    Balancer(int x, int y, int r);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    void Init();
    void Update();
    void Delete();
    void Restore(int arg);
    void Promote();
};

class Splitter: public Machine {
public:
    int ejectPriority = 0;
    bool mirrored = false;
    static inline std::vector<std::shared_ptr<OptiImage>> splitterTexture;
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejectorA, ejectorB;
    std::shared_ptr<Item> backupItem;
    Splitter(int x, int y, int r, bool mirrored);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    void Init();
    void Update();
    void Delete();
    void Restore(int arg);
    void Promote();
};

class Merger: public Machine {
public:
    int acceptPriority = 0;
    bool mirrored = false;
    static inline std::vector<std::shared_ptr<OptiImage>> mergerTexture;
    std::shared_ptr<ItemAcceptor> acceptorA, acceptorB;
    std::shared_ptr<ItemEjector> ejector;
    std::shared_ptr<Item> backupItemA, backupItemB;
    Merger(int x, int y, int r, bool mirrored);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    void Init();
    void Update();
    void Delete();
    void Restore(int arg);
    void Promote();
};



#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BALANCER_HPP
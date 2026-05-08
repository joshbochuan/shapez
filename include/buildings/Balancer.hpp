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
public:
    int acceptPriority, ejectPriority;
    static inline std::shared_ptr<OptiImage> balancerTexture;
    std::shared_ptr<ItemAcceptor> acceptorA, acceptorB;
    std::shared_ptr<ItemEjector> ejectorA, ejectorB;
    int transferStates = 0; // 4 bits: aA, aB, eA, eB
    Balancer(int x, int y, int r);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    void Init();
    void Update();
    void Delete();
    void Restore(int arg, std::shared_ptr<ItemEjector> from);
    void Promote();
};

class Splitter: public Machine {
public:
    int ejectPriority = 0;
    int lastEjectPriority = 0;
    bool mirrored = false;
    static inline std::vector<std::shared_ptr<OptiImage>> splitterTexture;
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejectorA, ejectorB;
    Splitter(int x, int y, int r, bool mirrored);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    void Init();
    void Update();
    void Delete();
    void Restore(int arg, std::shared_ptr<ItemEjector> from);
    void Promote();
};

class Merger: public Machine {
public:
    int acceptPriority = 0;
    int lastAcceptPriority = 0;
    bool mirrored = false;
    static inline std::vector<std::shared_ptr<OptiImage>> mergerTexture;
    std::shared_ptr<ItemAcceptor> acceptorA, acceptorB;
    std::shared_ptr<ItemEjector> ejector;
    Merger(int x, int y, int r, bool mirrored);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    void Init();
    void Update();
    void Delete();
    void Restore(int arg, std::shared_ptr<ItemEjector> from);
    void Promote();
};



#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BALANCER_HPP
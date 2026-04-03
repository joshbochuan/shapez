//
// Created by joshb on 2026/3/25.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_STACKER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_STACKER_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"
#include "Util/Image.hpp"
#include "Opti/OptiImage.hpp"

class Stacker: public Machine {
private:
    float cooldown = 0;
public:
    static inline std::shared_ptr<OptiImage> stackerTexture;
    std::shared_ptr<ItemAcceptor> acceptorA, acceptorB;
    std::shared_ptr<ItemEjector> ejector;
    Stacker(int x, int y, int r);
    void Init();
    void Update();
    void Delete();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_STACKER_HPP
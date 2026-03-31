//
// Created by joshb on 2026/3/25.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_MIXER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_MIXER_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"
#include "Util/Image.hpp"

inline std::shared_ptr<Util::Image> mixerTexture;

class Mixer: public Machine {
private:
    float cooldown = 0;
public:
    std::shared_ptr<ItemAcceptor> acceptorA, acceptorB;
    std::shared_ptr<ItemEjector> ejector;
    Mixer(int x, int y, int r);
    void Init();
    void Update();
    void Delete();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_MIXER_HPP
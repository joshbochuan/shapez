//
// Created by joshb on 2026/3/25.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_MIXER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_MIXER_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"
#include "Util/Image.hpp"
#include "Opti/OptiImage.hpp"

class Mixer: public Machine {
private:
    float cooldown = 0;
public:
    static inline std::shared_ptr<OptiImage> mixerTexture;
    std::shared_ptr<ItemAcceptor> acceptorA, acceptorB;
    std::shared_ptr<ItemEjector> ejector;
    std::shared_ptr<Item> backupItemA, backupItemB;
    Mixer(int x, int y, int r);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    void Init();
    void Update();
    void Delete();
    void Restore(int arg, std::shared_ptr<ItemEjector> from);
    void Promote();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_MIXER_HPP
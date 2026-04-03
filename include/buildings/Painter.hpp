//
// Created by joshb on 2026/3/25.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_PAINTER_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_PAINTER_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"
#include "Util/Image.hpp"
#include "Opti/OptiImage.hpp"

class Painter: public Machine {
private:
    float cooldown;
public:
    static inline std::vector<std::shared_ptr<OptiImage>> painterTextures;
    std::shared_ptr<ItemAcceptor> acceptorA, acceptorB;
    std::shared_ptr<ItemEjector> ejector;
    bool mirrored;
    Painter(int x, int y, int r, bool mirrored);
    void Init();
    void Update();
    void Delete();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_PAINTER_HPP
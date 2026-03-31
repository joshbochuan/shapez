//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_ROTATOR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_ROTATOR_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"
#include "Util/Image.hpp"

enum class RotatorType {
    ROTATE_CW,
    ROTATE_CCW,
    ROTATE_180
};

class Rotator: public Machine {
private:
    RotatorType type;
    float cooldown;
public:
    static inline std::shared_ptr<Util::Image> rotatorCWTexture;
    static inline std::shared_ptr<Util::Image> rotator180Texture;
    static inline std::shared_ptr<Util::Image> rotatorCCWTexture;
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejector;
    Rotator(int x, int y, int r, RotatorType type);
    void Init();
    void Update();
    void Delete();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_ROTATOR_HPP
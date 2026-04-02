//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
#include "Machine.hpp"
#include "../items/Item.hpp"
#include "Util/Animation.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"
#include "Util/Animation.hpp"
#include <vector>

enum class BeltType {
    LEFT,
    RIGHT,
    FORWARD
};

class Belt: public Machine {
public:
    static inline std::vector<std::shared_ptr<Util::Image>> beltForwardTexture;
    static inline std::vector<std::shared_ptr<Util::Image>> beltLeftTexture;
    static inline std::vector<std::shared_ptr<Util::Image>> beltRightTexture;
    static inline std::vector<std::shared_ptr<Util::Image>> beltInTexture; // belt in
    static inline std::vector<std::shared_ptr<Util::Image>> beltOutTexture;
    Belt(int x, int y, int r, BeltType type);
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejector;
    BeltType type;
    void Init();
    void Update();
    void Delete();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
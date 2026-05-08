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
#include "Opti/OptiImage.hpp"

enum class BeltType {
    LEFT,
    RIGHT,
    FORWARD
};

class Belt: public Machine {
public:
    static inline std::vector<std::shared_ptr<OptiImage>> beltForwardTexture;
    static inline std::vector<std::shared_ptr<OptiImage>> beltLeftTexture;
    static inline std::vector<std::shared_ptr<OptiImage>> beltRightTexture;
    static inline std::vector<std::shared_ptr<OptiImage>> beltInTexture; // belt in
    static inline std::vector<std::shared_ptr<OptiImage>> beltOutTexture;
    Belt(int x, int y, int r, BeltType type);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejector;
    BeltType type;
    void Init();
    void Update();
    void Delete();
    void Restore(int arg, std::shared_ptr<ItemEjector> from);
    void Promote();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BELT_HPP
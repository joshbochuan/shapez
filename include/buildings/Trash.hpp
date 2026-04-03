//
// Created by joshb on 2026/3/21.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_TRASH_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_TRASH_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "Util/Image.hpp"
#include "Opti/OptiImage.hpp"

class Trash: public Machine {
public:
    static inline std::shared_ptr<OptiImage> trashTexture;
    std::vector<std::shared_ptr<ItemAcceptor>> m_Acceptors;
    Trash(int x, int y);
    void Init();
    void Update();
    void Delete();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_TRASH_HPP
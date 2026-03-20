//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_BELTLEFT_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_BELTLEFT_HPP
#include "Belt.hpp"
#include "Util/Animation.hpp"

class BeltLeft: public Belt {
private:
    std::shared_ptr<Util::Animation> m_Animation;
public:
    BeltLeft(int x, int y, int r);
};


#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BELTLEFT_HPP
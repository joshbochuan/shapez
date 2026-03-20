//
// Created by User on 2026/3/20.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_BELTFORWARD_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_BELTFORWARD_HPP
#include "Belt.hpp"
#include "Util/Animation.hpp"

class BeltForward: public Belt {
private:
    std::shared_ptr<Util::Animation> m_Animation;
public:
    BeltForward(int x, int y, int r, float rate);
    void Update();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BELTFORWARD_HPP
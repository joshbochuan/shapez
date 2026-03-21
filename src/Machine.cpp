//
// Created by User on 2026/3/20.
//

#include "Machine.hpp"

Machine::Machine(int x, int y, int r, float& rate) : rate(rate) {
    this->x = x;
    this->y = y;
    this->r = r;
    this->rate = rate;
}

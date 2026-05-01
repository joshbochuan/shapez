//
// Created by joshb on 2026/4/30.
//

#ifndef SHAPEZ_THROUGHPUTCOUNTER_HPP
#define SHAPEZ_THROUGHPUTCOUNTER_HPP
#include <vector>

class ThroughputCounter {
    std::vector<int> buffer;
    int bufferVal = 0;
public:
    ThroughputCounter(float seconds);
    void Reset();
    void Push(int val);
    float throughput=0;
};

#endif //SHAPEZ_THROUGHPUTCOUNTER_HPP
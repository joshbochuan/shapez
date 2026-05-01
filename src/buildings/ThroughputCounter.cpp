//
// Created by joshb on 2026/4/30.
//
#include "buildings/ThroughputCounter.hpp"
#include "config.hpp"

ThroughputCounter::ThroughputCounter(float seconds) {
    throughput = 0;
    bufferVal = 0;
    for (int i=0; i<static_cast<int>(seconds * FPS_CAP); i++) {buffer.push_back(0);}
}

void ThroughputCounter::Push(int val) {
    bufferVal -= buffer.front();
    buffer.erase(buffer.begin());
    buffer.push_back(val);
    bufferVal += val;
    throughput = static_cast<float>(bufferVal * FPS_CAP)/static_cast<float>(buffer.size());
}

void ThroughputCounter::Reset() {
    for (int & i : buffer) {i = 0;}
    bufferVal = 0;
    throughput = 0;
}

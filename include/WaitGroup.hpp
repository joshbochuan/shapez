//
// Created by User on 2026/4/10.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_WAITGROUP_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_WAITGROUP_HPP

#pragma once
#include <mutex>
#include <condition_variable>

class WaitGroup {
public:
    void Add(int n) {
        std::lock_guard<std::mutex> lock(m);
        count += n;
    }

    void Done() {
        std::lock_guard<std::mutex> lock(m);
        if (--count == 0) {
            cv.notify_all();
        }
    }

    void Wait() {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&]() { return count == 0; });
    }

private:
    std::mutex m;
    std::condition_variable cv;
    int count = 0;
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_WAITGROUP_HPP

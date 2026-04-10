//
// Created by User on 2026/4/10.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_THREADPOOL_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_THREADPOOL_HPP

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        this->numThreads = numThreads;
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this]() {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty())
                            return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }

        condition.notify_all();

        for (std::thread &worker : workers)
            worker.join();
    }

    void Enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::move(task));
        }
        condition.notify_one();
    }

    int size() {return numThreads;}

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    int numThreads = 8;
    bool stop;
};

inline ThreadPool pool(std::min(static_cast<int>(std::thread::hardware_concurrency()), 12));

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_THREADPOOL_HPP

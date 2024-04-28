#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <queue>
#include <utility>
#include <chrono>
#include <iostream>
#include <atomic>

using std::thread;
using std::condition_variable;
using std::mutex;
using std::unique_lock;
using std::vector;
using std::queue;
using std::pair;
using std::cout;
using std::endl;
using std::atomic;

class thread_pool
{
        vector<thread> threads;
        condition_variable new_task_signal;
        mutex new_task_mutex;
        atomic<bool> stop_flag;

        int n;

        queue<pair<void (*)(void*), void*>> tasks;

        void worker_thread_func(int thread_id)
        {
                while (true)
                {
                        // Wait for a new task
                        unique_lock<mutex> lk(new_task_mutex);

                        if (this->tasks.empty() && !stop_flag)
                        {
                                cout << "Worker thread: " << thread_id << " - waiting for a new task..." << endl;
                                new_task_signal.wait(lk, [this]{
                                                cout << "In wait lambda: stop flag = " << stop_flag << endl;
                                                return !this->tasks.empty() || this->stop_flag;
                                                });
                        }

                        // Execute the task
                        if (!this->tasks.empty())
                        {
                                auto task = tasks.front();
                                tasks.pop();
                                lk.unlock();
                                cout << "Worker thread: " << thread_id << " - executing task..." << endl;
                                task.first(task.second);
                        }
                        else if (this->stop_flag)
                        {
                                cout << "Worker thread: " << thread_id << " - exiting..." << endl;
                                        return;
                        }

                }
        }

public:
                thread_pool(int n) : n(n), stop_flag(false) {}

                void start()
                {
                        for (int i = 0; i < n; ++i)
                        {
                                threads.emplace_back(&thread_pool::worker_thread_func, this, i);
                        }
                }

                void stop()
                {
                        stop_flag = true;
                        new_task_signal.notify_all();
                        for (auto& worker_thread : threads)
                        {
                                worker_thread.join();
                        }
                }

                void add_task(void (*f)(void*), void* arg)
                {
                        unique_lock<mutex> lk(new_task_mutex);
                        tasks.emplace(f, arg);
                        new_task_signal.notify_one();
                }
};


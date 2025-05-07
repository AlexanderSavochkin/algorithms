#include "../include/producer_consumer.hpp"
#include <thread>
#include <iostream>
#include <functional>
#include <vector>
#include <random>

using std::thread;
using std::ref;
using std::vector;

const size_t NUM_PRODUCERS = 3;
const size_t NUM_CONSUMERS = 11;

mutex m;

int get_random_time()
{
    static thread_local std::mt19937 gen(std::random_device{}());
    static thread_local std::uniform_int_distribution<> dist(500, 1999);
    return dist(gen);
}

void producer_thread_func(int thread_id, ProducerConsumer<int>& pc)
{
    for (int i = 0; i < 10; ++i)
    {
        int wait_time = get_random_time();
        std::cout << "Thread " << thread_id << ": random back off time = " << wait_time << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        pc.emplace_item(100 * (thread_id + 1) + i);
    }
}

void consumer_thread_func(int thread_id, ProducerConsumer<int>& pc)
{
    try {
        for (;;)
        {
            int item = pc.get_item();
            std::cout << "Worker " << thread_id << " processing item: " << item << std::endl;
        }
    } catch (const std::runtime_error& e) {
        std::cout << "Worker " << thread_id << " shutting down.\n";
    }
}

int main()
{   
    ProducerConsumer<int> pc(5);
    vector<thread> ts;

    for (int i = 0; i < NUM_PRODUCERS; ++i)
        ts.push_back(thread(producer_thread_func, i, ref(pc)));

    for (int i = 0; i < NUM_CONSUMERS; ++i)
        ts.push_back(thread(consumer_thread_func, i, ref(pc)));


    //Wait for producers to finish work
    for (int i = 0; i < NUM_PRODUCERS; ++i)
        ts[i].join();

    pc.set_shutdown();
    std::cout << "Stopped producing..." << std::endl;

    for (int i = NUM_PRODUCERS; i < NUM_PRODUCERS + NUM_CONSUMERS; ++i)
        ts[i].join(); // Wait for consumers

    std::cout << "Bye!" << std::endl;

    return 0;
}
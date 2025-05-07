#pragma once

#include <queue>
#include <condition_variable>
#include <mutex>
#include <utility>
#include <atomic>


using std::queue;
using std::condition_variable;
using std::unique_lock;
using std::mutex;
using std::move;
using std::forward;
using std::atomic;


template<typename PayloadT>
class ProducerConsumer
{
    size_t queue_size;
    queue<PayloadT> q;
    mutex m;
    condition_variable producer_cv;
    condition_variable consumer_cv;

    std::atomic<bool> shutdown { false };
 
public:
    explicit ProducerConsumer(size_t queue_size) : queue_size(queue_size)
    {
    }

    void set_shutdown()
    {
        shutdown = true;
        consumer_cv.notify_all();
    }

    //Api for producer. 
    void add_item(PayloadT payload)
    {
        //
        unique_lock<mutex> lk(m);
        producer_cv.wait(lk, [this]{return q.size() < queue_size;} );
        q.push(payload);
        consumer_cv.notify_one();
    }

    // Rvalue reference version - move the payload
    void add_item(PayloadT&& payload)
    {
        unique_lock<std::mutex> lk(m);
        producer_cv.wait(lk, [this]{return q.size() < queue_size;});
        q.push(move(payload));  // Move the payload into the queue
        consumer_cv.notify_one();
    }

    // Perfect forwarding version - handles both lvalue and rvalue references
    template<typename T>
    void emplace_item(T&& payload)
    {
        unique_lock<std::mutex> lk(m);
        producer_cv.wait(lk, [this]{return q.size() < queue_size;});
        q.emplace(forward<T>(payload));  // Forward the argument
        consumer_cv.notify_one();
    }

    //Api for consumer 
    PayloadT get_item()
    {
        //
        unique_lock<mutex> lk(m);
        consumer_cv.wait(lk, [this]{return !q.empty() || shutdown;} );

        if (q.empty() && shutdown)
            throw std::runtime_error("Shutdown with empty queue");

        PayloadT result = q.front();
        q.pop();
        producer_cv.notify_one();
        return result;
    }
};
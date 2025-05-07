#pragma once

#include <condition_variable>
#include <mutex>

using std::mutex;
using std::condition_variable;
using std::unique_lock;

class ReadWriteLockCondVar {

  unsigned num_active_readers { 0 };
  bool active_writer { false };
  unsigned waiting_writers { 0 };

  mutex m;
  condition_variable cv;


public:
  void read_lock() {
    unique_lock<mutex> lk(m);
    cv.wait(
        lk, 
        [this] {
            return !active_writer && waiting_writers == 0;
        }
    );
    ++num_active_readers;
  }

  void read_unlock() {
    unique_lock<mutex> lk(m);
    --num_active_readers;
    if (num_active_readers == 0 && waiting_writers > 0)
      cv.notify_all();
  }

  void write_lock() {
    unique_lock<mutex> lk(m);
    ++waiting_writer;

    cv.wait(
        lk, 
        [this] {
            return !active_writer && (num_active_readers == 0);
        }
    );

    --waiting_writer;

    active_writer = true;
  }

  void write_unlock() {
    unique_lock<mutex> lk(m);
    active_writer = false;
    cv.notify_all();
  }

  explicit ReadWriteLockCondVar() {}
};

#pragma once

#include <mutex>
#include <semaphore>

using std::mutex;
using std::unique_lock;

class ReadWriteLockTwoMutexes {
  mutex r_mutex;
  std::binary_semaphore w_semaphore{ 1 };

  uintmax_t readers = {0};

public:
  void read_lock() {
    std::unique_lock<std::mutex> lk(r_mutex);
    ++readers;
    if (readers == 1) {
        w_semaphore.acquire();
    }
  }

  void write_lock() {
      w_semaphore.acquire();
  }

  void write_unlock() {
      w_semaphore.release();
  }

  void read_unlock() {
    std::unique_lock<std::mutex> lk(r_mutex);
    --readers;
    if (readers == 0) {
        w_semaphore.release();
    }
  }

  explicit ReadWriteLockTwoMutexes() {}
};


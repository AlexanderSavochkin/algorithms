#include <cstdlib>
#include <vector>
#include <thread>
#include <chrono>
#include <unordered_set>
#include <iostream>
#include <functional>


#include "../../../../test/catch_amalgamated.hpp"
#include "../include/read_write_lock_two_mutexes.hpp"

using std::rand;
using std::vector;
using std::thread;
using std::clog;
using std::endl;
using std::ref;

enum class EventType 
{
    Read_Trying_Enter,
    Read_Entered,
    Read_Exiting,
    Write_Trying_Enter,
    Write_Entered,
    Write_Exiting
};

struct EventInfo
{
    EventType type;
	int thread_id;
};

std::mutex m, log_m;
std::condition_variable readers_acquired_the_lock;
std::condition_variable writer_acquired_the_lock;
ReadWriteLockTwoMutexes rw_lock;

void add_log(vector<EventInfo>& log, EventInfo evinfo)
{
	std::lock_guard<std::mutex> lk(log_m);
	log.push_back(evinfo);
}

void reader_thread_func(
    int thread_id,
    ReadWriteLockTwoMutexes& rw_lock,
    vector<EventInfo>& log)
{
    add_log(log, { EventType::Read_Trying_Enter, thread_id });
    rw_lock.read_lock();
    add_log(log, {EventType::Read_Entered, thread_id} );

    std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 1000));

    readers_acquired_the_lock.notify_all();

    add_log(log, {EventType::Read_Exiting, thread_id});
    rw_lock.read_unlock();
}

void writers_thread_func(
    int thread_id,
    ReadWriteLockTwoMutexes& rw_lock,
    vector<EventInfo>& log
)
{
    add_log(log, {EventType::Write_Trying_Enter, thread_id});
    rw_lock.write_lock();
    add_log(log, {EventType::Write_Entered, thread_id});

    std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 1000));

    writer_acquired_the_lock.notify_all();

    add_log(log, {EventType::Write_Exiting, thread_id});
    rw_lock.write_unlock();
}

bool check(int num_iter, int readers_size)
{
    vector<EventInfo> log;
    for (int iter = 0; iter < num_iter; ++iter)
    {
        vector<thread> allthreads;

        for (int i = 0; i < readers_size / 2; ++i)
        {
            thread rthr(reader_thread_func, i, ref(rw_lock), ref(log));
            allthreads.push_back(move(rthr));
        }

        {
            unique_lock<mutex> lk(m);
            readers_acquired_the_lock.wait(lk); //TODO: Deal with spurious wakeups
        }

        thread wthr(writers_thread_func, 0, ref(rw_lock), ref(log));
        allthreads.push_back(move(wthr));

        {
            unique_lock<mutex> lk(m);
            writer_acquired_the_lock.wait(lk); //TODO: Deal with spurious wakeups
        }

        for (int i = readers_size / 2; i < readers_size; ++i)
        {
			thread rthr(reader_thread_func, i, ref(rw_lock), ref(log));
			allthreads.push_back(move(rthr));
		}

        for (auto&& t : allthreads)
        {
            t.join();
        }

        //Check that we didn't have interleaved reads and writes, all the reads from the first 
        //batch must be done before the write and all the reads from the second branch must go after the write
        bool correct = true;
        std::unordered_set<int> first_batch_readers;
        auto it = log.begin();
        for (; it != log.end() && it->type != EventType::Write_Entered; ++it) {
            if (it->type == EventType::Read_Exiting) {
                if (it->thread_id > readers_size / 2) 
                {
                    clog << "Reader from the 2 batch read before the writer" << endl;
                    correct = false;
                }
				first_batch_readers.insert(it->thread_id);
			}
        }

        //Check that exactly half of the readers from the first batch read before the writer
        if (first_batch_readers.size() != readers_size / 2) 
        {
            clog << "Not all readers from the first batch read before the writer" << endl;
            correct = false;
        }

        //Check that we didn't reach end of the log and that the next event is write exiting
        if (it == log.end() || (++it)->type != EventType::Write_Exiting) {
			clog << "Write exiting event expected after write entering" << endl;
			correct = false;
		}

        //Check that all the readers from the second batch read after the writer
        for (; it != log.end(); ++it)
        {
            if (it->type == EventType::Read_Exiting)
            {
                first_batch_readers.insert(it->thread_id);
            }
        }
        
        if (first_batch_readers.size() != readers_size / 2)
        {
            clog << "Not all readers worked" << endl;
            correct = false;
        }

        return correct;
    }
}

TEST_CASE( "Test bit indexted tree", "[bit-test]" ) {
    REQUIRE(check(5, 10));
}

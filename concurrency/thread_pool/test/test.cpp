int main()
{
        thread_pool pool(2);

        cout << "Starting thread pool..." << endl;

        pool.start();
        std::this_thread::sleep_for (std::chrono::seconds(5));


        cout << "Adding tasks..." << endl;

        pool.add_task([](void* arg) {
                cout << "Task 1 rinning on thread pool for 3 sec:" << endl;
                std::this_thread::sleep_for (std::chrono::seconds(3));
        }, nullptr);

        pool.add_task([](void* arg) {
                cout << "Task 2 rinning on thread pool for 4 sec:" << endl;
                std::this_thread::sleep_for (std::chrono::seconds(4));
        }, nullptr);
        pool.add_task([](void* arg) {
                cout << "Task 3 rinning on thread pool for 1 sec:" << endl;
                std::this_thread::sleep_for (std::chrono::seconds(1));
        }, nullptr);

        pool.stop();

        cout << "Bye" << endl;

        return 0;
}
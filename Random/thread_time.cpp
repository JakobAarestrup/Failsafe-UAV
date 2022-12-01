#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

/* void print(int n, const std::string &str)
{
    std::string msg = std::to_string(n) + " : " + str;
    std::cout << msg << std::endl;
}

int main()
{
    std::vector<std::string> s = {
        "Educative.blog",
        "Educative",
        "courses",
        "are great"};
    std::vector<std::thread> threads;

    for (int i = 0; i < s.size(); i++)
    {
        threads.push_back(std::thread(print, i, s[i]));
    }

    for (auto &th : threads)
    {
        th.join();
    }
    return 0;
}
 */
void run()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "good morning" << std::endl;
}

void run2()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Gamer time" << std::endl;
}

int main()
{
    std::vector<std::thread> threads;
    auto exec_IMU = []()
    { while (true) run(); };
    auto exec_loop = []()
    { while (true) run2(); };

    threads.push_back(std::thread(exec_imu));
    threads.push_back(std::thread(exec_imu2));

    for (auto &th : threads)
    {
        th.join();
    }
    /*  std::thread(exec_imu);
     t.join();
     std::thread t2(exec_imu);
     t2.join();
  */
    return 0;
}

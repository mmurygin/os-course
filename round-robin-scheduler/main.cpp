#include <iostream>
#include "scheduler.hpp"

using namespace std;

int main()
{
    scheduler_setup(3);

    new_thread(1);
    new_thread(2);
    new_thread(3);

    cout << "current thread: " << current_thread() << endl;

    for (int i = 0; i < 10; i++)
    {
        timer_tick();
        cout << "current thread: " << current_thread() << endl;
    }
    int block = current_thread();
    cout << "block thread: " << block << endl;
    block_thread();
    cout << "current thread: " << current_thread() << endl;
    for (int i = 0; i < 4; i++)
    {
        timer_tick();
        cout << "current thread: " << current_thread() << endl;
    }
    wake_thread(block);
    cout << "wake thread: " << block << endl;
    cout << "current thread: " << current_thread() << endl;
    for (int i = 0; i < 4; i++)
    {
        timer_tick();
        cout << "current thread: " << current_thread() << endl;
    }
    cout << "exit thread: " << current_thread() << endl;
    exit_thread();
    cout << "current thread: " << current_thread() << endl;
    for (int i = 0; i < 4; i++)
    {
        timer_tick();
        cout << "current thread: " << current_thread() << endl;
    }
    cout << "exit thread: " << current_thread() << endl;
    exit_thread();
    cout << "current thread: " << current_thread() << endl;
    for (int i = 0; i < 4; i++)
    {
        timer_tick();
        cout << "current thread: " << current_thread() << endl;
    }
    cout << "exit thread: " << current_thread() << endl;
    exit_thread();
    cout << "current thread: " << current_thread() << endl;
}

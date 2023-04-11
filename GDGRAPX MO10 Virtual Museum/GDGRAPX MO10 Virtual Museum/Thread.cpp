#include "Thread.h"

void Thread::start()
{
	std::thread(&Thread::run, this).detach();
}

void Thread::sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
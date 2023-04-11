#pragma once
#include <thread>

class Thread
{
public:
	Thread() {}
	~Thread() {}

	void start();
	static void sleep(int ms);

protected:
	virtual void run() = 0;
};
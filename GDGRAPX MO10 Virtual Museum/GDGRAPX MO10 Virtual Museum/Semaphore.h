#pragma once
#include<semaphore>

class Semaphore
{
public:
	Semaphore(int available);
	~Semaphore();

	void acquire();
	void acquire(int permitCount);
	void release();
	void release(int permitCount);

private:
	const static int GLOBAL_MAX_PERMITS = 100;
	std::counting_semaphore<GLOBAL_MAX_PERMITS>* semaphore;
};
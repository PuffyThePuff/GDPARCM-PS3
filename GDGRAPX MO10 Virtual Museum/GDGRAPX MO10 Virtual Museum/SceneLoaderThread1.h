#pragma once
#include"Thread.h"
#include"ModelContainer.h"
#include<vector>
#include"Semaphore.h"
#include<GL/glew.h>
// #include"LoaderThreadListener.h"

class SceneLoaderThread1 : public Thread
{
public:
	SceneLoaderThread1(Semaphore* shared_mutex /*, LoaderThreadListener* thread_listener*/);

	float getFinishedModelCount() { return (float)finishedModelCount; }
	float getTotalModelCount() { return (float)totalModelCount; }

private:
	void run() override;

	Semaphore* mutex;
	// LoaderThreadListener* listener;

	int finishedModelCount = 0;
	int totalModelCount = 3;
};
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
	SceneLoaderThread1(
		std::vector<ModelContainer*> active_models,
		Semaphore* shared_mutex /*,
		LoaderThreadListener* thread_listener*/
	);

	float getFinishedModelCount() { return (float)finishedModelCount; }
	float getTotalModelCount() { return (float)totalModelCount; }
	bool finished() { return isFinished; }

private:
	void run() override;

	std::vector<ModelContainer*> activeModels;
	Semaphore* mutex;
	// LoaderThreadListener* listener;

	int finishedModelCount;
	int totalModelCount;
	bool isFinished = false;
};
#pragma once
#include"Thread.h"
#include"ModelContainer.h"
#include<vector>
#include"Semaphore.h"
#include<GL/glew.h>

class SceneLoaderThread1 : public Thread
{
public:
	SceneLoaderThread1(std::vector<ModelContainer*> active_models, Semaphore* shared_mutex);

private:
	void run() override;

	std::vector<ModelContainer*> activeModels;
	Semaphore* mutex;
};
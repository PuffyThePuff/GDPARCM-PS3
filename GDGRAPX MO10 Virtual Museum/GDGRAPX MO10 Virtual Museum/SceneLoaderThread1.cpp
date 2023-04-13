#include "SceneLoaderThread1.h"
#include"obj_mesh.h"

SceneLoaderThread1::SceneLoaderThread1(
	std::vector<ModelContainer*> active_models,
	Semaphore* shared_mutex /*,
	LoaderThreadListener* thread_listener*/
)
{
	activeModels = active_models;
	mutex = shared_mutex;
	// listener = thread_listener;
}

void SceneLoaderThread1::run() {
	ModelContainer* meteor = new ModelContainer(0.f, 0.f, 0.f);
	LoadObjFile(meteor->objData, "Meteor/Meteor.obj");
	LoadObjToMemory(meteor->objData, 1.f, meteor->offsets);
	/*
	mutex->acquire();
	listener->onLoadFinish(1, meteor);
	mutex->release();
	*/
}
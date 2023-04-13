#pragma once
#include"ModelContainer.h"
#include<vector>
#include<string>
#include"Semaphore.h"
#include"Thread.h"
#include"SceneLoaderThread1.h"

class SceneHandler : public Thread // , public ButtonListener, public LoaderThreadListener
{
public:
	SceneHandler(std::vector<ModelContainer*>* active_models, int* current_scene);

	void onButtonRelease(std::string button_name) /* override */;
	void onLoadFinish(int scene_id, ModelContainer* model_to_add) /* override */;

	float getProgress(int scene_id);
	
	Semaphore* barrier;
	Semaphore* mutex;

private:
	void run() override;

	void selectScene(int scene_id);
	void loadScene(int scene_id);
	void unloadScene(int scene_id);
	void loadAllScenes();
	void unloadAllScenes();

	void addToScene(int scene_id, ModelContainer* model_to_add);

	std::vector<ModelContainer*>* activeModels;
	int* currentScene;
	bool currentSceneIsLoading = true;

	std::vector<ModelContainer*> scene1Models;
	std::vector<ModelContainer*> scene2Models;
	std::vector<ModelContainer*> scene3Models;
	std::vector<ModelContainer*> scene4Models;
	std::vector<ModelContainer*> scene5Models;

	SceneLoaderThread1* loader1;
	/*
	SceneLoaderThread2* loader2;
	...
	*/
};
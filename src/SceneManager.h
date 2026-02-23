#pragma once

#include "Module.h"
#include "Scene.h"
#include <list>

class SceneManager : public Module
{
public:

	SceneManager();

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void LoadScenes();

	void SetCurrentScene(std::string sceneID);

public:

	std::list<std::shared_ptr<Scene>> scenes;
	std::shared_ptr<Scene> currentScene = nullptr;
	bool paused = false;

};

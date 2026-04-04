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

	bool PreUpdate();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void LoadScenes();

	void SetCurrentScene(std::string sceneID, std::string spawnID = "default");

	EntityManager* GetEntityManager() const;
	
	MissionManager* GetMissionManager() const;

	DialogManager* GetDialogManager() const;

public:

	std::list<Scene*> scenes;
	Scene* currentScene = nullptr;
	bool paused = false;
	std::string mapsPath = "Assets/Maps/";

};

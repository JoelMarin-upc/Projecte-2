#pragma once

#include "Module.h"
#include "Scene.h"
#include <list>

struct SceneInfo {
	std::string id;
	std::string mapPath;
	std::string mapName;
};

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

	void DoTransition();

	EntityManager* GetEntityManager() const;
	
	MissionManager* GetMissionManager() const;

	DialogManager* GetDialogManager() const;

public:

	std::list<SceneInfo> sceneInfos;
	Scene* currentScene = nullptr;
	bool paused = false;
	std::string mapsPath = "Assets/Maps/";
	std::string pendingSceneID = "";
	std::string pendingSpawnId = "";
	bool hasQueuedTransition = false;

	bool triggerFirstMonologue = true;
};

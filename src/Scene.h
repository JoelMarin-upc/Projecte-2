#pragma once

#include "Module.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Timer.h"
#include "XMLHandler.h"
#include "UIManager.h"
#include "EntityManager.h"
#include "MissionManager.h"
#include "DialogManager.h"
#include "InteractableItem.h"
#include "MenuManager.h"

struct SDL_Texture;

struct SceneData {
	std::string mapPath;
	std::string mapName;
	std::string musicPath;
};

class Scene : public Module
{
public:

	Scene(std::string _id, std::string mapPath, std::string mapName);
	
	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void TogglePause();
	void SaveGame();
	void LoadGame();
	void LoadMap(std::string mapPath, std::string mapName);
	void LoadScene();
	void EndScene();

	void StartDialog(std::string characterId);
	void EndDialog();

	Vector2D GetPlayerPosition();

	bool OnUIMouseClickEvent(UIElement* uiElement);

	bool hasEnded;

	std::string id;
	std::string name;

	EntityManager* entityManager;
	MissionManager* missionManager;
	DialogManager* dialogManager;

private:
	
	Map* map;

	std::shared_ptr<InteractableItem> testItem;
	std::shared_ptr<Player> player;
	SceneData data;
	XMLHandler* persistance = new XMLHandler();
	Timer gameTimer;
	bool gameStarted = false;
	bool paused = false;
	bool isOnDialog = false;
	GameData mapData;
};
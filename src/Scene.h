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
#include "Combat.h"
#include "EnemyParty.h"

struct SDL_Texture;

struct SceneData {
	std::string mapPath;
	std::string mapName;
	std::string musicPath;
};

class Scene : public Module
{
public:

	Scene(std::string _id, std::string mapPath, std::string mapName, std::string combatMapName = "");
	
	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start(std::string spawnId = "default");

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
	void LoadScene(std::string spawnId = "default");
	void EndScene();
	void EndGame();
	void CheckTimers();

	void CheckTransitions();

	void StartDialog(std::string characterId);
	void EndDialog();

	void StartCombat(std::shared_ptr<Enemy> enemy);
	void EndCombat(EnemyParty* enemyParty, CombatResult combatResult);

	Vector2D GetPlayerPosition();

	bool OnUIMouseClickEvent(UIElement* uiElement);

	bool GetIsOnDialog() {
		return isOnDialog;
	}

	bool hasEnded;

	std::string id;
	std::string name;
	std::string pendingSpawnId = "default";

	EntityManager* entityManager;
	MissionManager* missionManager;
	DialogManager* dialogManager;

private:
	
	Map* map;
	Map* combatMap;
	std::string mapsPath;
	std::string mapName;
	std::string combatMapName;

	std::shared_ptr<InteractableItem> testItem;
	std::shared_ptr<Player> player;
	SceneData data;
	XMLHandler* persistance = new XMLHandler();
	Timer gameTimer;
	bool gameStarted = false;
	bool paused = false;
	bool isOnDialog = false;

	float combatCooldownSeconds = 5.f;
	bool hasCombatCooldown = false;
	Timer combatTimer;

	Combat* combat = nullptr;

	std::string  activeDialogId = "";

	GameData mapData;
	
	int sw;
	int sh;
	SDL_Texture* logo;
	SDL_Rect b_logo;
	std::shared_ptr<UIImage> studioLogo;
	int hoverFxId;
	int clickFxId;
	int logoFxId;
	int elevatorFxId;

};
#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Timer.h"
#include "XMLHandler.h"
#include "UIManager.h"
#include "EntityManager.h"
#include "MissionManager.h"
#include "DialogManager.h"

struct SDL_Texture;

enum Menu {
	NONE,
	MAIN,
	PAUSE,
	SETTINGS,
	CHECKPOINTS,
	DEATHSCREEN,
	CREDITS
};

enum UIID {
	START_GAME,
	PAUSED_LBL,
	SETTINGS_LBL,
	COUNTINUE_GAME,
	RESUME,
	SETTINGS_MENU,
	CREDITS_BTN,
	CREDITS_LBL,
	MUSIC_VOLUME_LABEL,
	MUSIC_VOLUME,
	FX_VOLUME_LABEL,
	FX_VOLUME,
	FULLSCREEN_LABEL,
	FULLSCREEN,
	BACK_MENU,
	BACK_MAIN_MENU,
	EXIT,
	UI_COINS,
	UI_POWER,
	UI_TIME,
	CHECKPOINT_TELEPORTATION,
	MAP1,
	MAP1_TP1,
	MAP1_TP2,
	MAP1_TP3,
	MAP1_TP4,
	MAP1_TP5,
	MAP2,
	MAP2_TP1,
	MAP2_TP2,
	MAP2_TP3,
	MAP2_TP4,
	MAP2_TP5,
	YOU_DIED_LBL,
	PRESS_TO_CONTINUE_LBL
};

struct SceneData {
	std::string mapPath;
	std::string mapName;
	std::string musicPath;
};

class Scene : public Module
{
public:

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
	void LoadScene();
	void LoadMap();
	void EndScene();

	bool hasEnded;

private:

	EntityManager* entityManager;
	MissionManager* missionManager;
	DialogManager* dialogManager;
	Map* map;

	std::shared_ptr<Player> player;
	SceneData data;
	XMLHandler* persistance = new XMLHandler();
	Timer gameTimer;
	bool paused = false;

};
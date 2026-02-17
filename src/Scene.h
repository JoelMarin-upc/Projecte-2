#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Timer.h"
#include "Persistence.h"
#include "GroundEnemy.h"
#include "UIManager.h"

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

	void CreateUIElements(bool onlyPositions);
	void ShowMenu(Menu menu);
	void MainMenu();
	void SettingsMenu();
	void PauseMenu();
	void CheckpointsMenu();
	void DeathScreen();
	void CreditsMenu();
	void TogglePause();
	void ToggleCheckpoints();
	void ToggleDeathScreen();
	void StartGame(bool continueGame);
	void SaveGame();
	void LoadGame();
	void LoadScene();
	void LoadMap(SaveData* data = nullptr);
	void LoadAllScenes();
	void ReloadEnemies();
	void LoadEnemies(std::vector<EnemyData> enemyList);
	void ReloadItems();
	void LoadItems(std::vector<ItemData> itemList);
	SceneData GetCurrentScene();
	void NextScene();
	void ShowHelp();
	void DrawUI();
	Vector2D GetPlayerPosition();
	Vector2D GetPlayerPositionCenter();
	void LoadEntities(std::shared_ptr<Player>& player);
	void TeleportPlayer(int tp);
	bool OnUIMouseClickEvent(UIElement* uiElement);
	void CheckMusic();

	int currentScene;
	std::string currentMusic;
private:

	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::shared_ptr<Entity>> items;
	std::vector<SceneData> scenes;
	bool showHelp = false;
	SDL_Texture* helpTexture;
	SDL_Texture* lifeTexture;
	SDL_Texture* coinTexture;
	SDL_Texture* powerTexture;
	SDL_Texture* bossbar8;
	SDL_Texture* bossbar7;
	SDL_Texture* bossbar6;
	SDL_Texture* bossbar5;
	SDL_Texture* bossbar4;
	SDL_Texture* bossbar3;
	SDL_Texture* bossbar2;
	SDL_Texture* bossbar1;
	SDL_Texture* bossbar0;
	Persistence* persistance = new Persistence();
	Timer gameTimer;
	int savedTime = 0;
	int pausedTime = 0;
	std::shared_ptr<UILabel> coinsLabel;
	std::shared_ptr<UILabel> powerLabel;
	std::shared_ptr<UILabel> timeLabel;

	std::shared_ptr<UIButton> startGame;
	std::shared_ptr<UILabel> paused_lbl;
	std::shared_ptr<UILabel> settings_lbl;
	std::shared_ptr<UIButton> continueGame;
	std::shared_ptr<UIButton> resume;
	std::shared_ptr<UIButton> settings;
	std::shared_ptr<UIButton> credits_btn;
	std::shared_ptr<UILabel> credits_lbl;
	std::shared_ptr<UILabel> musicVolume_lbl;
	std::shared_ptr<UISlider> musicVolume;
	std::shared_ptr<UILabel> fxVolume_lbl;
	std::shared_ptr<UISlider> fxVolume;
	std::shared_ptr<UILabel> fullscreen_lbl;
	std::shared_ptr<UICheckbox> fullscreen;
	std::shared_ptr<UIButton> backMenu;
	std::shared_ptr<UIButton> backMainMenu;
	std::shared_ptr<UIButton> exit;
	std::shared_ptr<UILabel> checkpointTeleportation;
	std::shared_ptr<UILabel> map1;
	std::shared_ptr<UIButton> map1TP1;
	std::shared_ptr<UIButton> map1TP2;
	std::shared_ptr<UIButton> map1TP3;
	std::shared_ptr<UIButton> map1TP4;
	std::shared_ptr<UIButton> map1TP5;
	std::shared_ptr<UILabel> map2;
	std::shared_ptr<UIButton> map2TP1;
	std::shared_ptr<UIButton> map2TP2;
	std::shared_ptr<UIButton> map2TP3;
	std::shared_ptr<UIButton> map2TP4;
	std::shared_ptr<UIButton> map2TP5;
	std::shared_ptr<UILabel> youDied;
	std::shared_ptr<UILabel> pressToContinue;

	int saveFxId;
	int loadFxId;

	

	bool gameStarted = false;
	bool shouldExit = false;
	bool paused = false;
	bool checkpoints = false;
	bool deathScreen = false;
	Menu previousMenu = NONE;
	Menu currentMenu = NONE;
	SDL_Color* background = nullptr;
	SDL_Color menuBackground = { 200, 120, 180, 255 };
	SDL_Color pauseBackground = { 100, 100, 100, 100 };
	SDL_Texture* menuBackgroundTexture;
	bool useBackgroundTexture = false;

	int uiLockFrame = -1;

	int sw,sh;
};
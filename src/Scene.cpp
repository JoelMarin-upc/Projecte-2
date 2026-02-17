#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "Item.h"
#include "GroundEnemy.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "tracy/Tracy.hpp"

Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	helpTexture = Engine::GetInstance().textures->Load(configParameters.child("helpText").attribute("path").as_string());
	lifeTexture = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("life").as_string());
	coinTexture = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("coin").as_string());
	powerTexture = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("power").as_string());
	menuBackgroundTexture = Engine::GetInstance().textures->Load(configParameters.child("textures").attribute("menuBackground").as_string());

	bossbar8 = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("bossbar8").as_string());
	bossbar7 = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("bossbar7").as_string());
	bossbar6 = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("bossbar6").as_string());
	bossbar5 = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("bossbar5").as_string());
	bossbar4 = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("bossbar4").as_string());
	bossbar3 = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("bossbar3").as_string());
	bossbar2 = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("bossbar2").as_string());
	bossbar1 = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("bossbar1").as_string());
	bossbar0 = Engine::GetInstance().textures->Load(configParameters.child("ui").attribute("bossbar0").as_string());

	//initialize audio effect
	saveFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("save").as_string());
	loadFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("load").as_string());

	CreateUIElements(false);
	ShowMenu(MAIN);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	ZoneScoped;
	if (!gameStarted) return true;
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) TogglePause();
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) ToggleCheckpoints();
	if (deathScreen && Engine::GetInstance().input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) ToggleDeathScreen();
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	CheckMusic();
	ZoneScoped;
	if (!gameStarted) return true;
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) showHelp = !showHelp;
	ShowHelp();
	DrawUI();
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate(float dt)
{
	ZoneScoped;
	if (shouldExit) return false;
	bool ret = true;
	if (background) Engine::GetInstance().render->DrawRectangle({ 0, 0, Engine::GetInstance().render->camera.w, Engine::GetInstance().render->camera.h }, background->r, background->g, background->b, background->a, true, false);
	if (useBackgroundTexture)
	{
		int x = -(menuBackgroundTexture->w - Engine::GetInstance().render->camera.w) / 2;
		int y = -(menuBackgroundTexture->h - Engine::GetInstance().render->camera.h) / 2;
		Engine::GetInstance().render->DrawTexture(menuBackgroundTexture, x, y);
	}
	if (!gameStarted) return true;
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		for (auto& enemy : enemies) {
			enemy->ToggleDebug();
		}
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) SaveGame();
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) LoadGame();
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		currentScene = 0;
		//SaveGame();
		//SaveData data = persistance->Load();
		LoadScene();
		//player->lives = data.lives;
		//player->coins = data.coins;
		LoadMap();
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		currentScene = 1;
		//SaveGame();
		//SaveData data = persistance->Load();
		LoadScene();
		//player->lives = data.lives;
		//player->coins = data.coins;
		LoadMap();
	}
	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Scene::CreateUIElements(bool onlyPositions)
{
	SDL_Color mainColorDef = { 0, 0, 255, 255 };
	SDL_Color mainColorDis = { 200, 200, 200, 255 };
	SDL_Color mainColorHov = { 0, 0, 100, 255 };
	SDL_Color mainColorPre = { 0, 255, 255, 255 };
	SDL_Color secondaryDef = { 0, 255, 0, 255 };
	SDL_Color secondaryDis = { 100, 100, 100, 255 };
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color black = { 0, 0, 0, 255 };
	SDL_Color yellow = { 255, 255, 0, 255 };
	SDL_Color blue = { 0, 255, 255, 255 };
	SDL_Color red = { 255, 0, 0, 0 };

	sw = Engine::GetInstance().render->camera.w;
	sh = Engine::GetInstance().render->camera.h;

	SDL_Rect pos1 = { sw / 2, sh / 2 - 97.5f, 0, 0 };
	SDL_Rect pos2 = { sw / 2, sh / 2 - 47.5f, 0, 0 };
	SDL_Rect pos3 = { sw / 2, sh / 2 - 12.5f, 0, 0 };
	SDL_Rect pos4 = { sw / 2, sh / 2 + 22.5f, 0, 0 };
	SDL_Rect pos5 = { sw / 2, sh / 2 + 57.5f, 0, 0 };

	SDL_Rect b_startGame = { pos1.x - 125, pos1.y, 250, 40 };
	SDL_Rect b_paused_lbl = { pos1.x - 100, pos1.y, 200, 40 };
	SDL_Rect b_settings_lbl = { pos1.x - 100, pos1.y, 200, 40 };
	SDL_Rect b_continueGame = { pos2.x - 75, pos2.y, 150, 25 };
	SDL_Rect b_resume = { pos2.x - 75, pos2.y, 150, 25 };
	SDL_Rect b_settings = { pos4.x - 75, pos4.y, 150, 25 };
	SDL_Rect b_credits_btn = { pos3.x - 75, pos3.y, 150, 25 };
	SDL_Rect b_credits = { pos3.x - 400, pos3.y-100, 800, 25 };
	SDL_Rect b_musicVolume_lbl = { pos2.x - 300, pos2.y, 200, 25 };
	SDL_Rect b_musicVolume = { pos2.x + 100, pos2.y, 200, 25 };
	SDL_Rect b_fxVolume_lbl = { pos3.x - 300, pos3.y, 200, 25 };
	SDL_Rect b_fxVolume = { pos3.x + 100, pos3.y, 200, 25 };
	SDL_Rect b_fullscreen_lbl = { pos4.x - 300, pos4.y, 200, 25 };
	SDL_Rect b_fullscreen = { pos4.x + 100, pos4.y, 25, 25 };
	SDL_Rect b_backMenu = { pos5.x - 75, pos5.y, 150, 25 };
	SDL_Rect b_backMainMenu = { pos3.x - 75, pos3.y, 150, 25 };
	SDL_Rect b_exit = { pos5.x - 75, pos5.y, 150, 25 };
	SDL_Rect b_checkpointTeleportation = { pos1.x - 300, pos1.y -100, 600, 40 };
	SDL_Rect b_map1 = { pos1.x - 235, pos1.y - 50, 100, 25 };
	SDL_Rect b_map1TP1 = { pos1.x - 200, pos1.y, 30, 30 };
	SDL_Rect b_map1TP2 = { pos2.x - 200, pos2.y, 30, 30 };
	SDL_Rect b_map1TP3 = { pos3.x - 200, pos3.y, 30, 30 };
	SDL_Rect b_map1TP4 = { pos4.x - 200, pos4.y, 30, 30 };
	SDL_Rect b_map1TP5 = { pos5.x - 200, pos5.y, 30, 30 };
	SDL_Rect b_map2 = { pos1.x + 165, pos1.y - 50, 100, 25 };
	SDL_Rect b_map2TP1 = { pos1.x + 200, pos1.y, 30, 30 };
	SDL_Rect b_map2TP2 = { pos1.x + 200, pos2.y, 30, 30 };
	SDL_Rect b_map2TP3 = { pos1.x + 200, pos3.y, 30, 30 };
	SDL_Rect b_map2TP4 = { pos1.x + 200, pos4.y, 30, 30 };
	SDL_Rect b_map2TP5 = { pos1.x + 200, pos5.y, 30, 30 };
	SDL_Rect b_youDied = { pos1.x-250, pos1.y-150, 500, 200 };
	SDL_Rect b_pressToContinue = { pos1.x-150 , pos1.y +150, 300, 50 };

	int xCoins = Engine::GetInstance().render->camera.w - (coinTexture->w * 1.5f) - 20;
	int yCoins = lifeTexture->h + 5;
	SDL_Rect b_coins = { xCoins, yCoins, 0, 0 };

	int xPower = Engine::GetInstance().render->camera.w - (powerTexture->w * 1.5f) - 20;
	int yPower = lifeTexture->h + coinTexture->h + 5;
	SDL_Rect b_power = { xPower, yPower, 0, 0 };

	int xTime = Engine::GetInstance().render->camera.w - 70;
	int yTime = lifeTexture->h + coinTexture->h + powerTexture->h + 10;
	SDL_Rect b_time = { xTime, yTime, 0, 0 };

	if (onlyPositions) {
		startGame->SetBounds(b_startGame);
		paused_lbl->SetBounds(b_paused_lbl);
		settings_lbl->SetBounds(b_settings_lbl);
		continueGame->SetBounds(b_continueGame);
		resume->SetBounds(b_resume);
		settings->SetBounds(b_settings);
		credits_btn->SetBounds(b_credits_btn);
		credits_lbl->SetBounds(b_credits);
		musicVolume_lbl->SetBounds(b_musicVolume_lbl);
		musicVolume->SetBounds(b_musicVolume);
		fxVolume_lbl->SetBounds(b_fxVolume_lbl);
		fxVolume->SetBounds(b_fxVolume);
		fullscreen_lbl->SetBounds(b_fullscreen_lbl);
		fullscreen->SetBounds(b_fullscreen);
		backMenu->SetBounds(b_backMenu);
		backMainMenu->SetBounds(b_backMainMenu);
		exit->SetBounds(b_exit);
		coinsLabel->SetBounds(b_coins);
		powerLabel->SetBounds(b_power);
		timeLabel->SetBounds(b_time);
		checkpointTeleportation->SetBounds(b_checkpointTeleportation);
		map1->SetBounds(b_map1);
		map1TP1->SetBounds(b_map1TP1);
		map1TP2->SetBounds(b_map1TP2);
		map1TP3->SetBounds(b_map1TP3);
		map1TP4->SetBounds(b_map1TP4);
		map1TP5->SetBounds(b_map1TP5);
		map2->SetBounds(b_map1);
		map2TP1->SetBounds(b_map2TP1);
		map2TP2->SetBounds(b_map2TP2);
		map2TP3->SetBounds(b_map2TP3);
		map2TP4->SetBounds(b_map2TP4);
		map2TP5->SetBounds(b_map2TP5);
		youDied->SetBounds(b_youDied);
		pressToContinue->SetBounds(b_pressToContinue);
	}
	else {
		int hoverFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("hover").as_string());
		int clickFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("click").as_string());

		// MENUS
		startGame = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)START_GAME, b_startGame, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Start Game", 5)));
		paused_lbl = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)PAUSED_LBL, b_paused_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("PAUSED")));
		settings_lbl = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)SETTINGS_LBL, b_settings_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("SETTINGS")));
		continueGame = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)COUNTINUE_GAME, b_continueGame, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Continue Game", 5)));
		resume = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)RESUME, b_resume, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Resume", 5)));
		settings = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)SETTINGS_MENU, b_settings, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Settings", 5)));
		credits_btn = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)CREDITS_BTN, b_credits_btn, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Credits", 5)));
		credits_lbl = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)CREDITS_LBL, b_credits, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("This game was made by Oscar Jimenez and Joel Marin.")));
		musicVolume_lbl = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)MUSIC_VOLUME_LABEL, b_musicVolume_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Music volume")));
		musicVolume = std::dynamic_pointer_cast<UISlider>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, (int)MUSIC_VOLUME, b_musicVolume, this, { white, mainColorDis, mainColorDef, mainColorHov, mainColorPre, mainColorDis, white }, hoverFxId, clickFxId, UIParameters::Slider(true, 0, 10, 1, 10)));
		fxVolume_lbl = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)FX_VOLUME_LABEL, b_fxVolume_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("FX volume")));
		fxVolume = std::dynamic_pointer_cast<UISlider>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, (int)FX_VOLUME, b_fxVolume, this, { white, mainColorDis, mainColorDef, mainColorHov, mainColorPre, mainColorDis, white }, hoverFxId, clickFxId, UIParameters::Slider(true, 0, 10, 1, 10)));
		fullscreen_lbl = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)FULLSCREEN_LABEL, b_fullscreen_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Fullscreen")));
		fullscreen = std::dynamic_pointer_cast<UICheckbox>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::CHECKBOX, (int)FULLSCREEN, b_fullscreen, this, { mainColorDef, mainColorHov, mainColorDis, black, secondaryDef, secondaryDis }, hoverFxId, clickFxId, UIParameters::Checkbox(5, false)));
		backMenu = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)BACK_MENU, b_backMenu, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Back", 20)));
		backMainMenu = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)BACK_MAIN_MENU, b_backMainMenu, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Back to main menu", 5)));
		exit = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)EXIT, b_exit, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Exit", 20)));
		checkpointTeleportation = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)CHECKPOINT_TELEPORTATION, b_checkpointTeleportation, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Checkpoint Teleportation")));
		map1 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)MAP1, b_map1, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Map 1")));
		map1TP1 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP1_TP1, b_map1TP1, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("1", 5)));
		map1TP2 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP1_TP2, b_map1TP2, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("2", 5)));
		map1TP3 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP1_TP3, b_map1TP3, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("3", 5)));
		map1TP4 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP1_TP4, b_map1TP4, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("4", 5)));
		map1TP5 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP1_TP5, b_map1TP5, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("5", 5)));
		map2 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)MAP2, b_map2, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Map 2")));
		map2TP1 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP2_TP1, b_map2TP1, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("1", 5)));
		map2TP2 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP2_TP2, b_map2TP2, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("2", 5)));
		map2TP3 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP2_TP3, b_map2TP3, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("3", 5)));
		map2TP4 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP2_TP4, b_map2TP4, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("4", 5)));
		map2TP5 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MAP2_TP5, b_map2TP5, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("5", 5)));
		youDied = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)YOU_DIED_LBL, b_youDied, this, { red, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("YOU DIED")));
		pressToContinue = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)PRESS_TO_CONTINUE_LBL, b_pressToContinue, this, { red, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Press 'c' to continue")));

		// UI
		coinsLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, UI_COINS, b_coins, this, { yellow, mainColorDis }, hoverFxId, clickFxId));
		coinsLabel->active = false;

		powerLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, UI_POWER, b_power, this, { blue, mainColorDis }, hoverFxId, clickFxId));
		powerLabel->active = false;

		timeLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, UI_TIME, b_time, this, { white, mainColorDis }, hoverFxId, clickFxId));
		timeLabel->active = false;
	}
}

void Scene::ShowMenu(Menu menu)
{
	uiLockFrame = Engine::GetInstance().frameCount;

	startGame->active = false;
	paused_lbl->active = false;
	settings_lbl->active = false;
	continueGame->active = false;
	resume->active = false;
	settings->active = false;
	credits_btn->active = false;
	credits_lbl->active = false;
	musicVolume_lbl->active = false;
	musicVolume->active = false;
	fxVolume_lbl->active = false;
	fxVolume->active = false;
	fullscreen_lbl->active = false;
	fullscreen->active = false;
	backMenu->active = false;
	backMainMenu->active = false;
	exit->active = false;
	coinsLabel->active = false;
	powerLabel-> active = false;
	timeLabel->active = false;
	checkpointTeleportation->active = false;
	map1->active = false;
	map1TP1->active = false;
	map1TP2->active = false;
	map1TP3->active = false;
	map1TP4->active = false;
	map1TP5->active = false;
	map2->active = false;
	map2TP1->active = false;
	map2TP2->active = false;
	map2TP3->active = false;
	map2TP4->active = false;
	map2TP5->active = false;
	youDied->active = false;
	pressToContinue->active = false;
	background = nullptr;
	useBackgroundTexture = false;

	switch (menu)
	{
	case NONE:
		coinsLabel->active = true;
		powerLabel->active = true;
		timeLabel->active = true;
		break;
	case MAIN:
		MainMenu();
		break;
	case PAUSE:
		PauseMenu();
		break;
	case SETTINGS:
		SettingsMenu();
		break;
	case CHECKPOINTS:
		CheckpointsMenu();
		break;
	case DEATHSCREEN:
		DeathScreen();
		break;
	case CREDITS:
		CreditsMenu();
		break;
	default:
		break;
	}

	previousMenu = currentMenu;
	currentMenu = menu;
}

void Scene::MainMenu()
{
	if (previousMenu == NONE || previousMenu == MAIN || previousMenu == PAUSE) {
		currentMusic = (char*)configParameters.child("music").attribute("mainMenu").as_string();
		Engine::GetInstance().audio->PlayMusic(currentMusic.c_str());
	}
	background = &menuBackground;
	useBackgroundTexture = true;
	startGame->active = true;
	continueGame->active = true;
	if (persistance->SaveFileExists()) continueGame->Enable();
	else continueGame->Disable();
	credits_btn->active = true;
	settings->active = true;
	exit->active = true;
}

void Scene::SettingsMenu()
{
	if (gameStarted) background = &pauseBackground;
	else {
		background = &menuBackground;
		useBackgroundTexture = true;
	}
	coinsLabel->active = gameStarted;
	timeLabel->active = gameStarted;
	settings_lbl->active = true;
	musicVolume_lbl->active = true;
	musicVolume->active = true;
	musicVolume->SetValue(Engine::GetInstance().audio->GetMusicVolume() * 10);
	fxVolume_lbl->active = true;
	fxVolume->active = true;
	fxVolume->SetValue(Engine::GetInstance().audio->GetFxVolume() * 10);
	fullscreen_lbl->active = true;
	fullscreen->active = true;
	fullscreen->checked = Engine::GetInstance().window->fullscreen;
	backMenu->active = true;
}

void Scene::PauseMenu()
{
	background = &pauseBackground;
	coinsLabel->active = true;
	powerLabel->active = true;
	timeLabel->active = true;
	paused_lbl->active = true;
	resume->active = true;
	backMainMenu->active = true;
	settings->active = true;
	exit->active = true;
}

void Scene::CheckpointsMenu() {
	background = &pauseBackground;
	checkpointTeleportation->active = true;
	map1->active = true;
	map1TP1->active = true;
	map1TP2->active = true;
	map1TP3->active = true;
	map1TP4->active = true;
	map1TP5->active = true;
	map2->active = true;
	map2TP1->active = true;
	map2TP2->active = true;
	map2TP3->active = true;
	map2TP4->active = true;
	map2TP5->active = true;
}

void Scene::DeathScreen() {
	background = &pauseBackground;
	youDied->active = true;
	pressToContinue->active = true;
}


void Scene::CreditsMenu()
{
	background = &menuBackground;
	useBackgroundTexture = true;
	credits_lbl->active = true;
	backMainMenu->active = true;
}

void Scene::TogglePause()
{
	if (checkpoints || deathScreen) return;
	paused = !paused;
	Engine::GetInstance().entityManager->paused = paused;
	Engine::GetInstance().physics->paused = paused;
	ShowMenu(paused ? Menu::PAUSE : Menu::NONE);
	if (paused) pausedTime = gameTimer.ReadSec() + pausedTime;
	else gameTimer.Start();
}

void Scene::ToggleCheckpoints() {
	if (paused) return;
	checkpoints = !checkpoints;
	Engine::GetInstance().entityManager->paused = checkpoints;
	Engine::GetInstance().physics->paused = checkpoints;
	ShowMenu(checkpoints ? Menu::CHECKPOINTS : Menu::NONE);
	if (checkpoints) pausedTime = gameTimer.ReadSec() + pausedTime;
	else gameTimer.Start();
}

void Scene::ToggleDeathScreen() {
	deathScreen = !deathScreen;
	Engine::GetInstance().entityManager->paused = deathScreen;
	Engine::GetInstance().physics->paused = deathScreen;
	ShowMenu(deathScreen ? Menu::DEATHSCREEN : Menu::NONE);
	if (deathScreen) pausedTime = gameTimer.ReadSec() + pausedTime;
	else {
		gameTimer.Start();
		player->Respawn();
		Engine::GetInstance().scene->ReloadEnemies();
		Engine::GetInstance().scene->ReloadItems();
	}
}

void Scene::StartGame(bool continueGame)
{
	gameStarted = true;
	paused = false;
	Engine::GetInstance().entityManager->paused = false;
	Engine::GetInstance().physics->paused = false;
	currentScene = 0;
	LoadAllScenes();
	if (continueGame) {
		LoadGame();
	}
	else {
		LoadScene();
		LoadMap();
	}
	coinsLabel->active = true;
	powerLabel->active = true;
	timeLabel->active = true;
	gameTimer.Start();
	ShowMenu(NONE);
}

void Scene::SaveGame()
{
	Engine::GetInstance().audio->PlayFx(saveFxId);
	std::vector<EnemyData> enemyList;
	for (std::shared_ptr<Enemy> enemy : enemies) {
		if (!enemy->active) continue;
		EnemyData e;
		e.enType = enemy->enType;
		e.position = enemy->position;
		enemyList.push_back(e);
	}

	std::vector<ItemData> itemList;
	for (std::shared_ptr<Entity> item : items) {
		if (!item->active) continue;
		ItemData i;
		i.type = (int)item->type;
		i.position = item->position;
		itemList.push_back(i);
	}

	SaveData data(currentScene, gameTimer.ReadSec() + savedTime + pausedTime, player->lives, player->coins, player->hasItem1, player->hasItem2, player->position, player->respawnPos, enemyList, itemList);
	persistance->Save(data);
}

void Scene::LoadGame()
{
	SaveData data = persistance->Load();
	if (!data.exists) return;
	gameTimer.Start();
	currentScene = data.currentScene;
	savedTime = data.time;
	pausedTime = 0;
	LoadScene();
	LoadMap(&data);
}

void Scene::LoadScene()
{
	Engine::GetInstance().audio->PlayFx(loadFxId);
	Engine::GetInstance().physics->CreateWorld();
	Engine::GetInstance().entityManager->entities.clear();
	enemies.clear();
	items.clear();
	player = nullptr;
	auto playerEntity = Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player = std::dynamic_pointer_cast<Player>(playerEntity);
	player->speed = configParameters.child("player").attribute("speed").as_float();
	player->texturePath = configParameters.child("player").attribute("texture").as_string();
	player->textureDamagedPath = configParameters.child("player").attribute("textureDamaged").as_string();
	player->animationsPath = configParameters.child("player").attribute("animationSet").as_string();
	player->itemChargeTexture0Path = configParameters.child("player").attribute("itemChargeTexture0").as_string();
	player->itemChargeTexture1Path = configParameters.child("player").attribute("itemChargeTexture1").as_string();
	player->itemChargeTexture2Path = configParameters.child("player").attribute("itemChargeTexture2").as_string();
	if (currentScene == 0) {
		player->hasItem1 = false;
		player->hasItem2 = false;
	}
	else if (currentScene == 1) {
		player->hasItem1 = true;
		player->hasItem2 = false;
	}
	Engine::GetInstance().render->follow = playerEntity;
}

void Scene::LoadMap(SaveData* data)
{
	auto currentScene = GetCurrentScene();
	currentMusic = currentScene.musicPath.c_str();
	Engine::GetInstance().audio->PlayMusic(currentMusic.c_str());
	Engine::GetInstance().map->CleanUp();
	Engine::GetInstance().map->Load(currentScene.mapPath, currentScene.mapName);
	if (data) 
	{
		LoadEnemies(data->enemies);
		LoadItems(data->items);
		player->lives = data->lives;
		player->coins = data->coins;
		player->hasItem1 = data->hasItem1;
		player->hasItem2 = data->hasItem2;
		player->hasLivesSet = true;
		player->respawnPos = data->playerPosition;
		player->hasRespawn = true;
	}
	else
	{
		auto enemyList = Engine::GetInstance().map->GetEnemies();
		auto itemList = Engine::GetInstance().map->GetItems();
		LoadEnemies(enemyList);
		LoadItems(itemList);
	}
	player->Start();
	if (data) player->respawnPos = data->playerRespawnPoint;
}

void Scene::LoadAllScenes()
{
	for (pugi::xml_node mapNode = configParameters.child("maps").child("map"); mapNode != NULL; mapNode = mapNode.next_sibling("map")) {
		SceneData scene = SceneData();
		scene.mapPath = mapNode.attribute("path").as_string();
		scene.mapName = mapNode.attribute("name").as_string();
		scene.musicPath = mapNode.attribute("music").as_string();
		scenes.push_back(scene);
	}
}

void Scene::ReloadEnemies()
{
	bool debug = false;
	if (enemies.size() > 0 && enemies[0]->pathfinding->debug) debug = true;
	for (auto& e : enemies) {
		Engine::GetInstance().physics->DeletePhysBody(e->pbody);
		e->pbody = nullptr;
		e->Disable();
	}
	enemies.clear();
	auto enemyList = Engine::GetInstance().map->GetEnemies();
	LoadEnemies(enemyList);
	if (debug) for (auto& e : enemies) e->ToggleDebug();
}

void Scene::LoadEnemies(std::vector<EnemyData> enemyList)
{
	for (EnemyData enemy : enemyList) {
		std::shared_ptr<Entity> e;
		if (enemy.enType == 0) e = Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY_AIR);
		else if (enemy.enType == 1) e = Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY_GROUND);
		else {
			e = Engine::GetInstance().entityManager->CreateEntity(EntityType::BOSS);
		}
		e->position = enemy.position;
		enemies.push_back(std::dynamic_pointer_cast<Enemy>(e));
		e->Awake();
		e->Start();
	}
}

void Scene::ReloadItems() {
	std::vector<ItemData> itemList(0);
	itemList.clear();
	for (auto& i : items) {
		ItemData id;
		id.position = i->position;
		id.type = (char)i->type;

		if (i->type == EntityType::KEY) {
			if (player->doorOpen == true);
			else itemList.push_back(id);
		}
		else if (i->type == EntityType::COIN && i->active) itemList.push_back(id);
		else if (i->type == EntityType::ITEM && i->active) itemList.push_back(id);
		i->Disable();
	}

	items.clear();
	std::vector<ItemData> itemListFull = Engine::GetInstance().map->GetItems();
	for (auto& i : itemListFull) {
		if (i.type == (char)EntityType::HEART || i.type == (char)EntityType::RECHARGE) itemList.push_back(i);
	}
	LoadItems(itemList);
}

void Scene::LoadItems(std::vector<ItemData> itemList)
{
	for (ItemData item : itemList) {
		EntityType t = (EntityType)item.type;
		std::shared_ptr<Entity> i = Engine::GetInstance().entityManager->CreateEntity(t);
		i->position = item.position;
		items.push_back(std::dynamic_pointer_cast<Entity>(i));
		i->Awake();
		i->Start();
	}
}

SceneData Scene::GetCurrentScene()
{
	return scenes[currentScene];
}

void Scene::NextScene()
{
	currentScene++;
	if (currentScene >= scenes.size()) {
		currentScene = 0;
		gameStarted = false;
		paused = false;
		ShowMenu(CREDITS);
		return;
	}
	LoadScene();
	LoadMap();
}

void Scene::ShowHelp()
{
	if (!showHelp) return;
	Engine::GetInstance().render->DrawTexture(helpTexture, -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y);
}

void Scene::DrawUI()
{
	int x, y;
	int lives = player->lives;
	for (int i = 0; i < lives; i++) {
		x = Engine::GetInstance().render->camera.w - lifeTexture->w - (i * (lifeTexture->w + 5)) - 5 - Engine::GetInstance().render->camera.x;
		y = -Engine::GetInstance().render->camera.y + 5;
		Engine::GetInstance().render->DrawTexture(lifeTexture, x, y);
	}

	x = Engine::GetInstance().render->camera.w - coinTexture->w - 5 - Engine::GetInstance().render->camera.x;
	y = -Engine::GetInstance().render->camera.y + 8 + lifeTexture->h;
	
	//for (int i = 0; i < coins; i++) Engine::GetInstance().render->DrawTexture(coinTexture, x1 - (i * 20), y); // sacar
	Engine::GetInstance().render->DrawTexture(coinTexture, x, y);

	x = Engine::GetInstance().render->camera.w - powerTexture->w - 5 - Engine::GetInstance().render->camera.x;
	y = -Engine::GetInstance().render->camera.y + 8 + lifeTexture->h + coinTexture->h;
	Engine::GetInstance().render->DrawTexture(powerTexture, x, y);

	int coins = player->coins;
	coinsLabel->text = std::to_string(coins).c_str();

	int power = 0;
	if (player->hasItem2) power = 2;
	else if (player->hasItem1) power = 1;
	powerLabel->text = std::to_string(power).c_str();


	int totalSeconds = paused ? savedTime + pausedTime : gameTimer.ReadSec() + savedTime + pausedTime;
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << minutes
		<< ":"
		<< std::setw(2) << std::setfill('0') << seconds;
	std::string time = oss.str();

	timeLabel->text = time;


	x = Engine::GetInstance().render->camera.w - (sw / 2) - 160 - Engine::GetInstance().render->camera.x;;
	y = -Engine::GetInstance().render->camera.y + 10.0f;

	if (player->inBoss) {
		int lives = Engine::GetInstance().entityManager->aliveTentacles;
		switch (lives) {
			case 8:
				Engine::GetInstance().render->DrawTexture(bossbar8, x, y);
				break;
			case 7:
				Engine::GetInstance().render->DrawTexture(bossbar7, x, y);
				break;
			case 6:
				Engine::GetInstance().render->DrawTexture(bossbar6, x, y);
				break;
			case 5:
				Engine::GetInstance().render->DrawTexture(bossbar5, x, y);
				break;
			case 4:
				Engine::GetInstance().render->DrawTexture(bossbar4, x, y);
				break;
			case 3:
				Engine::GetInstance().render->DrawTexture(bossbar3, x, y);
				break;
			case 2:
				Engine::GetInstance().render->DrawTexture(bossbar2, x, y);
				break;
			case 1:
				Engine::GetInstance().render->DrawTexture(bossbar1, x, y);
				break;
			case 0:
				Engine::GetInstance().render->DrawTexture(bossbar0, x, y);
				break;
		}
	}
}

Vector2D Scene::GetPlayerPosition()
{
	return player->GetPosition();
}

Vector2D Scene::GetPlayerPositionCenter()
{
	return player->GetPositionCenter();
}

void Scene::TeleportPlayer(int tp) {
	b2Vec2 newPos;
	if (tp < 6) {
		if (player->checkpointPositions0.size() < tp) return;
		if (currentScene == 0) {
			newPos = player->checkpointPositions0[tp - 1];
		}
		else if (currentScene == 1) {
			currentScene = 0;
			LoadScene();
			LoadMap();
			newPos = player->checkpointPositions0[tp - 1];
		}
	}
	else {
		if (player->checkpointPositions1.size() < tp - 5) return;
		if (currentScene == 0) {
			currentScene = 1;
			LoadScene();
			LoadMap();
			newPos = player->checkpointPositions1[tp - 6];
		}
		else if (currentScene == 1) {
			newPos = player->checkpointPositions1[tp - 6];
		}
	}
	
	
	player->position;
	b2Body_SetTransform(player->pbody->body, { PIXEL_TO_METERS(newPos.x),PIXEL_TO_METERS(newPos.y) }, { 1,0 });
	ToggleCheckpoints();
}

void Scene::CheckMusic() {
	if (Engine::GetInstance().audio->MusicPlaying()) return;
	Engine::GetInstance().audio->PlayMusic(currentMusic.c_str());

}

bool Scene::OnUIMouseClickEvent(UIElement* uiElement)
{
	if (uiLockFrame == Engine::GetInstance().frameCount) return true;

	// L16: TODO 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", uiElement->id);

	float musicVol;
	float fxVol;
	switch ((UIID)uiElement->id)
	{
	case START_GAME:
		StartGame(false);
		break;
	case PAUSED_LBL:
		break;
	case SETTINGS_LBL:
		break;
	case COUNTINUE_GAME:
		StartGame(true);
		break;
	case RESUME:
		TogglePause();
		break;
	case SETTINGS_MENU:
		ShowMenu(SETTINGS);
		break;
	case CREDITS_BTN:
		ShowMenu(CREDITS);
		break;
	case CREDITS_LBL:
		break;
	case MUSIC_VOLUME_LABEL:
		break;
	case MUSIC_VOLUME:
		musicVol = ((UISlider*)uiElement)->GetValue() / 10;
		Engine::GetInstance().audio->SetMusicVolume(musicVol);
		break;
	case FX_VOLUME_LABEL:
		break;
	case FX_VOLUME:
		fxVol = ((UISlider*)uiElement)->GetValue() / 10;
		Engine::GetInstance().audio->SetFxVolume(fxVol);
		break;
	case FULLSCREEN_LABEL:
		break;
	case FULLSCREEN:
		Engine::GetInstance().window->SetFullscreen(((UICheckbox*)uiElement)->checked);
		CreateUIElements(true);
		break;
	case BACK_MENU:
		ShowMenu(previousMenu);
		break;
	case BACK_MAIN_MENU:
		ShowMenu(MAIN);
		gameStarted = false;
		break;
	case MAP1_TP1:
		TeleportPlayer(1);
		break;
	case MAP1_TP2:
		TeleportPlayer(2);
		break;
	case MAP1_TP3:
		TeleportPlayer(3);
		break;
	case MAP1_TP4:
		TeleportPlayer(4);
		break;
	case MAP1_TP5:
		TeleportPlayer(5);
		break;
	case MAP2_TP1:
		TeleportPlayer(6);
		break;
	case MAP2_TP2:
		TeleportPlayer(7);
		break;
	case MAP2_TP3:
		TeleportPlayer(8);
		break;
	case MAP2_TP4:
		TeleportPlayer(9);
		break;
	case MAP2_TP5:
		TeleportPlayer(10);
		break;
	case EXIT:
		shouldExit = true;
		break;
	default:
		break;
	}

	return true;
}
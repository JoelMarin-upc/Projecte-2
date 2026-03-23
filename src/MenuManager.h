#pragma once

#include "Module.h"
#include "Inventory.h"
#include <list>

enum UIID {
	GAME_TITLE,
	CREDITS,
	PAUSED_LBL,
	START_GAME,
	CONTINUE_GAME,
	RESUME_GAME,
	SETTINGS,
	SETTINGS_LABEL,
	MUSIC_VOLUME_LABEL,
	MUSIC_VOLUME,
	FX_VOLUME_LABEL,
	FX_VOLUME,
	FULLSCREEN_LABEL,
	FULLSCREEN,
	BACK_MENU,
	BACK_MAIN_MENU,
	EXIT
};

class MenuManager : public Module
{
public:

	MenuManager();

	// Destructor
	virtual ~MenuManager();

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

	void Load();
	void ShowMainMenu();
	void ShowPauseMenu();
	void ShowSettingsMenu();
	void ShowInventory(Inventory* inventory);
	void ShowShop(Inventory* customer, Inventory* shop = nullptr);
	void HideMenu();

private:
	void ShowInventory(Inventory* inventory, bool isShop);

	// UI ELEMENTS
};

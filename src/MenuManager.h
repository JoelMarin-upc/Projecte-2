#pragma once

#include "Module.h"
#include "Inventory.h"
#include "UIManager.h"
#include <list>

enum MenuType {
	MAIN,
	PAUSE,
	SETTINGS,
	CREDITS,
	INVENTORY,
	SHOP,
	COMBAT_INVENTORY,
	DEATHSCREEN,
	NONE
};

enum UIID {
	GAME_TITLE,
	LOGO,
	PAUSED_LABEL,
	START_GAME,
	CONTINUE_GAME,
	RESUME_GAME,
	CREDITS_BUTTON,
	CREDITS_LABEL,
	GAME_OVER_LABEL,
	SETTINGS_BUTTON,
	SETTINGS_LABEL,
	MUSIC_VOLUME_LABEL,
	MUSIC_VOLUME,
	FX_VOLUME_LABEL,
	FX_VOLUME,
	FULLSCREEN_LABEL,
	FULLSCREEN,
	BACK_MENU,
	BACK_MAIN_MENU,
	EXIT,
	C_PLAYER,
	C_NPC1,
	C_NPC2,
	C_NPC3,
	C_ENEMY1,
	C_ENEMY2,
	C_ENEMY3,
	C_ENEMY4,
	ACTION1,
	ACTION2,
	ACTION3,
	ACTION4,
	END_TURN,
	LOG1,
	LOG2,
	LOG3,
	LOG4,
	STANCE1,
	STANCE2,
	STANCE3,
	STANCE4,
	HINT,
	INVENTORY_LABEL,
	MONEY_LABEL,
	SHOP_LABEL,
	USE,
	DROP,
	BUY,
	SELL,
	AMOUNT,
	HELMET,
	BODY,
	BOOTS,
	WEAPON,
	SELECTED_ITEM,
	EXIT_SHOP
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

	void Load(bool onlyPositions);
	void LoadInventory(bool onlyPositions);
	void SetObserver(Module* observer);
	void ShowMainMenu();
	void ShowPauseMenu();
	void ShowSettingsMenu();
	void ShowCreditsMenu();
	void ShowInventory(Inventory* inventory);
	void ShowShop(Inventory* customer, Inventory* shop);
	void ShowDeathScreen();
	void HideMenu();
	void ShowPreviousMenu();
	void RedrawInventory();
	void ShowCombatInventory(Inventory* inventory);

private:
	void ShowInventory(Inventory* inventory, bool isShop);

public:
	MenuType currentMenu;
	MenuType previousMenu;

	std::shared_ptr<UIImage> gameTitle;
	std::shared_ptr<UIImage> studioLogo;
	std::shared_ptr<UILabel> pausedLabel;
	std::shared_ptr<UILabel> gameOverLabel;
	std::shared_ptr<UIButton> startGame;
	std::shared_ptr<UIButton> continueGame;
	std::shared_ptr<UIButton> resumeGame;
	std::shared_ptr<UIButton> settingsButton;
	std::shared_ptr<UIButton> creditsButton;
	std::shared_ptr<UILabel> settingsLabel;
	std::shared_ptr<UILabel> creditsLabel1;
	std::shared_ptr<UILabel> creditsLabel2;
	std::shared_ptr<UILabel> musicVolumeLabel;
	std::shared_ptr<UISlider> musicVolumeSlider;
	std::shared_ptr<UILabel> fxVolumeLabel;
	std::shared_ptr<UISlider> fxVolumeSlider;
	std::shared_ptr<UILabel> fullscreenLabel;
	std::shared_ptr<UICheckbox> fullscreenCheckbox;
	std::shared_ptr<UIButton> backMenu;
	std::shared_ptr<UIButton> backMainMenu;
	std::shared_ptr<UIButton> exit;

	int uiLockFrame = -1;

	const int baseSlotsId = 900;
	const int baseShopSlotsId = 950;
	std::vector<std::shared_ptr<UISlot>> inventorySlots;
	std::vector<std::shared_ptr<UISlot>> shopSlots;
	std::shared_ptr<UILabel> inventoryLabel;
	std::shared_ptr<UILabel> moneyLabel;
	std::shared_ptr<UILabel> shopLabel;
	std::shared_ptr<UIButton> use;
	std::shared_ptr<UIButton> drop;
	std::shared_ptr<UIButton> buy;
	std::shared_ptr<UIButton> sell;
	std::shared_ptr<UISlider> amount;
	std::shared_ptr<UIImage> helmet;
	std::shared_ptr<UIImage> body;
	std::shared_ptr<UIImage> boots;
	std::shared_ptr<UIImage> weapon;
	std::shared_ptr<UISlot> selectedItem;
	std::shared_ptr<UIButton> exitShop;

	Inventory* currentInventory = nullptr;
	Inventory* currentShop = nullptr;
};
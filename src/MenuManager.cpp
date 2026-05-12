#include "MenuManager.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Item.h"
#include "Audio.h"
#include "Window.h"
#include "SceneManager.h"

MenuManager::MenuManager() : Module()
{
	name = "MenuManager";
}

// Destructor
MenuManager::~MenuManager()
{}

// Called before render is available
bool MenuManager::Awake()
{
	previousMenu = NONE;
	currentMenu = NONE;

	return true;
}

bool MenuManager::Start() 
{
	Load(false);
	return true;
}

bool MenuManager::PreUpdate() { return true; }

bool MenuManager::Update(float dt) 
{
	if (!showingPopUp && !popUpQueue.empty()) {
		ShowMissionPopup(popUpQueue.front());
		popUpQueue.pop();
	}
	if (showingPopUp && popUpTimer.ReadSec() > popUpSeconds) {
		HideMissionPopup();
	}
	return true; 
}

bool MenuManager::PostUpdate(float dt) { return true; }

// Called before quitting
bool MenuManager::CleanUp() { return true; }

void MenuManager::Load(bool onlyPositions)
{
	SDL_Texture* title = Engine::GetInstance().textures->Load("Assets/Textures/Game_Title.png");
	SDL_Texture* backNormal = Engine::GetInstance().textures->Load("Assets/Textures/backButtonNormal.png");
	SDL_Texture* backHov = Engine::GetInstance().textures->Load("Assets/Textures/backButtonHover.png");
	SDL_Texture* backPres = Engine::GetInstance().textures->Load("Assets/Textures/backButtonPressed.png");
	SDL_Texture* backDis = backNormal;
	SDL_Texture* continueNormal = Engine::GetInstance().textures->Load("Assets/Textures/continueButtonNormal.png");
	SDL_Texture* continueHov = Engine::GetInstance().textures->Load("Assets/Textures/continueButtonHover.png");
	SDL_Texture* continuePres = Engine::GetInstance().textures->Load("Assets/Textures/continueButtonPressed.png");
	SDL_Texture* continueDis = Engine::GetInstance().textures->Load("Assets/Textures/continueButtonDissabled.png");
	SDL_Texture* creditsNormal = Engine::GetInstance().textures->Load("Assets/Textures/creditsButtonNormal.png");
	SDL_Texture* creditsHov = Engine::GetInstance().textures->Load("Assets/Textures/creditsButtonHover.png");
	SDL_Texture* creditsPres = Engine::GetInstance().textures->Load("Assets/Textures/creditsButtonPressed.png");
	SDL_Texture* creditsDis = creditsNormal;
	SDL_Texture* exitNormal = Engine::GetInstance().textures->Load("Assets/Textures/exitButtonNormal.png");
	SDL_Texture* exitHov = Engine::GetInstance().textures->Load("Assets/Textures/exitButtonHover.png");
	SDL_Texture* exitPres = Engine::GetInstance().textures->Load("Assets/Textures/exitButtonPressed.png");
	SDL_Texture* exitDis = exitNormal;
	SDL_Texture* resumeNormal = Engine::GetInstance().textures->Load("Assets/Textures/resumeButtonNormal.png");
	SDL_Texture* resumeHov = Engine::GetInstance().textures->Load("Assets/Textures/resumeButtonHover.png");
	SDL_Texture* resumePres = Engine::GetInstance().textures->Load("Assets/Textures/resumeButtonPressed.png");
	SDL_Texture* resumeDis = resumeNormal;
	SDL_Texture* settingsNormal = Engine::GetInstance().textures->Load("Assets/Textures/settingsButtonNormal.png");
	SDL_Texture* settingsHov = Engine::GetInstance().textures->Load("Assets/Textures/settingsButtonHover.png");
	SDL_Texture* settingsPres = Engine::GetInstance().textures->Load("Assets/Textures/settingsButtonPressed.png");
	SDL_Texture* settingsDis = settingsNormal;
	SDL_Texture* startNormal = Engine::GetInstance().textures->Load("Assets/Textures/startButtonNormal.png");
	SDL_Texture* startHov = Engine::GetInstance().textures->Load("Assets/Textures/startButtonHover.png");
	SDL_Texture* startPres = Engine::GetInstance().textures->Load("Assets/Textures/startButtonPressed.png");
	SDL_Texture* startDis = startNormal;


	SDL_Color mainColorDef = { 0, 0, 255, 255 };
	SDL_Color mainColorDis = { 200, 200, 200, 255 };
	SDL_Color mainColorHov = { 0, 0, 100, 255 };
	SDL_Color mainColorPre = { 0, 255, 255, 255 };
	SDL_Color secondaryDef = { 0, 255, 0, 255 };
	SDL_Color secondaryDis = { 100, 100, 100, 255 };
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color black = { 0, 0, 0, 255 };
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color yellow = { 255, 255, 0, 255 };
	SDL_Color blue = { 0, 255, 255, 255 };
	SDL_Color red = { 255, 0, 0, 0 };

	/*int sw = Engine::GetInstance().render->camera.w;
	int sh = Engine::GetInstance().render->camera.h;*/

	int sw = Engine::GetInstance().window->width;
	int sh = Engine::GetInstance().window->height;

	int centerX = sw / 2;
	int centerY = sh / 2;

	//Main menu elements
	SDL_Rect b_gameTitle = { centerX - title->w / 2, centerY - title->h/2 - 150, 0, 0 };

	SDL_Rect b_popUpTitle = { 20, 20, 350, 40 };
	SDL_Rect b_popUp = { 20, 80, sw / 3 * 2, 100 };

	SDL_Rect b_missionJournalTitle = { 20, 20, 200, 60 };
	
	SDL_Rect b_startGame = { centerX - 75, centerY, 144, 64 };
	SDL_Rect b_continueGame = { centerX - 100, centerY + 70, 192, 64 };
	SDL_Rect b_settings = { centerX - 100, centerY + 140, 192, 64 };
	SDL_Rect b_credits_btn = { centerX - 90, centerY + 210, 176, 64 };
	SDL_Rect b_exit = { centerX - 75, centerY + 280, 144, 64 };
	
	//Settings menu elements
	SDL_Rect b_settings_lbl = { centerX - 100, centerY - 200, 200, 40 };
	SDL_Rect b_musicVolume_lbl = { centerX - 300, centerY - 100, 200, 25 };
	SDL_Rect b_musicVolume = { centerX + 100, centerY - 100, 200, 25 };
	SDL_Rect b_fxVolume_lbl = { centerX - 300, centerY - 50, 200, 25 };
	SDL_Rect b_fxVolume = { centerX + 100, centerY - 50, 200, 25 };
	SDL_Rect b_fullscreen_lbl = { centerX - 300, centerY - 0, 200, 25 };
	SDL_Rect b_fullscreen = { centerX + 100, centerY - 0, 25, 25 };
	SDL_Rect b_vsync_lbl = { centerX - 300, centerY + 50, 200, 25 };
	SDL_Rect b_vsync = { centerX + 100, centerY + 50, 25, 25 };
	SDL_Rect b_backMenu = { centerX - 100, centerY + 210, 144, 64 };

	//Credits menu elements
	SDL_Rect b_credits1 = { centerX - 500, centerY - 200, 1000, 25 };
	SDL_Rect b_credits2 = { centerX - 500, centerY - 160, 1000, 25 };
	SDL_Rect b_backMainMenu = { centerX - 75, centerY + 210, 144, 64 };

	//Pause menu elements
	SDL_Rect b_paused_lbl = { centerX - 60, centerY - 10, 200, 40 };
	SDL_Rect b_resume = { centerX - 95, centerY + 70, 176, 64 };

	//Other elements
	SDL_Rect b_gameOver_lbl = { centerX - 90, centerY - 100, 300, 60 };
	
	if (onlyPositions) {
		gameTitle->SetBounds(b_gameTitle);
		startGame->SetBounds(b_startGame);
		pausedLabel->SetBounds(b_paused_lbl);
		gameOverLabel->SetBounds(b_gameOver_lbl);
		settingsLabel->SetBounds(b_settings_lbl);
		continueGame->SetBounds(b_continueGame);
		resumeGame->SetBounds(b_resume);
		settingsButton->SetBounds(b_settings);
		creditsButton->SetBounds(b_credits_btn);
		creditsLabel1->SetBounds(b_credits1);
		creditsLabel2->SetBounds(b_credits2);
		musicVolumeLabel->SetBounds(b_musicVolume_lbl);
		musicVolumeSlider->SetBounds(b_musicVolume);
		fxVolumeLabel->SetBounds(b_fxVolume_lbl);
		fxVolumeSlider->SetBounds(b_fxVolume);
		fullscreenLabel->SetBounds(b_fullscreen_lbl);
		vsyncLabel->SetBounds(b_vsync_lbl);
		vsyncCheckbox->SetBounds(b_vsync);
		fullscreenCheckbox->SetBounds(b_fullscreen);

		backMenu->SetBounds(b_backMenu);
		backMainMenu->SetBounds(b_backMainMenu);
		exit->SetBounds(b_exit);
		missionPopUpTitle->SetBounds(b_popUpTitle);
		missionPopUp->SetBounds(b_popUp);
		missionJournalTitle->SetBounds(b_missionJournalTitle);
		for (int i = 0; i < missionJournal.size(); i++) {
			int y = (i + 1) * 100;
			SDL_Rect bounds = { 20, y, sw - 40, 80 };
			missionJournal[i]->SetBounds(bounds);
		}
	}
	else {
		int hoverFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("hover").as_string());
		int clickFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("click").as_string());

		// MENUS
		gameTitle = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)GAME_TITLE, b_gameTitle, this, {  }, hoverFxId, clickFxId, UIParameters::Image(title, title, title, title)));
		startGame = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)START_GAME, b_startGame, this, {  }, hoverFxId, clickFxId, UIParameters::Image(startDis, startNormal, startHov, startPres)));
		pausedLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)PAUSED_LABEL, b_paused_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("PAUSED")));
		gameOverLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)GAME_OVER_LABEL , b_gameOver_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("GAME OVER")));
		settingsLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)SETTINGS_LABEL, b_settings_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("SETTINGS")));
		continueGame = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)CONTINUE_GAME, b_continueGame, this, { }, hoverFxId, clickFxId, UIParameters::Image(continueDis, continueNormal, continueHov, continuePres)));
		resumeGame = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)RESUME_GAME, b_resume, this, { }, hoverFxId, clickFxId, UIParameters::Image(resumeDis, resumeNormal, resumeHov, resumePres)));
		settingsButton = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)SETTINGS_BUTTON, b_settings, this, { }, hoverFxId, clickFxId, UIParameters::Image(settingsDis, settingsNormal, settingsHov, settingsPres)));
		creditsButton = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)CREDITS_BUTTON, b_credits_btn, this, { }, hoverFxId, clickFxId, UIParameters::Image(creditsDis, creditsNormal, creditsHov, creditsPres)));
		creditsLabel1 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)CREDITS_LABEL, b_credits1, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("This game was made by TEAMDAYO, a game studio created by")));
		creditsLabel2 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)CREDITS_LABEL, b_credits2, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Clara Sanchez, Sofia Barja, Yin Ye, Kirsten Neubauer and Joel Marin.")));
		musicVolumeLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)MUSIC_VOLUME_LABEL, b_musicVolume_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Music volume")));
		musicVolumeSlider = std::dynamic_pointer_cast<UISlider>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, (int)MUSIC_VOLUME, b_musicVolume, this, { white, mainColorDis, mainColorDef, mainColorHov, mainColorPre, mainColorDis, white }, hoverFxId, clickFxId, UIParameters::Slider(true, 0, 10, 1, 10)));
		fxVolumeLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)FX_VOLUME_LABEL, b_fxVolume_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("FX volume")));
		fxVolumeSlider = std::dynamic_pointer_cast<UISlider>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, (int)FX_VOLUME, b_fxVolume, this, { white, mainColorDis, mainColorDef, mainColorHov, mainColorPre, mainColorDis, white }, hoverFxId, clickFxId, UIParameters::Slider(true, 0, 10, 1, 10)));
		fullscreenLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)FULLSCREEN_LABEL, b_fullscreen_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Fullscreen")));
		fullscreenCheckbox = std::dynamic_pointer_cast<UICheckbox>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::CHECKBOX, (int)FULLSCREEN, b_fullscreen, this, { mainColorDef, mainColorHov, mainColorDis, black, secondaryDef, secondaryDis }, hoverFxId, clickFxId, UIParameters::Checkbox(5, false)));
		vsyncLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)VSYNC_LABEL, b_vsync_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("VSync")));
		vsyncCheckbox = std::dynamic_pointer_cast<UICheckbox>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::CHECKBOX, (int)VSYNC, b_vsync, this, { mainColorDef, mainColorHov, mainColorDis, black, secondaryDef, secondaryDis }, hoverFxId, clickFxId, UIParameters::Checkbox(5, false)));
		backMenu = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)BACK_MENU, b_backMenu, this, { }, hoverFxId, clickFxId, UIParameters::Image(backDis, backNormal, backHov, backPres)));
		backMainMenu = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)BACK_MAIN_MENU, b_backMainMenu, this, { }, hoverFxId, clickFxId, UIParameters::Image(backDis, backNormal, backHov, backPres)));
		exit = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)EXIT, b_exit, this, { }, hoverFxId, clickFxId, UIParameters::Image(exitDis, exitNormal, exitHov, exitPres)));
		
		missionPopUpTitle = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)POPUP_TITLE, b_popUpTitle, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, green }, hoverFxId, clickFxId, UIParameters::Button("", 20)));
		missionPopUp = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)POPUP, b_popUp, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("", 20)));
		missionPopUpTitle->state = UIElementState::DISABLED;
		missionPopUp->state = UIElementState::DISABLED;

		missionJournalTitle = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)MISSION_JOURNAL_TITLE, b_missionJournalTitle, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Mission journal", 20)));
		missionJournalTitle->state = UIElementState::DISABLED;

		int journalEntries = (sw - 100) / 100;
		for (int i = 0; i < journalEntries; i++) {
			int y = (i + 1) * 100;
			SDL_Rect bounds = { 20, y, sw - 40, 80 };
			std::shared_ptr<UIButton> mission = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, baseJournalId + i, bounds, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("", 20)));
			mission->state = UIElementState::DISABLED;
			missionJournal.push_back(mission);
		}
	}

	LoadInventory(onlyPositions);
}

void MenuManager::LoadInventory(bool onlyPositions)
{
	int sw = Engine::GetInstance().window->width;
	int sh = Engine::GetInstance().window->height;

	int centerX = sw / 2;
	int centerY = sh / 2;

	SDL_Rect b_inventoryLabel = { 20, 20, 200, 20 };
	SDL_Rect b_moneyLabel = { centerX - 60, 20, 100, 20 };
	SDL_Rect b_shopLabel = { sw - 80, 20, 60, 20 };
	SDL_Rect b_use = { centerX - 110, 60, 90, 40};
	SDL_Rect b_drop = { centerX + 10, 60, 90, 40 };
	SDL_Rect b_buy = { centerX - 110, 120, 90, 40 };
	SDL_Rect b_sell = { centerX + 10, 120, 90, 40 };
	SDL_Rect b_helmet = { centerX - 65, centerY - 100, 60, 60 };
	SDL_Rect b_body = { centerX - 65, centerY - 30, 60, 60 };
	SDL_Rect b_boots = { centerX - 65, centerY + 40, 60, 60 };
	SDL_Rect b_weapon = { centerX + 5, centerY - 30, 60, 60 };
	SDL_Rect b_amount = { centerX - 100, 180, 200, 40 };
	SDL_Rect b_selectedItem = { centerX - 100, sh - 200, 200, 160 };
	SDL_Rect b_exitShop = { sw - 200, 15, 100, 30 };
	SDL_Rect b_hp = { sw / 2 + 150, sh / 2 - 110, 200, 40 };
	SDL_Rect b_attack = { sw / 2 + 150, sh / 2 - 50, 200, 40 };
	SDL_Rect b_defense = { sw / 2 + 150, sh / 2 + 10, 200, 40 };
	SDL_Rect b_speed = { sw / 2 + 150, sh / 2 + 70, 200, 40 };
	std::vector<SDL_Rect> inventorySlotBounds = std::vector<SDL_Rect>();
	std::vector<SDL_Rect> shopSlotBounds = std::vector<SDL_Rect>();
	const int margin = 10;
	const int columns = 4;
	const int rows = MAX_SLOTS / columns;
	int x = margin;
	int y = margin + 40;
	int boxW = 160;
	int boxY = 150;

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			inventorySlotBounds.push_back({ x + boxW * i + margin * i, y + boxY * j + margin * j, boxW, boxY });

	x = sw;
	y = margin + 40;

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			shopSlotBounds.push_back({ x - boxW * i - margin * i - boxW - margin, y + boxY * j + margin * j, boxW, boxY });

	if (onlyPositions) {
		hp->SetBounds(b_hp);
		attack->SetBounds(b_attack);
		defense->SetBounds(b_defense);
		speed->SetBounds(b_speed);
		inventoryLabel->SetBounds(b_inventoryLabel);
		moneyLabel->SetBounds(b_moneyLabel);
		shopLabel->SetBounds(b_shopLabel);
		use->SetBounds(b_use);
		drop->SetBounds(b_drop);
		buy->SetBounds(b_buy);
		sell->SetBounds(b_sell);
		amount->SetBounds(b_amount);
		helmet->SetBounds(b_helmet);
		body->SetBounds(b_body);
		boots->SetBounds(b_boots);
		weapon->SetBounds(b_weapon);
		selectedItem->SetBounds(b_selectedItem);
		exitShop->SetBounds(b_exitShop);
		for (int i = 0; i < inventorySlotBounds.size(); i++) {
			if (inventorySlots.size() <= i) continue;
			std::shared_ptr<UISlot> slot = inventorySlots[i];
			slot->SetBounds(inventorySlotBounds[i]);
		}
		for (int i = 0; i < shopSlotBounds.size(); i++) {
			if (shopSlots.size() <= i) continue;
			std::shared_ptr<UISlot> slot = shopSlots[i];
			slot->SetBounds(shopSlotBounds[i]);
		}
	}
	else {
		int hoverFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("hover").as_string());
		int clickFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("click").as_string());

		SDL_Color mainColorDef = { 100, 100, 100, 255 };
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

		hp = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)HP_STAT, b_hp, this, { mainColorDef, mainColorDef, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("HP: ", 5)));
		attack = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)ATTACK_STAT, b_attack, this, { mainColorDef, mainColorDef, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Attack: ", 5)));
		defense = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)DEFEND_STAT, b_defense, this, { mainColorDef, mainColorDef, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Defense: ", 5)));
		speed = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)SPEED_STAT, b_speed, this, { mainColorDef, mainColorDef, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Speed: ", 5)));
		hp->state = UIElementState::DISABLED;
		attack->state = UIElementState::DISABLED;
		defense->state = UIElementState::DISABLED;
		speed->state = UIElementState::DISABLED;

		inventoryLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)INVENTORY_LABEL, b_inventoryLabel, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Inventory")));
		moneyLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)MONEY_LABEL, b_moneyLabel, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Gold: ")));
		shopLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)SHOP_LABEL, b_shopLabel, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Shop")));
		use = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)USE, b_use, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Use", 5)));
		drop = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)DROP, b_drop, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Drop", 5)));
		buy = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)BUY, b_buy, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Buy", 5)));
		sell = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)SELL, b_sell, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Sell", 5)));
		amount = std::dynamic_pointer_cast<UISlider>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, (int)AMOUNT, b_amount, this, { white, mainColorDis, mainColorDef, mainColorHov, mainColorPre, mainColorDis, white }, hoverFxId, clickFxId, UIParameters::Slider(true, 1, 10, 1, 10)));
		helmet = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)HELMET, b_helmet, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Image(nullptr, nullptr, nullptr, nullptr, new SDL_Color(mainColorDef), true)));
		helmet->state = UIElementState::DISABLED;
		body = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)BODY, b_body, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Image(nullptr, nullptr, nullptr, nullptr, new SDL_Color(mainColorDef), true)));
		body->state = UIElementState::DISABLED;
		boots = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)BOOTS, b_boots, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Image(nullptr, nullptr, nullptr, nullptr, new SDL_Color(mainColorDef), true)));
		boots->state = UIElementState::DISABLED;
		weapon = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)WEAPON, b_weapon, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Image(nullptr, nullptr, nullptr, nullptr, new SDL_Color(mainColorDef), true)));
		weapon->state = UIElementState::DISABLED;
		selectedItem = std::dynamic_pointer_cast<UISlot>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLOT, (int)SELECTED_ITEM, b_selectedItem, this, { mainColorDef, mainColorDef, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Default()));
		selectedItem->state = UIElementState::DISABLED;
		exitShop = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)EXIT_SHOP, b_exitShop, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Exit", 5, 5)));
		for (int i = 0; i < inventorySlotBounds.size(); i++) {
			std::shared_ptr<UISlot> slot = std::dynamic_pointer_cast<UISlot>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLOT, (int)baseSlotsId + i, inventorySlotBounds[i], this, {mainColorDef, mainColorDis, mainColorHov, mainColorPre, white}, hoverFxId, clickFxId, UIParameters::Default()));
			inventorySlots.push_back(slot);
		}
		for (int i = 0; i < shopSlotBounds.size(); i++) {
			std::shared_ptr<UISlot> slot = std::dynamic_pointer_cast<UISlot>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLOT, (int)baseShopSlotsId + i, shopSlotBounds[i], this, {mainColorDef, mainColorDis, mainColorHov, mainColorPre, white}, hoverFxId, clickFxId, UIParameters::Default()));
			shopSlots.push_back(slot);
		}
	}
}

void MenuManager::SetObserver(Module* observer) 
{
	gameTitle->observer = observer;
	startGame->observer = observer;
	pausedLabel->observer = observer;
	settingsLabel->observer = observer;
	continueGame->observer = observer;
	resumeGame->observer = observer;
	settingsButton->observer = observer;
	creditsButton->observer = observer;
	creditsLabel1->observer = observer;
	creditsLabel2->observer = observer;
	musicVolumeLabel->observer = observer;
	musicVolumeSlider->observer = observer;
	fxVolumeLabel->observer = observer;
	fxVolumeSlider->observer = observer;
	fullscreenLabel->observer = observer;
	fullscreenCheckbox->observer = observer;
	vsyncLabel->observer = observer;
	vsyncCheckbox->observer = observer;
	backMenu->observer = observer;
	backMainMenu->observer = observer;
	exit->observer = observer;
	missionPopUpTitle->observer = observer;
	missionPopUp->observer = observer;
	missionJournalTitle->observer = observer;
	inventoryLabel->observer = observer;
	moneyLabel->observer = observer;
	shopLabel->observer = observer;
	use->observer = observer;
	drop->observer = observer;
	buy->observer = observer;
	sell->observer = observer;
	amount->observer = observer;
	helmet->observer = observer;
	body->observer = observer;
	boots->observer = observer;
	weapon->observer = observer;
	selectedItem->observer = observer;
	exitShop->observer = observer;
	hp->observer = observer;
	attack->observer = observer;
	defense->observer = observer;
	speed->observer = observer;
	for (std::shared_ptr<UISlot> slot : inventorySlots) slot->observer = observer;
	for (std::shared_ptr<UISlot> slot : shopSlots) slot->observer = observer;
	for (std::shared_ptr<UIButton> mission : missionJournal) mission->observer = observer;
}

void MenuManager::ShowMainMenu()
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = MAIN;

	gameTitle->active = true;
	startGame->active = true;
	continueGame->active = true;
	pugi::xml_document doc = XMLHandler::LoadFile("Assets/Entities/characters.xml");
	bool hasSave = doc.child("characters").attribute("savedGame").as_bool(false);
	if (!hasSave) {
		continueGame->Disable();
	}
	else {
		continueGame->Enable();
	}
	//if (XMLHandler::SaveFileExists()) continueGame->Enable();
	//else continueGame->Disable();
	creditsButton->active = true;
	settingsButton->active = true;
	exit->active = true;
}

void MenuManager::ShowPauseMenu()
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = PAUSE;

	pausedLabel->active = true;
	resumeGame->active = true;
	backMainMenu->active = true;
	settingsButton->active = true;
	exit->active = true;
}

void MenuManager::ShowSettingsMenu()
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = SETTINGS;

	settingsLabel->active = true;
	musicVolumeLabel->active = true;
	musicVolumeSlider->active = true;
	musicVolumeSlider->SetValue(Engine::GetInstance().audio->GetTargetMusicVolume() * 10);
	fxVolumeLabel->active = true;
	fxVolumeSlider->active = true;
	fxVolumeSlider->SetValue(Engine::GetInstance().audio->GetFxVolume() * 10);
	fullscreenLabel->active = true;
	fullscreenCheckbox->active = true;
	fullscreenCheckbox->checked = Engine::GetInstance().window->fullscreen;
	vsyncLabel->active = true;
	vsyncCheckbox->active = true;
	vsyncCheckbox->checked = Engine::GetInstance().vsyncEnabled;

	backMenu->active = true;
}

void MenuManager::ShowCreditsMenu()
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = CREDITS;

	creditsLabel1->active = true;
	creditsLabel2->active = true;
	backMainMenu->active = true;
}

void MenuManager::ShowInventory(Inventory* inventory, std::shared_ptr<Character> character)
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = INVENTORY;

	ShowInventory(inventory, false, character);
}

void MenuManager::ShowShop(Inventory* customer, Inventory* shop)
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = SHOP;

	ShowInventory(customer, false);
	ShowInventory(shop, true);
}

void MenuManager::ShowDeathScreen()
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = DEATHSCREEN;

	gameOverLabel->active = true;
	backMainMenu->active = true;
	exit->active = true;
}

void MenuManager::AddMissionPopup(Mission* mission)
{
	popUpQueue.push(mission);
}

void MenuManager::ShowMissionPopup(Mission* mission, float popUpSeconds)
{
	this->popUpSeconds = popUpSeconds;
	popUpTimer = Timer();
	showingPopUp = true;
	missionPopUpTitle->active = true;
	missionPopUp->active = true;
	missionPopUpTitle->text = mission->completed ? "MISSION COMPLETED" : "NEW MISSION";
	missionPopUp->text = mission->ToString();
}

void MenuManager::HideMissionPopup()
{
	showingPopUp = false;
	missionPopUpTitle->active = false;
	missionPopUp->active = false;
	if (!popUpQueue.empty())
	{
		ShowMissionPopup(popUpQueue.front());
		popUpQueue.pop();
	}
}

void MenuManager::ShowMissionJournal(MissionManager* missionManager)
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = MISSION_JOURNAL;

	missionJournalTitle->active = true;
	std::vector<Mission*> missions = missionManager->GetActiveMissions();
	for (int i = 0; i < missions.size(); i++) {
		if (i >= missionJournal.size()) break;
		missionJournal[i]->active = true;
		missionJournal[i]->text = missions[i]->ToString();
	}
}

void MenuManager::HideMenu()
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	currentMenu = NONE;
	gameTitle->active = false;
	startGame->active = false;
	pausedLabel->active = false;
	gameOverLabel->active = false;
	settingsLabel->active = false;
	continueGame->active = false;
	resumeGame->active = false;
	settingsButton->active = false;
	creditsButton->active = false;
	creditsLabel1->active = false;
	creditsLabel2->active = false;
	musicVolumeLabel->active = false;
	musicVolumeSlider->active = false;
	fxVolumeLabel->active = false;
	fxVolumeSlider->active = false;
	fullscreenLabel->active = false;
	fullscreenCheckbox->active = false;
	vsyncLabel->active = false;
	vsyncCheckbox->active = false;
	backMenu->active = false;
	backMainMenu->active = false;
	exit->active = false;
	missionPopUpTitle->active = false;
	missionPopUp->active = false;
	showingPopUp = false;
	popUpQueue = std::queue<Mission*>();
	missionJournalTitle->active = false;
	for (std::shared_ptr<UIButton> mission : missionJournal) mission->active = false;

	inventoryLabel->active = false;
	moneyLabel->active = false;
	shopLabel->active = false;
	use->active = false;
	drop->active = false;
	buy->active = false;
	sell->active = false;
	amount->active = false;
	helmet->active = false;
	body->active = false;
	boots->active = false;
	weapon->active = false;
	selectedItem->SetItem(nullptr);
	selectedItem->active = false;
	exitShop->active = false;
	hp->active = false;
	attack->active = false;
	defense->active = false;
	speed->active = false;
	for (std::shared_ptr<UISlot> slot : inventorySlots)
	{
		slot->SetItem(nullptr);
		slot->active = false;
	}
	for (std::shared_ptr<UISlot> slot : shopSlots)
	{
		slot->SetItem(nullptr);
		slot->active = false;
	}
}

void MenuManager::ShowPreviousMenu()
{
	switch (previousMenu)
	{
	case MAIN:
		ShowMainMenu();
		break;
	case PAUSE:
		ShowPauseMenu();
		break;
	case SETTINGS:
		ShowSettingsMenu();
		break;
	case CREDITS:
		ShowCreditsMenu();
		break;
	case DEATHSCREEN:
		ShowDeathScreen();
		break;
	case NONE:
		HideMenu();
		break;
	}
}

void MenuManager::RedrawInventory()
{
	ShowInventory(currentInventory, false, currentCharacter);
	if (currentMenu == SHOP) ShowInventory(currentShop, true);
	selectedItem->SetItem(nullptr);
}

void MenuManager::ShowInventory(Inventory* inventory, bool isShop, std::shared_ptr<Character> character)
{
	amount->active = true;
	selectedItem->active = true;
	if (isShop) {
		currentShop = inventory;
		shopLabel->active = true;
		buy->active = true;
		sell->active = true;
		exitShop->active = true;
		hp->active = false;
		attack->active = false;
		defense->active = false;
		speed->active = false;
		for (int i = 0; i < shopSlots.size(); i++) {
			std::shared_ptr<UISlot> slot = shopSlots[i];
			slot->active = true;
			slot->SetItem(nullptr);
			if (i >= inventory->items.size()) continue;
			std::shared_ptr<InteractableItem> item = inventory->items[i];
			slot->SetItem(item, item->count);
		}
	}
	else {
		currentInventory = inventory;
		currentCharacter = character;
		inventoryLabel->active = true;
		moneyLabel->text = "Gold: " + std::to_string(inventory->gold);
		moneyLabel->active = true;
		use->text = "Use";
		use->active = true;
		drop->active = true;

		if (character) {
			hp->active = true;
			hp->text = "HP: " + std::to_string((int)character->HP()) + "/" + std::to_string((int)character->MaxHP());
			attack->active = true;
			attack->text = "Attack: " + std::to_string((int)character->Attack());
			defense->active = true;
			defense->text = "Defense: " + std::to_string((int)character->Defense());
			speed->active = true;
			speed->text = "Speed: " + std::to_string((int)character->Speed());
		}
		else {
			hp->active = false;
			attack->active = false;
			defense->active = false;
			speed->active = false;
		}
		
		for (int i = 0; i < inventorySlots.size(); i++) {
			std::shared_ptr<UISlot> slot = inventorySlots[i];
			slot->active = true;
			slot->SetItem(nullptr);
			if (i >= inventory->items.size()) continue;
			std::shared_ptr<InteractableItem> item = inventory->items[i];
			slot->SetItem(item, item->count);
		}
		helmet->active = true;
		body->active = true;
		boots->active = true;
		weapon->active = true;
		if (inventory->equippedHelmet) helmet->SetImage(inventory->equippedHelmet->icon, inventory->equippedHelmet->icon, inventory->equippedHelmet->icon, inventory->equippedHelmet->icon);
		else helmet->SetImage(nullptr, nullptr, nullptr, nullptr);
		if (inventory->equippedBody) body->SetImage(inventory->equippedBody->icon, inventory->equippedBody->icon, inventory->equippedBody->icon, inventory->equippedBody->icon);
		else body->SetImage(nullptr, nullptr, nullptr, nullptr);
		if (inventory->equippedBoots) boots->SetImage(inventory->equippedBoots->icon, inventory->equippedBoots->icon, inventory->equippedBoots->icon, inventory->equippedBoots->icon);
		else boots->SetImage(nullptr, nullptr, nullptr, nullptr);
		if (inventory->equippedWeapon) weapon->SetImage(inventory->equippedWeapon->icon, inventory->equippedWeapon->icon, inventory->equippedWeapon->icon, inventory->equippedWeapon->icon);
		else weapon->SetImage(nullptr, nullptr, nullptr, nullptr);
	}
}

void MenuManager::ShowCombatInventory(Inventory* inventory)
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = COMBAT_INVENTORY;

	std::vector<std::shared_ptr<InteractableItem>> consumables = std::vector<std::shared_ptr<InteractableItem>>();
	for (auto& item : inventory->items)
		if (std::dynamic_pointer_cast<Consumable>(item)) 
			consumables.push_back(item);

	for (int i = 0; i < inventorySlots.size(); i++) {
		std::shared_ptr<UISlot> slot = inventorySlots[i];
		slot->active = true;
		slot->SetItem(nullptr);
		if (i >= consumables.size()) continue;
		std::shared_ptr<InteractableItem> consumable = consumables[i];
		slot->SetItem(consumable, consumable->count);
	}
}

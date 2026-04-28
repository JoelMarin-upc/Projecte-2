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

bool MenuManager::Update(float dt) { return true; }

bool MenuManager::PostUpdate(float dt) { return true; }

// Called before quitting
bool MenuManager::CleanUp() { return true; }

void MenuManager::Load(bool onlyPositions)
{
	SDL_Texture* title = Engine::GetInstance().textures->Load("Assets/Textures/Game_Title.png");

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

	/*int sw = Engine::GetInstance().render->camera.w;
	int sh = Engine::GetInstance().render->camera.h;*/

	int sw = Engine::GetInstance().window->width;
	int sh = Engine::GetInstance().window->height;

	int centerX = sw / 2;
	int centerY = sh / 2;

	//Main menu elements
	SDL_Rect b_gameTitle = { centerX - title->w / 2, centerY - title->h/2 - 150, 0, 0 };
	SDL_Rect b_startGame = { centerX - 140, centerY + 10, 280, 50 };
	SDL_Rect b_continueGame = { centerX - 125, centerY + 80, 250, 35 };
	SDL_Rect b_settings = { centerX - 125, centerY + 130, 250, 35 };
	SDL_Rect b_credits_btn = { centerX - 125, centerY + 180, 250, 35 };
	SDL_Rect b_exit = { centerX - 125, centerY + 230, 250, 35 };
	
	//Settings menu elements
	SDL_Rect b_settings_lbl = { centerX - 100, centerY - 200, 200, 40 };
	SDL_Rect b_musicVolume_lbl = { centerX - 300, centerY - 100, 200, 25 };
	SDL_Rect b_musicVolume = { centerX + 100, centerY - 100, 200, 25 };
	SDL_Rect b_fxVolume_lbl = { centerX - 300, centerY - 50, 200, 25 };
	SDL_Rect b_fxVolume = { centerX + 100, centerY - 50, 200, 25 };
	SDL_Rect b_fullscreen_lbl = { centerX - 300, centerY - 0, 200, 25 };
	SDL_Rect b_fullscreen = { centerX + 100, centerY - 0, 25, 25 };
	SDL_Rect b_backMenu = { centerX - 125, centerY + 50, 250, 35 };

	//Credits menu elements
	SDL_Rect b_credits1 = { centerX - 500, centerY - 200, 1000, 25 };
	SDL_Rect b_credits2 = { centerX - 500, centerY - 160, 1000, 25 };
	SDL_Rect b_backMainMenu = { centerX - 125, centerY + 180, 250, 35 };

	//Pause menu elements
	SDL_Rect b_paused_lbl = { centerX - 60, centerY - 10, 200, 40 };
	SDL_Rect b_resume = { centerX - 125, centerY + 80, 250, 35 };

	//Other elements
	SDL_Rect b_gameOver_lbl = { centerX - 150, centerY, 300, 60 };
	
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
		fullscreenCheckbox->SetBounds(b_fullscreen);
		backMenu->SetBounds(b_backMenu);
		backMainMenu->SetBounds(b_backMainMenu);
		exit->SetBounds(b_exit);
	}
	else {
		int hoverFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("hover").as_string());
		int clickFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("click").as_string());

		// MENUS
		gameTitle = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)GAME_TITLE, b_gameTitle, this, {  }, hoverFxId, clickFxId, UIParameters::Image(title, title, title, title)));;
		startGame = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)START_GAME, b_startGame, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Start Game", 5)));
		pausedLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)PAUSED_LABEL, b_paused_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("PAUSED")));
		gameOverLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)GAME_OVER_LABEL , b_gameOver_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("GAME OVER")));
		settingsLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)SETTINGS_LABEL, b_settings_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("SETTINGS")));
		continueGame = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)CONTINUE_GAME, b_continueGame, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Continue Game", 5)));
		resumeGame = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)RESUME_GAME, b_resume, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Resume", 5)));
		settingsButton = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)SETTINGS_BUTTON, b_settings, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Settings", 5)));
		creditsButton = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)CREDITS_BUTTON, b_credits_btn, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Credits", 5)));
		creditsLabel1 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)CREDITS_LABEL, b_credits1, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("This game was made by TEAMDAYO, a game studio created by")));
		creditsLabel2 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)CREDITS_LABEL, b_credits2, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Clara Sanchez, Sofia Barja, Yin Ye, Kirsten Neubauer and Joel Marin.")));
		musicVolumeLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)MUSIC_VOLUME_LABEL, b_musicVolume_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Music volume")));
		musicVolumeSlider = std::dynamic_pointer_cast<UISlider>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, (int)MUSIC_VOLUME, b_musicVolume, this, { white, mainColorDis, mainColorDef, mainColorHov, mainColorPre, mainColorDis, white }, hoverFxId, clickFxId, UIParameters::Slider(true, 0, 10, 1, 10)));
		fxVolumeLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)FX_VOLUME_LABEL, b_fxVolume_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("FX volume")));
		fxVolumeSlider = std::dynamic_pointer_cast<UISlider>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, (int)FX_VOLUME, b_fxVolume, this, { white, mainColorDis, mainColorDef, mainColorHov, mainColorPre, mainColorDis, white }, hoverFxId, clickFxId, UIParameters::Slider(true, 0, 10, 1, 10)));
		fullscreenLabel = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)FULLSCREEN_LABEL, b_fullscreen_lbl, this, { white, mainColorDis }, hoverFxId, clickFxId, UIParameters::Label("Fullscreen")));
		fullscreenCheckbox = std::dynamic_pointer_cast<UICheckbox>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::CHECKBOX, (int)FULLSCREEN, b_fullscreen, this, { mainColorDef, mainColorHov, mainColorDis, black, secondaryDef, secondaryDis }, hoverFxId, clickFxId, UIParameters::Checkbox(5, false)));
		backMenu = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)BACK_MENU, b_backMenu, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Back", 20)));
		backMainMenu = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)BACK_MAIN_MENU, b_backMainMenu, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Back to main menu", 5)));
		exit = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)EXIT, b_exit, this, { mainColorDef, mainColorDis, mainColorHov, mainColorPre, white }, hoverFxId, clickFxId, UIParameters::Button("Exit", 20)));
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
	backMenu->observer = observer;
	backMainMenu->observer = observer;
	exit->observer = observer;
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
	for (std::shared_ptr<UISlot> slot : inventorySlots) slot->observer = observer;
	for (std::shared_ptr<UISlot> slot : shopSlots) slot->observer = observer;
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

void MenuManager::ShowInventory(Inventory* inventory)
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = INVENTORY;

	ShowInventory(inventory, false);
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
	backMenu->active = false;
	backMainMenu->active = false;
	exit->active = false;

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
	ShowInventory(currentInventory, false);
	if (currentMenu == SHOP) ShowInventory(currentShop, true);
	selectedItem->SetItem(nullptr);
}

void MenuManager::ShowInventory(Inventory* inventory, bool isShop)
{
	amount->active = true;
	selectedItem->active = true;
	if (isShop) {
		currentShop = inventory;
		shopLabel->active = true;
		buy->active = true;
		sell->active = true;
		exitShop->active = true;
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
		inventoryLabel->active = true;
		moneyLabel->text = "Gold: " + std::to_string(inventory->gold);
		moneyLabel->active = true;
		use->text = "Use";
		use->active = true;
		drop->active = true;
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

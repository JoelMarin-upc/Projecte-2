#include "MenuManager.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Item.h"
#include "Audio.h"
#include "Window.h"


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

	SDL_Rect pos1 = { sw / 2, sh / 2 - 197.5f, 0, 0 };
	SDL_Rect pos2 = { sw / 2, sh / 2 - 147.5f, 0, 0 };
	SDL_Rect pos3 = { sw / 2, sh / 2 - 112.5f, 0, 0 };
	SDL_Rect pos4 = { sw / 2, sh / 2 + 122.5f, 0, 0 };
	SDL_Rect pos5 = { sw / 2, sh / 2 + 157.5f, 0, 0 };

	SDL_Rect b_gameTitle = { pos1.x - title->w / 2, pos1.y - title->h + 220, 0, 0 };
	SDL_Rect b_startGame = { pos1.x - 125, pos1.y + 200, 250, 40 };
	SDL_Rect b_paused_lbl = { pos1.x - 100, pos1.y, 200, 40 };
	SDL_Rect b_gameOver_lbl = { pos1.x - 150, pos1.y, 300, 60 };
	SDL_Rect b_settings_lbl = { pos1.x - 100, pos1.y, 200, 40 };
	SDL_Rect b_continueGame = { pos2.x - 75, pos2.y + 200, 150, 25 };
	SDL_Rect b_resume = { pos2.x - 75, pos2.y, 150, 25 };
	SDL_Rect b_settings = { pos4.x - 75, pos4.y, 150, 25 };
	SDL_Rect b_credits_btn = { pos3.x - 75, pos3.y + 200, 150, 25 };
	SDL_Rect b_credits1 = { pos3.x - 500, pos3.y - 130, 1000, 25 };
	SDL_Rect b_credits2 = { pos3.x - 500, pos3.y - 90, 1000, 25 };
	SDL_Rect b_musicVolume_lbl = { pos2.x - 300, pos2.y, 200, 25 };
	SDL_Rect b_musicVolume = { pos2.x + 100, pos2.y, 200, 25 };
	SDL_Rect b_fxVolume_lbl = { pos3.x - 300, pos3.y, 200, 25 };
	SDL_Rect b_fxVolume = { pos3.x + 100, pos3.y, 200, 25 };
	SDL_Rect b_fullscreen_lbl = { pos4.x - 300, pos4.y, 200, 25 };
	SDL_Rect b_fullscreen = { pos4.x + 100, pos4.y, 25, 25 };
	SDL_Rect b_backMenu = { pos5.x - 75, pos5.y, 150, 25 };
	SDL_Rect b_backMainMenu = { pos3.x - 75, pos3.y, 150, 25 };
	SDL_Rect b_exit = { pos5.x - 75, pos5.y, 150, 25 };

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
}

void MenuManager::ShowMainMenu()
{
	uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = MAIN;

	gameTitle->active = true;
	startGame->active = true;
	continueGame->active = true;
	if (XMLHandler::SaveFileExists()) continueGame->Enable();
	else continueGame->Disable();
	creditsButton->active = true;
	settingsButton->active = true;
	exit->active = true;
}

void MenuManager::ShowPauseMenu()
{
	uiLockFrame = Engine::GetInstance().frameCount;

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
	uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = SETTINGS;

	settingsLabel->active = true;
	musicVolumeLabel->active = true;
	musicVolumeSlider->active = true;
	musicVolumeSlider->SetValue(Engine::GetInstance().audio->GetMusicVolume() * 10);
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
	uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = CREDITS;

	creditsLabel1->active = true;
	creditsLabel2->active = true;
	backMainMenu->active = true;
	studioLogo->active = true;
}

void MenuManager::ShowInventory(Inventory* inventory)
{
	uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = INVENTORY;

	ShowInventory(inventory, false);
}

void MenuManager::ShowShop(Inventory* customer, Inventory* shop)
{
	uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = SHOP;

	ShowInventory(customer, false);
	ShowInventory(shop, true);
}

void MenuManager::ShowDeathScreen()
{
	uiLockFrame = Engine::GetInstance().frameCount;

	previousMenu = currentMenu;
	HideMenu();
	currentMenu = DEATHSCREEN;

	gameOverLabel->active = true;
	backMainMenu->active = true;
	exit->active = true;
}

void MenuManager::HideMenu()
{
	uiLockFrame = Engine::GetInstance().frameCount;

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

void MenuManager::ShowInventory(Inventory* inventory, bool isShop)
{
	// SHOW INVENTORY
	// if (isShop) { right } else { left }
}

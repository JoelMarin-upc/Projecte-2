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
	SDL_Rect b_backMenu = { centerX - 100, centerY + 210, 144, 64 };

	//Credits menu elements
	SDL_Rect b_credits1 = { centerX - 500, centerY - 200, 1000, 25 };
	SDL_Rect b_credits2 = { centerX - 500, centerY - 160, 1000, 25 };
	SDL_Rect b_backMainMenu = { centerX - 75, centerY + 210, 144, 64 };

	//Pause menu elements
	SDL_Rect b_paused_lbl = { centerX - 60, centerY - 10, 200, 40 };
	SDL_Rect b_resume = { centerX - 95, centerY + 70, 176, 64 };

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
		backMenu = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)BACK_MENU, b_backMenu, this, { }, hoverFxId, clickFxId, UIParameters::Image(backDis, backNormal, backHov, backPres)));
		backMainMenu = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)BACK_MAIN_MENU, b_backMainMenu, this, { }, hoverFxId, clickFxId, UIParameters::Image(backDis, backNormal, backHov, backPres)));
		exit = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)EXIT, b_exit, this, { }, hoverFxId, clickFxId, UIParameters::Image(exitDis, exitNormal, exitHov, exitPres)));
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

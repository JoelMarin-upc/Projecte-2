#include "MenuManager.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Item.h"


MenuManager::MenuManager() : Module()
{
	name = "MenuManager";
	// INITIALIZE UI ELEMENTS
}

// Destructor
MenuManager::~MenuManager()
{}

// Called before render is available
bool MenuManager::Awake()
{
	return true;
}

bool MenuManager::Start() 
{
	return true;
}

// Called before quitting
bool MenuManager::CleanUp()
{
	return true;
}

void MenuManager::Load()
{
	
}

void MenuManager::ShowMainMenu()
{
	HideMenu();
	// SHOW MAIN MENU
}

void MenuManager::ShowPauseMenu()
{
	HideMenu();
	// SHOW PAUSE MENU
}

void MenuManager::ShowSettingsMenu()
{
	HideMenu();
	// SHOW SETTINGS MENU
}

void MenuManager::ShowInventory(Inventory* inventory)
{
	HideMenu();
	ShowInventory(inventory, false);
}

void MenuManager::ShowShop(Inventory* customer, Inventory* shop)
{
	HideMenu();
	ShowInventory(customer, false);
	ShowInventory(shop, true);
}

void MenuManager::HideMenu()
{
	// HIDE ALL UI ELEMENTS
}

void MenuManager::ShowInventory(Inventory* inventory, bool isShop)
{
	// SHOW INVENTORY
	// if (isShop) { right } else { left }
}

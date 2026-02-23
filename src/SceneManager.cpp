#include "SceneManager.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Item.h"


SceneManager::SceneManager() : Module()
{
	name = "scenemanager";
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake()
{
	
	return true;
}

bool SceneManager::Start() {

	return true;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	return true;
}

void SceneManager::LoadScenes()
{

}

void SceneManager::SetCurrentScene(std::string sceneID)
{

}

bool SceneManager::Update(float dt)
{
	return true;
}
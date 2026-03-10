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

bool SceneManager::Start() 
{
	LoadScenes();
	SetCurrentScene("SC-001");
	return true;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	return true;
}

void SceneManager::LoadScenes()
{
	// load from xml
	scenes = std::list<Scene*>();
	scenes.push_back(new Scene("testmap.tmx"));
}

void SceneManager::SetCurrentScene(std::string sceneID)
{
	for (Scene* s : scenes) if (s->id == sceneID) currentScene = s;
}

bool SceneManager::PreUpdate()
{
	if (currentScene == nullptr) return true;
	return currentScene->PreUpdate();
}

bool SceneManager::Update(float dt)
{
	if (currentScene == nullptr) return true;
	return currentScene->Update(dt);
}

bool SceneManager::PostUpdate(float dt)
{
	if (currentScene == nullptr) return true;
	return currentScene->PostUpdate(dt);
}
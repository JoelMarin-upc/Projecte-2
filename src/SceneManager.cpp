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
	SetCurrentScene("intro");
	//SetCurrentScene("main menu");
	//SetCurrentScene("SC-001");
	return true;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	return true;
}

void SceneManager::LoadScenes()
{
	//load from xml
	scenes = std::list<Scene*>();
	scenes.push_back(new Scene("intro", "", ""));
	scenes.push_back(new Scene("main menu", "", ""));
	
	scenes.push_back(new Scene("SC-002", mapsPath, "GroceriesShop.tmx"));
	scenes.push_back(new Scene("SC-003", mapsPath, "TutorialDungeon.tmx"));
	scenes.push_back(new Scene("SC-001", mapsPath, "RebelRefuge.tmx"));
}

void SceneManager::SetCurrentScene(std::string sceneID, std::string spawnId)
{
	for (Scene* s : scenes) if (s->id == sceneID) currentScene = s;
	if (currentScene) currentScene->Start(spawnId);
}

EntityManager* SceneManager::GetEntityManager() const
{
	return currentScene->entityManager;
}

MissionManager* SceneManager::GetMissionManager() const
{
	return currentScene->missionManager;
}

DialogManager* SceneManager::GetDialogManager() const
{
	return currentScene->dialogManager;
}

bool SceneManager::PreUpdate()
{
	if (currentScene == nullptr || paused) return true;
	return currentScene->PreUpdate();
}

bool SceneManager::Update(float dt)
{
	if (currentScene == nullptr || paused) return true;
	return currentScene->Update(dt);
}

bool SceneManager::PostUpdate(float dt)
{
	if (currentScene == nullptr || paused) return true;
	return currentScene->PostUpdate(dt);
}
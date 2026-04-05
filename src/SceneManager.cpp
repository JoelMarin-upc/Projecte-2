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
	pendingSceneID = "intro";
	pendingSpawnId = "";
	hasQueuedTransition = true;
	DoTransition();

	return true;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	return true;
}

void SceneManager::LoadScenes()
{
	sceneInfos.push_back({ "intro",     "",        "" });
	sceneInfos.push_back({ "main menu", "",        "" });
	sceneInfos.push_back({ "SC-001",    mapsPath,  "RebelRefuge.tmx" });
	sceneInfos.push_back({ "SC-002",    mapsPath,  "GroceriesShop.tmx" });
	sceneInfos.push_back({ "SC-003",    mapsPath,  "TutorialDungeon.tmx" });
}

void SceneManager::SetCurrentScene(std::string sceneID, std::string spawnId)
{
	pendingSceneID = sceneID;
	pendingSpawnId = spawnId;
	hasQueuedTransition = true;
}

void SceneManager::DoTransition()
{
	if (currentScene) {
		currentScene->CleanUp();
		delete currentScene;
		currentScene = nullptr;
	}

	Engine::GetInstance().physics->ResetPhysicsWorld();
	Engine::GetInstance().menuManager->HideMenu();

	for (const SceneInfo& info : sceneInfos) {
		if (info.id == pendingSceneID) {
			currentScene = new Scene(info.id, info.mapPath, info.mapName);
			currentScene->Awake();
			currentScene->Start(pendingSpawnId);
			hasQueuedTransition = false;
			return;
		}
	}
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

	if (hasQueuedTransition) {
		DoTransition();
	}

	return currentScene->PostUpdate(dt);
}
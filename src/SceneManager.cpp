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
	//Store the data of all scenes in the game, but don't load them yet
	LoadScenes();

	//Queues the intro scene and executes the transition to it
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
	//Registers and stores the id and map file of all scenes�
	sceneInfos.push_back({ "intro", "", "" });
	sceneInfos.push_back({ "main menu", "", "" });
	sceneInfos.push_back({ "game title", "", "" });
	sceneInfos.push_back({ "SC-001", mapsPath, "RebelRefuge.tmx" });
	sceneInfos.push_back({ "SC-002", mapsPath, "GroceriesShop.tmx" });
	sceneInfos.push_back({ "SC-003", mapsPath, "TutorialDungeon.tmx", "TutorialDungeon_combat.tmx" });
	sceneInfos.push_back({ "SC-004", mapsPath, "SecondDungeon.tmx", "TutorialDungeon_combat.tmx" });
}

void SceneManager::SetCurrentScene(std::string sceneID, std::string spawnId)
{
	//Queues a transition but doesn't do it immediately, to make sure the transition is done at the end of the frame to not cause read access violations
	pendingSceneID = sceneID;
	pendingSpawnId = spawnId;
	hasQueuedTransition = true;
}

void SceneManager::DoTransition()
{
	//Performs the actual transition, ONLY call at Start() or PostUpdate() to not call it between frames

	//First clean up the current scene
	if (currentScene) {
		/*if (currentScene->GetGameStarted()) {
			currentScene->SaveDialogState();
		}*/
		currentScene->CleanUp();
		delete currentScene;
		currentScene = nullptr;
	}

	//Destroy all colliders from the current scene
	Engine::GetInstance().physics->ResetPhysicsWorld();
	Engine::GetInstance().physics->ClearPendingDeletes();
	//Hide open menus
	Engine::GetInstance().menuManager->HideMenu();

	//Find the scene you want to transition and create it, spawns the player at the request spawn point and releases the queued transition
	for (const SceneInfo& info : sceneInfos) {
		if (info.id == pendingSceneID) {
			currentScene = new Scene(info.id, info.mapPath, info.mapName, info.combatMapName);
			currentScene->Awake();
			currentScene->Start(pendingSpawnId);
			hasQueuedTransition = false;
			return;
		}
	}
}

void SceneManager::EnterDungeon(std::string spawnId)
{
	if (dungeonLevel == 1) {
		SetCurrentScene("SC-003", spawnId);
	}
	else if (dungeonLevel == 2) {
		SetCurrentScene("SC-004", spawnId);
	}	
}

Scene* SceneManager::GetCurrentScene() const { 
	return currentScene; 
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

	if (!currentScene->GetIsOnDialog()) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
			SetCurrentScene("SC-001", "default");
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
			SetCurrentScene("SC-002", "shop_from_refuge");
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
			SetCurrentScene("SC-003", "dungeon_from_refuge");
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) {
			SetCurrentScene("SC-004", "dungeon_from_refuge");
		}
	}

	return currentScene->Update(dt);
}

bool SceneManager::PostUpdate(float dt)
{
	if (currentScene == nullptr || paused) return true;

	//Execute transition at the end of frame and delete current scene
	if (hasQueuedTransition) {
		DoTransition();
	}

	return currentScene->PostUpdate(dt);
}
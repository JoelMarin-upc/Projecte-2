#include "Scene.h"
#include "Log.h"
#include "Engine.h"

Scene::Scene(std::string mapName)
{
	id = "SC-001";
	name = "scene";
	map = new Map();
	entityManager = new EntityManager();
	missionManager = new MissionManager();
	dialogManager = new DialogManager();

	entityManager->CreateEntity("IT-001", EntityType::INTERACTABLE_ITEM);

	entityManager->CreateEntity("player", EntityType::PLAYER);
	entityManager->CreateEntity("CH-001", EntityType::NPC);

	//map->Load("base map path", mapName);
	/*entityManager->Load(entities);
	missionManager->Load(missions);
	dialogManager->Load(dialogs);*/
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	entityManager->Awake();
	missionManager->Awake();
	dialogManager->Awake();

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	entityManager->Start();
	missionManager->Start();
	dialogManager->Start();

	testItem = std::make_shared<InteractableItem>(InteractionType::TOGGLE);
	testItem->position.setX(500);
	testItem->position.setY(500);
	entityManager->AddEntity(testItem);
	testItem->Start();
	
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) StartDialog("CH-001");
	map->Update(dt);
	entityManager->Update(dt);
	missionManager->Update(dt);
	dialogManager->Update(dt);
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate(float dt)
{
	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Scene::TogglePause()
{
	paused = !paused;
	entityManager->paused = paused;
	//Engine::GetInstance().physics->paused = paused;
}

void Scene::SaveGame()
{
	
}

void Scene::LoadGame()
{
	
}

void Scene::LoadScene()
{
	// Load entities
}

void Scene::LoadMap()
{

}

void Scene::EndScene()
{
	
}

void Scene::StartDialog(std::string characterId)
{
	if (isOnDialog) return;
	if (!dialogManager->SetCurrentDialog(characterId)) return;
	isOnDialog = true;
	entityManager->paused = true;
}

void Scene::EndDialog()
{
	isOnDialog = false;
	entityManager->paused = false;
}

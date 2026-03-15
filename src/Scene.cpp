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
	entityManager->CreateEntity(EntityType::PLAYER);
	entityManager->CreateEntity(EntityType::NPC);
	entityManager->CreateEntity(EntityType::INTERACTABLE_ITEM);
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

	

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	testItem = std::make_shared<InteractableItem>(InteractionType::TOGGLE);
	testItem->position.setX(200);
	testItem->position.setY(200);
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
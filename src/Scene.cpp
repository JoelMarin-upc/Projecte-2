#include "Scene.h"
#include "Log.h"
#include "Engine.h"

Scene::Scene(std::string _id, std::string mapPath, std::string mapName)
{
	id = _id;
	name = "scene";
	
	entityManager = new EntityManager();
	missionManager = new MissionManager();
	dialogManager = new DialogManager();

	LoadMap(mapPath, mapName);
	LoadScene();

	entityManager->CreateEntity("player", EntityType::PLAYER);
	entityManager->CreateEntity("CH-001", EntityType::NPC);
	entityManager->CreateEntity("enemy", EntityType::ENEMY_GROUND);

	//entityManager->CreateEntity("IT-001", EntityType::INTERACTABLE_ITEM);

	//entityManager->CreateEntity("player", EntityType::PLAYER);
	//entityManager->CreateEntity("CH-001", EntityType::NPC);

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

	/*testItem = std::make_shared<InteractableItem>(InteractionType::TOGGLE);
	testItem->position.setX(500);
	testItem->position.setY(500);
	entityManager->AddEntity(testItem);
	testItem->Start();*/
	
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

void Scene::LoadMap(std::string mapPath, std::string mapName)
{
	map = new Map();
	map->Load(mapPath, mapName);
	mapData = map->gameData;
}

void Scene::LoadScene()
{
	std::string baseTexturePath = "Assets/Textures/";

	pugi::xml_document charactersDoc = XMLHandler::LoadFile("Assets/Entities/characters.xml");
	pugi::xml_node characters = charactersDoc.child("characters");
	pugi::xml_document itemsDoc = XMLHandler::LoadFile("Assets/Entities/items.xml");
	pugi::xml_node items = itemsDoc.child("items");

	pugi::xml_node pNode = characters.child("player");
	std::string id = pNode.attribute("id").as_string();
	std::string name = pNode.attribute("name").as_string();
	std::string texture = pNode.attribute("texture").as_string();
	player = std::dynamic_pointer_cast<Player>(entityManager->CreateEntity(id, name, baseTexturePath + texture, mapData.playerStartPosition, EntityType::PLAYER));

	for (NPCData npc : mapData.npcs) {
		for (pugi::xml_node cNode = characters.child("character"); cNode != NULL; cNode = cNode.next_sibling("character")) {
			if (cNode.attribute("id").as_string() != npc.id) continue;
			std::string name = cNode.attribute("name").as_string();
			std::string texture = cNode.attribute("texture").as_string();
			int type = cNode.attribute("type").as_int();
			entityManager->CreateEntity(npc.id, name, baseTexturePath + texture, npc.position, (EntityType)type);
		}
	}

	for (ItemData item : mapData.items) {
		for (pugi::xml_node iNode = items.child("item"); iNode != NULL; iNode = iNode.next_sibling("item")) {
			if (iNode.attribute("id").as_string() != item.id) continue;
			std::string name = iNode.attribute("name").as_string();
			std::string texture = iNode.attribute("texture").as_string();
			int type = iNode.attribute("type").as_int();
			int interactionType = iNode.attribute("interactionType").as_int();
			entityManager->CreateEntity(item.id, name, baseTexturePath + texture, item.position, (EntityType)type, (InteractionType)interactionType);
		}
	}
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

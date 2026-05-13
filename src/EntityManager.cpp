#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Item.h"
#include "NPC.h"
#include "InteractableItem.h"
#include "DungeonLever.h"
#include "DungeonExit.h"
#include "DungeonGate.h"
#include "PressurePlate.h"

EntityManager::EntityManager() : Module()
{
	name = "entitymanager";
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake()
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	/*for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Start();
	}*/

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;

	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->CleanUp();
	}

	entities.clear();

	return ret;
}

std::shared_ptr<Entity> EntityManager::CreateEntity(std::string id, std::string name, std::string texturePath, Vector2D position, EntityType type, ItemInteractionType interactionType, NPCInteractionType npcInteractionType)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();

	//L04: TODO 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	/*
	case EntityType::PLAYER:
		entity = std::make_shared<Player>(id, name, texturePath);
		break;
	case EntityType::NPC:
		entity = std::make_shared<NPC>(id, name, texturePath, npcInteractionType);
		break;
	case EntityType::INTERACTABLE_ITEM:
		entity = std::make_shared<InteractableItem>(id, name, texturePath, interactionType);
		break;
	*/
	default:
		break;
	}

	entities.push_back(entity);

	entity->position = position;

	entity->Start();

	return entity;
}

std::shared_ptr<Entity> EntityManager::CreateItem(std::string id, std::string name, std::string description, std::string texturePath, Vector2D position, std::string itemClass, EntityType type, ItemInteractionType interactionType, bool canStack, std::string toggledTexturePath, GearSlot slot)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();

	//L04: TODO 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::INTERACTABLE_ITEM:
		if (itemClass == "weapon") entity = std::make_shared<Weapon>(id, name, description, texturePath, canStack);
		else if (itemClass == "gear") entity = std::make_shared<Gear>(id, name, description, texturePath, slot, canStack);
		else if (itemClass == "consumable") entity = std::make_shared<Consumable>(id, name, description, texturePath, canStack);
		else if (itemClass == "lever") entity = std::make_shared<DungeonLever>(id, name, description, texturePath, toggledTexturePath);
		else if (itemClass == "exit") entity = std::make_shared<DungeonExit>(id, name, description, texturePath);
		else if (itemClass == "gate") entity = std::make_shared<DungeonGate>(id, name, description, texturePath);
		else if (itemClass == "plate") entity = std::make_shared<PressurePlate>(id, name, description, texturePath);
		else entity = std::make_shared<InteractableItem>(id, name, description, texturePath, interactionType, canStack, toggledTexturePath);
		break;
	default:
		break;
	}

	entities.push_back(entity);

	entity->position = position;

	entity->Start();

	return entity;
}

std::shared_ptr<Entity> EntityManager::CreateCharacter(std::string id, std::string name, std::string texturePath, std::string combatTexturePath, Vector2D position, EntityType type, NPCInteractionType npcInteractionType, std::string recuitMissionId)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();

	//L04: TODO 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
		entity = std::make_shared<Player>(id, name, texturePath, combatTexturePath);
		break;
	case EntityType::NPC:
		entity = std::make_shared<NPC>(id, name, texturePath, combatTexturePath, npcInteractionType, recuitMissionId);
		break;
	case EntityType::ENEMY:
		entity = std::make_shared<Enemy>(id, name, texturePath, combatTexturePath);
		break;
	default:
		break;
	}

	entities.push_back(entity);

	entity->position = position;

	entity->Start();

	return entity;
}

void EntityManager::DestroyEntity(std::shared_ptr<Entity> entity)
{
	entity->CleanUp();
	entities.remove(entity);
}

void EntityManager::AddEntity(std::shared_ptr<Entity> entity)
{
	if ( entity != nullptr) entities.push_back(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		if (!paused) ret = entity->Update(dt);
		entity->Draw(dt);
	}
	return ret;
}
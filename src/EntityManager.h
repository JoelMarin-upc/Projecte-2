#pragma once

#include "Module.h"
#include "Entity.h"
#include "InteractableItem.h"
#include "NPC.h"
#include <list>

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods

	std::shared_ptr<Entity> CreateEntity(std::string id, std::string name, std::string texturePath, Vector2D position, EntityType type, ItemInteractionType interactionType = ItemInteractionType::DEFAULT, NPCInteractionType npcInteractionType = NPCInteractionType::DEFAULT);

	std::shared_ptr<Entity> CreateItem(std::string id, std::string name, std::string description, std::string texturePath, Vector2D position, std::string itemClass, EntityType type, ItemInteractionType interactionType = ItemInteractionType::DEFAULT, bool canStack = false, std::string toggledTexturePath = "", GearSlot slot = GearSlot::BODY);

	std::shared_ptr<Entity> CreateCharacter(std::string id, std::string name, std::string texturePath, std::string combatTexturePath, Vector2D position, EntityType type, NPCInteractionType npcInteractionType = NPCInteractionType::DEFAULT);

	void DestroyEntity(std::shared_ptr<Entity> entity);

	void AddEntity(std::shared_ptr<Entity> entity);

	template<typename T>
	inline std::vector<std::shared_ptr<T>> GetEntities()
	{
		std::vector<std::shared_ptr<T>> result;

		for (const auto& e : entities)
			if (auto casted = std::dynamic_pointer_cast<T>(e))
				result.push_back(casted);

		return result;
	}

public:

	std::list<std::shared_ptr<Entity>> entities;
	bool paused = false;

};

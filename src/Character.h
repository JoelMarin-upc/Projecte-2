#pragma once

#include "DynamicEntity.h"
#include "Consumable.h"
#include "Inventory.h"
#include "Party.h"
#include "EnemyParty.h"

enum Stance {
	REST,
	DEFEND,
	CONCENTRATE,
	ASSIST,
	NO_STANCE
};

class Character : public DynamicEntity {
public:
	Character() {}
	Character(std::string id, std::string name, std::string texturePath, std::string combatTexturePath, EntityType type) : DynamicEntity(id, name, texturePath, combatTexturePath, type) {}

	float Attack();
	std::shared_ptr<Consumable> UseConsumable(std::string type);
	void TakeConsumable(std::shared_ptr<Consumable> consumable);
	void TakeStance(Stance stance, std::vector<std::shared_ptr<Character>> affectedCharacters = std::vector<std::shared_ptr<Character>>());
	bool TakeDamage(float damage);
	void CheckModifiers();
	void DrawHealthBar(const SDL_Rect& rect);
	void DrawHealthBar(SDL_Texture* texture);

	Stats* stats = nullptr;
	Inventory* inventory = nullptr;
	SDL_Texture* combatTexture = nullptr;
	bool isDead = false;
	bool hasFled = false;

	int walkFxId;
	int attackFxId;
	int dieFxId;

	Timer walkTimer;
	float walkMS = 400.0f;
};
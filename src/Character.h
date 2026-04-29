#pragma once

#include "DynamicEntity.h"
#include "Consumable.h"
#include "Inventory.h"

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
	Character(std::string id, std::string name, std::string texturePath, EntityType type) : DynamicEntity(id, name, texturePath, type) {}

	float Attack();
	std::shared_ptr<Consumable> UseConsumable(std::string type);
	void TakeConsumable(std::shared_ptr<Consumable> consumable);
	void TakeStance(Stance stance);
	bool TakeDamage(float damage);
	void CheckModifiers();
	void DrawHealthBar(SDL_Texture* texture);

	Stats* stats = nullptr;
	Inventory* inventory = nullptr;
	bool isDead = false;
	bool hasFled = false;
};
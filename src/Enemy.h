#pragma once

#include "AICharacter.h"
#include "EnemyParty.h"

class Enemy : public AICharacter {
public:
	Enemy(std::string id, std::string name, std::string texturePath) : AICharacter(id, name, texturePath, EntityType::ENEMY) {}
	virtual ~Enemy();

	bool Awake() override;
	bool Start() override;

	bool Update(float dt);
	void Draw(float dt);

	std::vector<std::string> GetActions();

	EnemyParty* party = nullptr;
};
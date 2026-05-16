#pragma once
#include "AICharacter.h"
#include "EnemyParty.h"
#include "Pathfinding.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>

enum class EnemyState {
	IDLE,
	CHASING
};

class Enemy : public AICharacter {
public:
	Enemy(std::string id, std::string name, std::string texturePath, std::string combatTexturePath) : AICharacter(id, name, texturePath, combatTexturePath, EntityType::ENEMY) {}
	virtual ~Enemy();

	Vector2D GetPosition();
	//void SetMap(Map* m) {map = m;}
	//void OnCollision(Collider* physA, Collider* physB);
	//void OnCollisionEnd(Collider* physA, Collider* physB);
private:
	void PerformPathfinding();
	void GetPhysicsValues();
	void Move(const Vector2D& target);
	void UpdateState(float dt);
	void ApplyPhysics();
	float DistanceTo(const Vector2D& v) const;

public: 
	float speed = 2.0f;
	std::shared_ptr<Pathfinding> pathfinding;
	std::vector<Vector2D> currentPath;
	SDL_Texture* texture;
	bool isActive = true;
	bool isDead = false;
	Collider* enemyBody = nullptr;
	Vector2D lastPlayerTile;
	Vector2D lastEnemyTile;
	Vector2D currentTarget;
	Map* map = nullptr;

	std::vector<std::string> GetActions();

	EnemyParty* party = nullptr;

private: 
	b2Vec2 velocity;
	EnemyState state = EnemyState::IDLE;
	float chaseDistance = 300.0f;

	bool Awake() override;
	bool Start() override;

	bool Update(float dt);
	void Draw(float dt);

	bool CleanUp();

};
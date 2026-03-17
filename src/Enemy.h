#pragma once
#include "AICharacter.h"
#include "Pathfinding.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>

class Enemy : public AICharacter {
public:

	Enemy(std::string id);

	virtual ~Enemy();

	bool Awake();

	bool Start() override;

	bool Update(float dt);
	void Draw(float dt);

	bool CleanUp();

	//void OnCollision(Collider* physA, Collider* physB);
	//void OnCollisionEnd(Collider* physA, Collider* physB);
private:
	void GetPhysicsValues();
	void Move();
	void ApplyPhysics();

public: 
	float speed = 3.0f;
	std::shared_ptr<Pathfinding> pathfinding;
	SDL_Texture* texture;
	bool isActive = true;
	Collider* pbody = nullptr;

private: 
	b2Vec2 velocity;

};
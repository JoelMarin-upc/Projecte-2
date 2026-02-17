#pragma once

#include "Entity.h"
#include "Animation.h"
#include <SDL3/SDL.h>
#include "Timer.h"

struct SDL_Texture;

class Recharge : public Entity
{
public:

	Recharge();
	virtual ~Recharge();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void Draw(float dt);

	bool CleanUp();

	bool Destroy();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Respawn();

	void CreateBody();
public:

	bool isPicked = false;

	bool visible = true;

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	AnimationSet anims;
	Timer respawnTimer;
	float respawnMS = 5000.0f;

	//L08 TODO 4: Add a physics to an item
	PhysBody* pbody;
};

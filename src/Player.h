#pragma once

#include "Party.h"
#include "Character.h"
#include "Animation.h"
#include "Timer.h"
#include "Inventory.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>

constexpr auto PI = 3.14159265;
struct SDL_Texture;

class Player : public Character//, public std::enable_shared_from_this<Player>
{
public:

	Player(std::string id, std::string name, std::string texturePath, std::string combatTexturePath);
	
	virtual ~Player();

	bool Awake();

	bool Start() override;

	bool Update(float dt);
	void Draw(float dt);

	void LoadAnimations();

	bool CleanUp();

	void OnCollision(Collider* physA, Collider* physB);
	void OnCollisionEnd(Collider* physA, Collider* physB);

	Vector2D GetPosition();
	Vector2D GetPositionCenter();

	void AddPartyMember(std::shared_ptr<NPC> member, bool write = false);
	void GodMode();

private:
	void CheckTimers();
	void GetPhysicsValues();
	void Move();
	void ApplyPhysics();
	void HandleAnimations();

public:

	std::string animationsPath;
	std::string facing = "down";
	std::string currentAnimation = "";
	bool isFacingRight = false;

	//Declare player parameters
	float speed = 3.0f;

	enum FacingDirection {
		UP,
		LEFT,
		DOWN,
		RIGHT
	};
	FacingDirection currentFacingDirection;

	bool isActive = true;
	Collider* pbody = nullptr;
	
	bool draw = true;

	Party* party;

private:
	b2Vec2 velocity;
	AnimationSet anims;
	int auxX, auxY;

};
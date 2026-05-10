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

	const char* textureDamagedPath;
	const char* itemChargeTexture0Path;
	const char* itemChargeTexture1Path;
	const char* itemChargeTexture2Path;

	//Declare player parameters
	float speed = 3.0f;
	SDL_Texture* texture;
	SDL_Texture* textureDamaged = NULL;
	SDL_Texture* itemChargeTexture0 = NULL;
	SDL_Texture* itemChargeTexture1 = NULL;
	SDL_Texture* itemChargeTexture2 = NULL;
	enum FacingDirection {
		UP,
		LEFT,
		DOWN,
		RIGHT
	};
	FacingDirection currentFacingDirection;

	//Audio fx
	int jumpFxId;
	int dashFxId;
	int walkFxId;
	int throwFxId;
	int deathFxId;
	int spawnFxId;
	int nextlevelFxId;
	int autosaveFxId;
	int lifeFxId;
	int coinFxId;
	int keyFxId;
	int doorFxId;
	int rechargeFxId;
	int itemFxId;
	int damageFxId;

	bool isActive = true;
	Timer deathTimer;
	Timer coyoteTimer;
	float coyoteMS = 200.0f;
	Collider* pbody = nullptr;
	float jumpForce = 0.9f; // The force to apply when jumping
	bool isJumping = false; // Flag to check if the player is currently jumping
	float dashForce = 1.f; // The force to apply when dashing
	bool dashed1 = false;
	bool isDashing = false; // Flag to check if the player is currently dashing
	bool dashed2 = false;
	Timer dashTimer; // Dash duration until movement stops
	float dashMS = 300.0f;
	bool canThrow1 = true; // Flag to see if the player can throw the object for dashing
	bool isThrow1 = false;
	bool throwCDBack1 = false;
	Timer throwTimer1; // Throw duration until object is destroyed
	bool canThrow2 = true; // Flag to see if the player can throw the object for dashing
	bool isThrow2 = false;
	bool throwCDBack2 = false;
	Timer throwTimer2; // Throw duration until object is destroyed
	float throwMS = 1000.0f;
	float spearOffset = 25.0f;
	bool spearCol1 = false;
	bool spearCol2 = false;
	float godModeSpeed = 0.1f;
	float deathMS = 600.0f;
	Timer walkTimer;
	float walkMS = 400.0f;
	Vector2D respawnPos;
	Vector2D newRespawnPos;
	bool hasRespawn = false;
	const int damageMS = 1000.f;
	Timer damageTimer;
	bool damaged = false;
	const int maxLives = 3;
	int lives;
	bool hasLivesSet = false;
	int coins = 0;
	bool hasItem1 = false;
	bool hasItem2 = false;
	std::vector<b2Vec2>checkpointPositions0;
	std::vector<b2Vec2>checkpointPositions1;
	bool draw = true;
	bool inBoss = false;
	int keyCount = 0;
	bool doorOpen = false;

	Party* party;

private:
	b2Vec2 velocity;
	AnimationSet anims;
	int auxX, auxY;

};
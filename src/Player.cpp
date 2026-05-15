#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include <memory>

Player::Player(std::string id, std::string name, std::string texturePath, std::string combatTexturePath) : Character(id, name, texturePath, combatTexturePath, EntityType::PLAYER)
{
}

Player::~Player() {

}

bool Player::Awake() {

	
	return true;
}

bool Player::Start() {

	//if (!hasRespawn) respawnPos = *Engine::GetInstance().map->playerStartPos;

	// load
	//texture = Engine::GetInstance().textures->Load("Assets/Textures/goldCoin.png");
	//textureDamaged = Engine::GetInstance().textures->Load(textureDamagedPath);
	//std::unordered_map<int, std::string> aliases = { {0,"idle"},{24,"move"},{40,"jump"},{32,"fall"},{48,"death"},{64,"throw"},{45,"falling"}};
	//anims.LoadFromTSX(animationsPath, aliases);
	//texturePath = "Assets/Textures/goldCoin.png";
	texW = 32;
	texH = 64;

	texture = Engine::GetInstance().textures->Load(texturePath.c_str());
	if (combatTexturePath != "") combatTexture = Engine::GetInstance().textures->Load(combatTexturePath.c_str());

	//AddCollider(ColliderType::CIRCLE, texture, 0, 0, -110, 0, 1, 1);
	AddCollider(ColliderType::SQUARE, texture, 0, 0, -110, -200, 1, 1);

	colliders[0]->etype = EntityType::PLAYER;
	pbody = colliders[0];
	pbody->listener = this;
	b2Body_SetFixedRotation(pbody->body, true);

	party = new Party(std::static_pointer_cast<Player>(shared_from_this()));

	return true;
}

bool Player::Update(float dt)
{
	GodMode();
	CheckTimers();
	if (isActive) {
		GetPhysicsValues();
		Move();
		ApplyPhysics();
		HandleAnimations();
		Draw(dt);
	}

	return true;
}

void Player::GodMode()
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
		if (godMode)
		{
			//b2Body_Disable(colliders[0]->body);
			speed = 30;
		}
		else
		{
			//b2Body_Enable(colliders[0]->body);
			speed = 3;
		}
	}
}

void Player::CheckTimers() {
	
}

void Player::GetPhysicsValues() {
	// Read current velocity
	velocity = Engine::GetInstance().physics->GetLinearVelocity(colliders[0]);
	velocity = { 0, velocity.y };
}

void Player::Move() {
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || Engine::GetInstance().input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity.y = -speed;
		currentFacingDirection = UP;
		if (!isJumping && walkTimer.ReadMSec() > walkMS) {
			Engine::GetInstance().audio->PlayFx(walkFxId);
			walkTimer = Timer();
		}
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || Engine::GetInstance().input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -speed;
		currentFacingDirection = LEFT;
		if (!isJumping && walkTimer.ReadMSec() > walkMS) {
			Engine::GetInstance().audio->PlayFx(walkFxId);
			walkTimer = Timer();
		}
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || Engine::GetInstance().input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity.y = speed;
		currentFacingDirection = DOWN;
		if (!isJumping && walkTimer.ReadMSec() > walkMS) {
			Engine::GetInstance().audio->PlayFx(walkFxId);
			walkTimer = Timer();
		}
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || Engine::GetInstance().input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speed;
		currentFacingDirection = RIGHT;
		if (!isJumping && walkTimer.ReadMSec() > walkMS) {
			Engine::GetInstance().audio->PlayFx(walkFxId);
			walkTimer = Timer();
		}
	}
	if ((Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) != KEY_REPEAT &&
		Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT) &&
		(Engine::GetInstance().input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT &&
			Engine::GetInstance().input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)) {
		velocity.y = 0;
	}
	if ((Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) != KEY_REPEAT &&
		Engine::GetInstance().input->GetKey(SDL_SCANCODE_RIGHT) != KEY_REPEAT) &&
		(Engine::GetInstance().input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT &&
			Engine::GetInstance().input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)) {
		velocity.x = 0;
	}
}

void Player::ApplyPhysics() {
	// Preserve vertical speed while jumping
	if (isJumping == true || isDashing == true) {
		velocity.y = Engine::GetInstance().physics->GetYVelocity(colliders[0]);
	}

	if (isDashing == true) {
		velocity.x = Engine::GetInstance().physics->GetXVelocity(colliders[0]);
	}

	// Apply velocity via helper
	Engine::GetInstance().physics->SetLinearVelocity(colliders[0], velocity);
}

void Player::HandleAnimations()
{
	if (animationsPath.empty()) return;

	bool isMoving = (std::abs(velocity.x) > 0.1f || std::abs(velocity.y) > 0.1f);

	if (isMoving) {
		if (std::abs(velocity.x) > std::abs(velocity.y)) {
			facing = "left";                     
			isFacingRight = (velocity.x > 0);
		}
		else {
			facing = (velocity.y > 0) ? "down" : "up";
		}

		std::string animName = "move_" + facing;
		if (currentAnimation != animName) {
			anims.SetCurrent(animName);
			currentAnimation = animName;
		}
	}
	else {
		if (currentAnimation != "idle") {
			anims.SetCurrent("idle");
			currentAnimation = "idle";
		}
	}
}

void Player::AddPartyMember(std::shared_ptr<NPC> member, bool write)
{
	if (!party) party = new Party(std::static_pointer_cast<Player>(shared_from_this()));
	party->AddMember(member, write);
	Engine::GetInstance().physics->SetCollisionFilter(member->colliders[0], 0, 0);
}

void Player::Draw(float dt) {
	if (!draw) return;
	/*anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();*/

	int x, y;
	colliders[0]->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);
	SDL_Texture* tex = damaged ? textureDamaged : texture;

	if (!animationsPath.empty()) {
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();
		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, 1, &animFrame, isFacingRight);
		DrawHealthBar(animFrame);
	}

	/*if (!isActive) return;
	tex = itemChargeTexture0;
	if (hasItem2) {
		if (canThrow1 && canThrow2) tex = itemChargeTexture2;
		else if (canThrow1 || canThrow2) tex = itemChargeTexture1;
	}
	else if (hasItem1 && canThrow1) tex = itemChargeTexture1;
	Engine::GetInstance().render->DrawTexture(tex, x - 8, y -8);*/
}

void Player::LoadAnimations()
{
	if (animationsPath.empty()) return;
	std::unordered_map<int, std::string> aliases = {
		{0, "idle"}, {4, "move_down"}, {8, "move_up"}, {12, "move_left"}
	};
	anims.LoadFromTSX(animationsPath.c_str(), aliases);
	anims.SetCurrent("idle");
	currentAnimation = "idle";
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures->UnLoad(texture);
	for (const auto& collider : colliders) Engine::GetInstance().physics->DestroyBody(collider);
	return true;
}

void Player::OnCollision(Collider* physA, Collider* physB) {
	switch (physB->etype) {
	case EntityType::INTERACTABLE_ITEM:
		LOG("Player is in range of interadctable item");
		break;
	case EntityType::ENEMY:
		Enemy* enemy = static_cast<Enemy*>(physB->listener);
		Engine::GetInstance().sceneManager->currentScene->StartCombat(std::static_pointer_cast<Enemy>(enemy->shared_from_this()));
		break;
	}
}

void Player::OnCollisionEnd(Collider* physA, Collider* physB)
{
	
}

Vector2D Player::GetPosition() {
	int x, y;
	colliders[0]->GetPosition(x, y);
	// Adjust for center
	return Vector2D((float)x, (float)y);
}

Vector2D Player::GetPositionCenter() {
	int x, y;
	colliders[0]->GetPosition(x, y);
	// Adjust for center
	return Vector2D((float)x + texW / 2, (float)y + texH / 2);
}


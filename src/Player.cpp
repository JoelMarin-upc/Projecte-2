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
#include "tracy/Tracy.hpp"

Player::Player(SDL_Texture tex) : Character()
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	
	return true;
}

bool Player::Start() {

	//if (!hasRespawn) respawnPos = *Engine::GetInstance().map->playerStartPos;

	// load
	texture = Engine::GetInstance().textures->Load(texturePath);
	textureDamaged = Engine::GetInstance().textures->Load(textureDamagedPath);
	std::unordered_map<int, std::string> aliases = { {0,"idle"},{24,"move"},{40,"jump"},{32,"fall"},{48,"death"},{64,"throw"},{45,"falling"}};
	anims.LoadFromTSX(animationsPath, aliases);

	texW = 32;
	texH = 32;

	itemChargeTexture0 = Engine::GetInstance().textures->Load(itemChargeTexture0Path);
	itemChargeTexture1 = Engine::GetInstance().textures->Load(itemChargeTexture1Path);
	itemChargeTexture2 = Engine::GetInstance().textures->Load(itemChargeTexture2Path);

	//initialize audio effect
	jumpFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("jump").as_string());
	dashFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("dash").as_string());
	walkFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("walk").as_string());
	throwFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("throw").as_string());
	deathFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("death").as_string());
	spawnFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("spawn").as_string());
	nextlevelFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("nextlevel").as_string());
	autosaveFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("autosave").as_string());
	lifeFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("life").as_string());
	coinFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("coin").as_string());
	keyFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("key").as_string());
	doorFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("door").as_string());
	rechargeFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("recharge").as_string());
	itemFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("item").as_string());
	damageFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("damage").as_string());

	checkpointPositions0 = std::vector<b2Vec2>(0);
	checkpointPositions1 = std::vector<b2Vec2>(0);
	return true;
}

bool Player::Update(float dt)
{
	ZoneScoped;
	GodMode();
	CheckTimers();
	if (isActive) {
		GetPhysicsValues();
		Move();
		ApplyPhysics();
		HandleAnimations();
	}

	return true;
}

void Player::GodMode()
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
		if (godMode) b2Body_Disable(pbody->body);
		else b2Body_Enable(pbody->body);
	}
}

void Player::CheckTimers() {
	
}

void Player::GetPhysicsValues() {
	// Read current velocity
	velocity = Engine::GetInstance().physics->GetLinearVelocity(pbody);
	velocity = { 0, velocity.y };
}

void Player::Move() {
	// Move left/right
	if (godMode) {
		b2Transform t = Engine::GetInstance().physics->GetTransform(pbody);
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			Engine::GetInstance().physics->MoveBody(pbody, b2Vec2{ t.p.x - godModeSpeed, t.p.y }, t.q);
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			Engine::GetInstance().physics->MoveBody(pbody, b2Vec2{ t.p.x + godModeSpeed, t.p.y }, t.q);
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
			Engine::GetInstance().physics->MoveBody(pbody, b2Vec2{ t.p.x, t.p.y - godModeSpeed }, t.q);
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			Engine::GetInstance().physics->MoveBody(pbody, b2Vec2{ t.p.x, t.p.y + godModeSpeed }, t.q);
		}
	}
	else {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			velocity.x = -speed;
			facingRight = false;
			if (!isJumping && walkTimer.ReadMSec() > walkMS) {
				Engine::GetInstance().audio->PlayFx(walkFxId);
				walkTimer = Timer();
			}
		}
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				velocity.x = speed;
				facingRight = true;
				if (!isJumping && walkTimer.ReadMSec() > walkMS) {
					Engine::GetInstance().audio->PlayFx(walkFxId);
					walkTimer = Timer();
				}
			}
	}
}

void Player::ApplyPhysics() {
	// Preserve vertical speed while jumping
	if (isJumping == true || isDashing == true) {
		velocity.y = Engine::GetInstance().physics->GetYVelocity(pbody);
	}

	if (isDashing == true) {
		velocity.x = Engine::GetInstance().physics->GetXVelocity(pbody);
	}

	// Apply velocity via helper
	Engine::GetInstance().physics->SetLinearVelocity(pbody, velocity);
}

void Player::HandleAnimations()
{
	if (isThrow1 && isThrow2) return;

	if (!isJumping) {
		if (abs(velocity.x) > 0.2) {
			if (currentAnimation != "move") anims.SetCurrent("move");
			currentAnimation = "move";
		}
		else {
			if (currentAnimation != "idle") anims.SetCurrent("idle");
			currentAnimation = "idle";
		}

	}
	else if (velocity.y > 0.2) {
		anims.SetCurrent("falling");
		currentAnimation = "falling";
	}
}

void Player::Draw(float dt) {
	ZoneScoped;
	if (!draw) return;
	anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);
	SDL_Texture* tex = damaged ? textureDamaged : texture;
	Engine::GetInstance().render->DrawTexture(tex, x - texW / 2, y - 6 - texH / 2, &animFrame, facingRight);

	if (!isActive) return;
	tex = itemChargeTexture0;
	if (hasItem2) {
		if (canThrow1 && canThrow2) tex = itemChargeTexture2;
		else if (canThrow1 || canThrow2) tex = itemChargeTexture1;
	}
	else if (hasItem1 && canThrow1) tex = itemChargeTexture1;
	Engine::GetInstance().render->DrawTexture(tex, x - 8, y -8);
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

void Player::OnCollision(Collider* physA, Collider* physB) {
	
}

void Player::OnCollisionEnd(Collider* physA, Collider* physB)
{
	
}

Vector2D Player::GetPosition() {
	int x, y;
	pbody->GetPosition(x, y);
	// Adjust for center
	return Vector2D((float)x, (float)y);
}

Vector2D Player::GetPositionCenter() {
	int x, y;
	pbody->GetPosition(x, y);
	// Adjust for center
	return Vector2D((float)x + texW / 2, (float)y + texH / 2);
}


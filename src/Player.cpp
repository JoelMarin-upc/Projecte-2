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

Player::Player(std::string id) : Character(id, EntityType::PLAYER)
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
	//texture = Engine::GetInstance().textures->Load("Assets/Textures/goldCoin.png");
	//textureDamaged = Engine::GetInstance().textures->Load(textureDamagedPath);
	//std::unordered_map<int, std::string> aliases = { {0,"idle"},{24,"move"},{40,"jump"},{32,"fall"},{48,"death"},{64,"throw"},{45,"falling"}};
	//anims.LoadFromTSX(animationsPath, aliases);
	texturePath = "Assets/Textures/goldCoin.png";
	texture = Engine::GetInstance().textures->Load(texturePath);
	AddCollider(ColliderType::CIRCLE, texture, 0, 0, 0, 0, 1, 1);

	colliders[0]->etype = EntityType::PLAYER;
	pbody = colliders[0];
	pbody->listener = this;

	texW = 32;
	texH = 32;

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
		if (godMode) b2Body_Disable(colliders[0]->body);
		else b2Body_Enable(colliders[0]->body);
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
	// Move left/right
	if (godMode) {
		b2Transform t = Engine::GetInstance().physics->GetTransform(colliders[0]);
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			Engine::GetInstance().physics->MoveBody(colliders[0], b2Vec2{ t.p.x - godModeSpeed, t.p.y }, t.q);
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			Engine::GetInstance().physics->MoveBody(colliders[0], b2Vec2{ t.p.x + godModeSpeed, t.p.y }, t.q);
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
			Engine::GetInstance().physics->MoveBody(colliders[0], b2Vec2{ t.p.x, t.p.y - godModeSpeed }, t.q);
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			Engine::GetInstance().physics->MoveBody(colliders[0], b2Vec2{ t.p.x, t.p.y + godModeSpeed }, t.q);
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
	if (!draw) return;
	/*anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();*/

	int x, y;
	colliders[0]->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);
	SDL_Texture* tex = damaged ? textureDamaged : texture;
	Engine::GetInstance().render->DrawTexture(tex, x - texW / 2, y - 6 - texH / 2/*, &animFrame, facingRight*/);

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
	switch (physB->etype) {
	case EntityType::INTERACTABLE_ITEM:
		LOG("Player is in range of interadctable item");
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


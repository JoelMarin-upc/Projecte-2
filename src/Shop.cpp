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

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	
	return true;
}

bool Player::Start() {

	if (!hasRespawn) respawnPos = *Engine::GetInstance().map->playerStartPos;

	// load
	texture = Engine::GetInstance().textures->Load(texturePath);
	textureDamaged = Engine::GetInstance().textures->Load(textureDamagedPath);
	std::unordered_map<int, std::string> aliases = { {0,"idle"},{24,"move"},{40,"jump"},{32,"fall"},{48,"death"},{64,"throw"},{45,"falling"}};
	anims.LoadFromTSX(animationsPath, aliases);

	texW = 32;
	texH = 32;
	Respawn();

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
		CheckGround();
		Move();
		Jump();
		CheckThrow();
		Dash();
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
	if (throwTimer1.ReadMSec() >= throwMS || spearCol1) {
		if (isThrow1) {
			isThrow1 = false;
			if (spear1->destroyed == false) spear1->Destroy();
			spearCol1 = false;
		}
	}
	if (throwTimer2.ReadMSec() >= throwMS || spearCol2) {
		if (isThrow2) {
			isThrow2 = false;
			if (spear2->destroyed == false) spear2->Destroy();
			spearCol2 = false;
		}
	}
	if (throwCDBack1 && !isThrow1) {
		canThrow1 = true;
		dashed1 = false;
		throwCDBack1 = false;
	}
	if (throwCDBack2 && !isThrow2) {
		canThrow2 = true;
		dashed2 = false;
		throwCDBack2 = false;
	}
	if (isDashing && dashTimer.ReadMSec() >= dashMS) {
		isDashing = false;
	}
	if (damaged && damageTimer.ReadMSec() >= damageMS) {
		damaged = false;
	}
	if (!isActive && deathTimer.ReadMSec() >= deathMS) {
		Engine::GetInstance().scene->ToggleDeathScreen();
		draw = false;
	}

}

void Player::CheckGround()
{
	float verticalVel = std::abs(velocity.y);
	if (verticalVel < 0.3)
	{
		int x, y;
		pbody->GetPosition(x, y);
		b2Vec2 feetPos{ x, y + 20 + texH / 2 };
		float _;
		int dist = pbody->RayCast(x, y, feetPos.x, feetPos.y, _, _);
		int distLeft = pbody->RayCast(x - texW / 2, y, feetPos.x - texW / 2, feetPos.y, _, _);
		int distRight = pbody->RayCast(x + texW / 2, y, feetPos.x + texW / 2, feetPos.y, _, _);
		if (dist != -1 || distLeft != -1 || distRight != -1) {
			isJumping = false;
			if (!canThrow1) throwCDBack1 = true;
			if (!canThrow2) throwCDBack2 = true;
		}
	}
	else {
		if (!isJumping) coyoteTimer.Start();
		isJumping = true;
	}
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

void Player::Jump() {
	if (godMode) return;
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (isJumping == false || coyoteTimer.ReadMSec() < coyoteMS)) {
		Engine::GetInstance().physics->SetLinearVelocity(pbody, { velocity.x,0 });
		Engine::GetInstance().physics->ApplyLinearImpulseToCenter(pbody, 0.0f, -jumpForce, true);
		anims.PlayOnce("jump");
		isJumping = true;
		Engine::GetInstance().audio->PlayFx(jumpFxId);
	}
}

void Player::CheckThrow() {
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		if (hasItem1 && canThrow1) {
			isThrow1 = true;
			canThrow1 = false;
			throwTimer1 = Timer();
			if (spear1!= NULL && spear1->active == true) spear1->Destroy();
			Throw(spear1);
		}
		else if (hasItem2 && canThrow2) {
			isThrow2 = true;
			canThrow2 = false;
			throwTimer2 = Timer();
			if (spear2 != NULL && spear2->active == true) spear2->Destroy();
			Throw(spear2);
		}
	}
}

void Player::Throw(std::shared_ptr<Spear>& spear) {
	spear = std::dynamic_pointer_cast<Spear>(Engine::GetInstance().entityManager->CreateEntity(EntityType::SPEAR));
	float angle;
	Vector2D initialPos;
	if (facingRight) {
		angle = 0;
		initialPos = Vector2D(spearOffset, 0);
	}
	else {
		angle = PI;
		initialPos = Vector2D(-spearOffset, 0);
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
			angle = PI / 4;
			initialPos = Vector2D(0, -spearOffset);
		}
		else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			angle = 7 * PI / 4;
			initialPos = Vector2D(spearOffset, 0);
		}
		else {
			angle = 0;
			initialPos = Vector2D(spearOffset, 0);
		}
	}
	else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
			angle = 3 * PI / 4;
			initialPos = Vector2D(0, -spearOffset);
		}
		else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			angle = 5 * PI / 4;
			initialPos = Vector2D(0, spearOffset);
		}
	}
	else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		angle = PI / 2;
		initialPos = Vector2D(0, -spearOffset);
	}
	else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		angle = 3 * PI / 2;
		initialPos = Vector2D(0, spearOffset);
	}
	anims.PlayOnce("throw");
	spear->position = position + initialPos + Vector2D{ (float)-texW / 4, (float)-texH / 2 };
	spear->Initialize(angle);
	Engine::GetInstance().audio->PlayFx(throwFxId);
}

void Player::Dash() {
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
		float x, y;
		if (dashed2 == false && isThrow2 == true) {
			if (spear2->destroyed == true) return;
			x = spear2->position.getX() - position.getX();
			y = spear2->position.getY() - position.getY();
			dashed1 = true;
			dashed2 = true;
		}
		else if (dashed1 == false && isThrow1 == true) {
			if (spear1->destroyed == true) return;
			x = spear1->position.getX() - position.getX();
			y = spear1->position.getY() - position.getY();
			dashed1 = true;
		}
		else return;
		float aux = sqrt(pow(x, 2) + pow(y, 2));
		x = x / aux;
		y = y / aux;
		Engine::GetInstance().physics->SetLinearVelocity(pbody,0, 0);
		Engine::GetInstance().physics->ApplyLinearImpulseToCenter(pbody, x * dashForce, y * dashForce, true);
		anims.PlayOnce("jump");
		isDashing = true;
		dashTimer = Timer();
		Engine::GetInstance().audio->PlayFx(dashFxId);
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

void Player::Damage()
{
	if (damaged) return;
	lives--;
	damaged = true;
	if (lives <= 0) Die();
	else {
		damageTimer.Start();
		Engine::GetInstance().audio->PlayFx(damageFxId);
	}
}

void Player::Die()
{
	anims.PlayOnce("death");
	isActive = false;
	deathTimer = Timer();
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = nullptr;
	pbody = Engine::GetInstance().physics->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::STATIC, 0x0002, 0xFFFF);
	Engine::GetInstance().audio->PlayFx(damageFxId);
	Engine::GetInstance().audio->PlayFx(deathFxId);
	inBoss = false;
	keyCount = 0;
}

void Player::Respawn() {
	isActive = true;
	draw = true;
	position = respawnPos;
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = nullptr;
	pbody = Engine::GetInstance().physics->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2.5, bodyType::DYNAMIC, 0x0002, 0xFFFF);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	Engine::GetInstance().audio->PlayFx(spawnFxId);
	if (!hasLivesSet) lives = maxLives;
	hasLivesSet = false;
	Engine::GetInstance().entityManager->aliveTentacles = Engine::GetInstance().entityManager->maxTentacles;
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

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		break;
	case ColliderType::ITEM:
		physB->listener->Destroy();
		if (!hasItem1) hasItem1 = true;
		else if (!hasItem2) hasItem2 = true;
		Engine::GetInstance().audio->PlayFx(itemFxId);
		break;
	case ColliderType::HEART:
		if (lives < maxLives)
		{
			physB->listener->Destroy();
			Engine::GetInstance().audio->PlayFx(lifeFxId);
			lives++;
		}
		break;
	case ColliderType::COIN:
		physB->listener->Destroy();
		Engine::GetInstance().audio->PlayFx(coinFxId);
		coins++;
		break;
	case ColliderType::RECHARGE:
		throwCDBack1 = true;
		canThrow1 = true;
		throwCDBack2 = true;
		canThrow2 = true;
		Engine::GetInstance().audio->PlayFx(rechargeFxId);
		break;
	case ColliderType::KEY:
		keyCount++;
		Engine::GetInstance().audio->PlayFx(keyFxId);
		break;
	case ColliderType::SPEAR:
		//spearCol1 = true;
		break;
	case ColliderType::DEATHZONE:
		Die();
		break;
	case ColliderType::ENEMY:
		Damage();
		break;
	case ColliderType::ATTACK:
		Damage();
		break;
	case ColliderType::RESPAWNPOINT:
		int x, y;
		physB->GetPosition(x,y);
		newRespawnPos = Vector2D( x, y );
		if (respawnPos != newRespawnPos) {
			respawnPos = newRespawnPos;
			Engine::GetInstance().audio->PlayFx(autosaveFxId);
			Engine::GetInstance().scene->SaveGame();
			//CheckCheckpointPositions;
			bool isIn = false;
			if (Engine::GetInstance().scene->currentScene == 0) {
				for (int i = 0; i < checkpointPositions0.size(); ++i) {
					if (checkpointPositions0[i].x == x && checkpointPositions0[i].y == y) {
						isIn = true;
						break;
					}
				}
				if (!isIn) {
					b2Vec2 newPos = { x,y };
					checkpointPositions0.emplace_back(newPos);
				}
			}
			else if (Engine::GetInstance().scene->currentScene == 1) {
				for (int i = 0; i < checkpointPositions1.size(); ++i) {
					if (checkpointPositions1[i].x == x && checkpointPositions1[i].y == y) {
						isIn = true;
						break;
					}
				}
				if (!isIn) {
					b2Vec2 newPos = { x,y };
					checkpointPositions1.emplace_back(newPos);
				}
			}
			
		}
		break;
	case ColliderType::NEXTLEVEL:
		Engine::GetInstance().audio->PlayFx(nextlevelFxId);
		Engine::GetInstance().scene->EndScene();
		break;
	case ColliderType::BOSSSTART:
		inBoss = true;
		Engine::GetInstance().scene->currentMusic = (char*)Engine::GetInstance().scene->configParameters.child("music").attribute("boss").as_string();
		Engine::GetInstance().audio->PlayMusic(Engine::GetInstance().scene->currentMusic.c_str());
		break;
	case ColliderType::DOOR:
		if (keyCount == 3) {
			doorOpen = true;
			b2DestroyBody(physB->body);
			Engine::GetInstance().map->DrawDoor();
			Engine::GetInstance().audio->PlayFx(doorFxId);
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("End Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("End Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	default:
		break;
	}
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


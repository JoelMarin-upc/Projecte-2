#include "Enemy.h"
#include "Textures.h"

Enemy::~Enemy()
{
}

bool Enemy::Awake()
{
    return true;
}

bool Enemy::Start()
{
    texture = Engine::GetInstance().textures->Load(texturePath.c_str());

    AddCollider(ColliderType::CIRCLE, texture, 0, 0, 0, 0, 1, 1);
    colliders[0]->etype = EntityType::ENEMY;
    colliders[0]->listener = this;

    party = new EnemyParty(std::static_pointer_cast<Enemy>(shared_from_this()));

    texW = 32;
    texH = 32;

    return true;
}

bool Enemy::Update(float dt)
{
    if (!active) return true;
    Draw(dt);
    return true;
}

void Enemy::Draw(float dt)
{
    int x, y;
    colliders[0]->GetPosition(x, y);
    position.setX((float)x);
    position.setY((float)y);
    Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - 6 - texH / 2/*, &animFrame, facingRight*/);
}

bool Enemy::CleanUp() {
    for (const auto& collider : colliders) Engine::GetInstance().physics->DestroyBody(collider);
    return true;
}
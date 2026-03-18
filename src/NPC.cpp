#include "NPC.h"

bool NPC::Start()
{
	//texturePath = "Assets/Textures/heart.png";
	texture = Engine::GetInstance().textures->Load(texturePath.c_str());
	AddCollider(ColliderType::CIRCLE, texture, 0, 0, 0, 0, 1, 1);

	texW = 32;
	texH = 32;
    return true;
}

bool NPC::Update(float dt)
{
    return true;
}

void NPC::Draw(float dt)
{
	int x, y;
	colliders[0]->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);
	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - 6 - texH / 2/*, &animFrame, facingRight*/);
}
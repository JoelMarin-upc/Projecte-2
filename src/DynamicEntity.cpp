#include "DynamicEntity.h"

void DynamicEntity::AddCollider(ColliderType type, SDL_Texture* tex, float xOffset, float yOffset, float wOffset, float hOffset, uint16_t categoryBits, uint16_t maskBits, int* points, int size)
{
	Collider* c = nullptr;
	switch (type)
	{
		case ColliderType::CIRCLE:
			c = Engine::GetInstance().physics->CreateCircle(position.getX() + xOffset, position.getY() + yOffset, tex->w + wOffset, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		case ColliderType::CIRCLE_SENSOR:
			c = Engine::GetInstance().physics->CreateCircleSensor(position.getX() + xOffset, position.getY() + yOffset, tex->w + wOffset, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		case ColliderType::SQUARE:
			c = Engine::GetInstance().physics->CreateRectangle(position.getX() + xOffset, position.getY() + yOffset, tex->w + wOffset, tex->h + hOffset, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		case ColliderType::SQUARE_SENSOR:
			c = Engine::GetInstance().physics->CreateRectangleSensor(position.getX() + xOffset, position.getY() + yOffset, tex->w + wOffset, tex->h + hOffset, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		case ColliderType::CHAIN:
			c = Engine::GetInstance().physics->CreateChain(position.getX() + xOffset, position.getY() + yOffset, points, size, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		default:
			break;
	}
	if (c) colliders.push_back(c);
}

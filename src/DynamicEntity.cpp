#include "DynamicEntity.h"

void DynamicEntity::AddCollider(ColliderType type, float xOffset, float yOffset, float wOffset, float hOffset, uint16_t categoryBits, uint16_t maskBits, int* points = nullptr, int size = 0)
{
	Collider* c = nullptr;
	switch (type)
	{
		case ColliderType::CIRCLE:
			c = Engine::GetInstance().physics->CreateCircle(position.getX() + xOffset, position.getY() + yOffset, texture.w + wOffset, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		case ColliderType::CIRCLE_SENSOR:
			c = Engine::GetInstance().physics->CreateCircleSensor(position.getX() + xOffset, position.getY() + yOffset, texture.w + wOffset, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		case ColliderType::SQUARE:
			c = Engine::GetInstance().physics->CreateRectangle(position.getX() + xOffset, position.getY() + yOffset, texture.w + wOffset, texture.h + hOffset, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		case ColliderType::SQUARE_SENSOR:
			c = Engine::GetInstance().physics->CreateRectangleSensor(position.getX() + xOffset, position.getY() + yOffset, texture.w + wOffset, texture.h + hOffset, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		case ColliderType::CHAIN:
			c = Engine::GetInstance().physics->CreateChain(position.getX() + xOffset, position.getY() + yOffset, points, size, bodyType::DYNAMIC, categoryBits, maskBits);
			break;
		default:
			break;
	}
	if (c) colliders.push_back(c);
}

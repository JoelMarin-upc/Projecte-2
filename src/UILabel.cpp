#include "UILabel.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

UILabel::UILabel(int id, SDL_Rect bounds, const char* text, SDL_Color color, SDL_Color colorDis) : UIElement(UIElementType::LABEL, id)
{
	this->bounds = bounds;
	this->text = text;
	this->color = color;
	this->colorDis = colorDis;

	drawBasic = false;
	state = UIElementState::NORMAL;
}

UILabel::~UILabel()
{

}

bool UILabel::Update(float dt)
{
	switch (state)
	{
	case UIElementState::DISABLED:
		Engine::GetInstance().render->DrawText(text.c_str(), bounds.x, bounds.y, bounds.w, bounds.h, colorDis);
		break;
	case UIElementState::NORMAL:
		Engine::GetInstance().render->DrawText(text.c_str(), bounds.x, bounds.y, bounds.w, bounds.h, color);
		break;
	}

	return false;
}
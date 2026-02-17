#pragma once

#include "UIElement.h"
#include "Vector2D.h"

class UILabel : public UIElement
{

public:

	UILabel(int id, SDL_Rect bounds, const char* text, SDL_Color color, SDL_Color colorDis);
	virtual ~UILabel();

	// Called each loop iteration
	bool Update(float dt);

private:

	bool drawBasic = false;
	SDL_Color color;
	SDL_Color colorDis;
};

#pragma once
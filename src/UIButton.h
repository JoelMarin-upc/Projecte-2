#pragma once

#include "UIElement.h"
#include "Vector2D.h"

class UIButton : public UIElement
{

public:

	UIButton(int id, SDL_Rect bounds, const char* text, int horizotalSpacing, int verticalSpacing, SDL_Color colorDef, SDL_Color colorDis, SDL_Color colorHov, SDL_Color colorPre, SDL_Color colorTxt, int hoverFxId, int clickFxId);
	virtual ~UIButton();

	// Called each loop iteration
	bool Update(float dt);

private:

	bool canClick = true;
	bool drawBasic = false;
	int horizotalSpacing;
	int verticalSpacing;
	SDL_Color colorDef;
	SDL_Color colorDis;
	SDL_Color colorHov;
	SDL_Color colorPre;
	SDL_Color colorTxt;
};

#pragma once
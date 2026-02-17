#pragma once

#include "UIElement.h"
#include "Vector2D.h"

class UICheckbox : public UIElement
{

public:

	UICheckbox(int id, SDL_Rect bounds, int spacing, SDL_Color colorOutDef, SDL_Color colorOutHov, SDL_Color colorOutDis, SDL_Color colorInDef, SDL_Color colorInChk, SDL_Color colorInDis, int hoverFxId, int clickFxId, bool checked = false);
	virtual ~UICheckbox();

	// Called each loop iteration
	bool Update(float dt);

	void SetBounds(SDL_Rect bounds);

	void Disable();

	bool checked = false;

private:

	bool canClick = true;
	bool drawBasic = false;
	int spacing;
	SDL_Rect innerBounds;
	SDL_Color colorOutDef;
	SDL_Color colorOutHov;
	SDL_Color colorOutDis;
	SDL_Color colorInDef;
	SDL_Color colorInChk;
	SDL_Color colorInDis;
	
};

#pragma once
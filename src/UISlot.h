#pragma once

#include "InteractableItem.h"
#include "UIElement.h"
#include "Vector2D.h"

class UISlot : public UIElement
{

public:

	UISlot(int id, SDL_Rect bounds, const char* text, int horizotalSpacing, int verticalSpacing, SDL_Color colorDef, SDL_Color colorDis, SDL_Color colorHov, SDL_Color colorPre, SDL_Color colorTxt, int hoverFxId, int clickFxId, InteractableItem* item, int amount = 0);
	virtual ~UISlot();

	// Called each loop iteration
	bool Update(float dt);

	void Draw(SDL_Color color);

	void SetItem(InteractableItem* item, int amount = 0);

	InteractableItem* item;
	int amount;

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
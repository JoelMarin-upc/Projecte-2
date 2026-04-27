#pragma once

#include "InteractableItem.h"
#include "UIElement.h"
#include "Vector2D.h"

class UISlot : public UIElement
{

public:

	UISlot(int id, SDL_Rect bounds, const char* text, int horizotalSpacing, int verticalSpacing, SDL_Color colorDef, SDL_Color colorDis, SDL_Color colorHov, SDL_Color colorPre, SDL_Color colorTxt, int hoverFxId, int clickFxId, InteractableItem* item, int amount = 0, bool showPrice = false);
	virtual ~UISlot();

	// Called each loop iteration
	bool Update(float dt);

	void Draw(SDL_Color color);

	void SetItem(InteractableItem* item, int amount = 0, bool showPrice = false, bool sellingPrice = false);

	InteractableItem* item;
	int amount;

private:

	bool canClick = true;
	bool drawBasic = false;
	bool showPrice = false;
	int price = 0;
	int horizotalSpacing;
	int verticalSpacing;
	SDL_Color colorDef;
	SDL_Color colorDis;
	SDL_Color colorHov;
	SDL_Color colorPre;
	SDL_Color colorTxt;
	
};

#pragma once
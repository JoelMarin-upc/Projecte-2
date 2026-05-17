#pragma once

#include "UIElement.h"
#include "Vector2D.h"

class UIImage : public UIElement
{

public:

	UIImage(int id, SDL_Rect bounds, SDL_Texture* disabledTex, SDL_Texture* normalTex, SDL_Texture* focusedTex, SDL_Texture* pressedTex, int hoverFxId, int clickFxId, SDL_Color* background = nullptr, bool drawImageOnCenter = false);
	virtual ~UIImage();

	// Called each loop iteration
	bool Update(float dt);

	void SetImage(SDL_Texture* disabledTex, SDL_Texture* normalTex, SDL_Texture* focusedTex, SDL_Texture* pressedTex);

private:

	bool canClick = true;
	bool drawBasic = false;
	bool drawImageOnCenter = false;
	SDL_Texture* disabledTex;
	SDL_Texture* normalTex;
	SDL_Texture* focusedTex;
	SDL_Texture* pressedTex;
	SDL_Color* background;
};
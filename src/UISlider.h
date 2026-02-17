#pragma once

#include "UIElement.h"
#include "Vector2D.h"

class UISlider : public UIElement
{

public:

	UISlider(int id, SDL_Rect bounds, bool showValue, float min, float max, float step, float value, SDL_Color colorBarDef, SDL_Color colorBarDis, SDL_Color colorSliderDef, SDL_Color colorSliderHov, SDL_Color colorSliderPre, SDL_Color colorSliderDis, SDL_Color colorText, int hoverFxId, int clickFxId);
	virtual ~UISlider();

	void SetValueFromMouse();

	void SetValue(float value);

	void SetBounds(SDL_Rect bounds);

	float GetValue() const { return value; }

	// Called each loop iteration
	bool Update(float dt);

private:

	bool showValue = false;
	SDL_Color colorBarDef;
	SDL_Color colorBarDis;
	SDL_Color colorSliderDef;
	SDL_Color colorSliderHov;
	SDL_Color colorSliderPre;
	SDL_Color colorSliderDis;
	SDL_Color colorTxt;
	SDL_Rect boundsSlider;
	float min, max, step, value;
};

#pragma once
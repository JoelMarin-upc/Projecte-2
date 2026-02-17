#include "UISlider.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"
#include <string>
#include <sstream>
#include <iomanip>

UISlider::UISlider(int id, SDL_Rect bounds, bool showValue, float min, float max, float step, float value, SDL_Color colorBarDef, SDL_Color colorBarDis, SDL_Color colorSliderDef, SDL_Color colorSliderHov, SDL_Color colorSliderPre, SDL_Color colorSliderDis, SDL_Color colorText, int hoverFxId, int clickFxId) : UIElement(UIElementType::BUTTON, id)
{
	this->hoverFxId = hoverFxId;
	this->clickFxId = clickFxId;
	this->bounds = bounds;

	this->colorBarDef = colorBarDef;
	this->colorBarDis = colorBarDis;
	this->colorSliderDef = colorSliderDef;
	this->colorSliderHov = colorSliderHov;
	this->colorSliderPre = colorSliderPre;
	this->colorSliderDis = colorSliderDis;
	this->colorTxt = colorText;

	this->showValue = showValue;
	this->min = min;
	this->max = max;
	if (step > (max - min)) step = max - min;
	this->step = step;
	if (value < min) value = min;
	if (value > max) value = max;
	this->value = value;

	boundsSlider.w = bounds.w / 10;
	boundsSlider.h = bounds.h;
	boundsSlider.y = bounds.y;

	float t = (value - min) / (max - min);
	int sliderMinX = bounds.x;
	int sliderMaxX = bounds.x + bounds.w - boundsSlider.w;

	boundsSlider.x = sliderMinX +
		(int)(t * (sliderMaxX - sliderMinX));
}

UISlider::~UISlider()
{

}

void UISlider::SetValueFromMouse()
{
	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();

	int sliderMinX = bounds.x;
	int sliderMaxX = bounds.x + bounds.w - boundsSlider.w;

	int newSliderX = (int)(mousePos.getX() - boundsSlider.w * 0.5f);

	if (newSliderX < sliderMinX)
		newSliderX = sliderMinX;
	if (newSliderX > sliderMaxX)
		newSliderX = sliderMaxX;

	boundsSlider.x = newSliderX;

	float t = (float)(boundsSlider.x - sliderMinX) /
		(float)(sliderMaxX - sliderMinX);

	float newValue = min + t * (max - min);

	if (step > 0.0f)
	{
		newValue = roundf(newValue / step) * step;
	}

	if (newValue < min) newValue = min;
	if (newValue > max) newValue = max;

	value = newValue;

	float snappedT = (value - min) / (max - min);
	boundsSlider.x = sliderMinX +
		(int)(snappedT * (sliderMaxX - sliderMinX));
}

void UISlider::SetValue(float value)
{
	if (value < min) value = min;
	if (value > max) value = max;
	this->value = value;
	float t = (value - min) / (max - min);
	int sliderMinX = bounds.x;
	int sliderMaxX = bounds.x + bounds.w - boundsSlider.w;

	boundsSlider.x = sliderMinX +
		(int)(t * (sliderMaxX - sliderMinX));
}

void UISlider::SetBounds(SDL_Rect bounds) {
	this->bounds = bounds;
	boundsSlider.w = bounds.w / 10;
	boundsSlider.h = bounds.h;
	boundsSlider.y = bounds.y;

	float t = (value - min) / (max - min);
	int sliderMinX = bounds.x;
	int sliderMaxX = bounds.x + bounds.w - boundsSlider.w;

	boundsSlider.x = sliderMinX +
		(int)(t * (sliderMaxX - sliderMinX));
}

bool UISlider::Update(float dt)
{
	Engine::GetInstance().render->DrawRectangle(bounds, 0, 0, 0, 255, true, false);

	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
	bool mouseOverSlider =
		mousePos.getX() > boundsSlider.x &&
		mousePos.getX() < boundsSlider.x + boundsSlider.w &&
		mousePos.getY() > boundsSlider.y &&
		mousePos.getY() < boundsSlider.y + boundsSlider.h;

	if (state != UIElementState::DISABLED)
	{
		if (state == UIElementState::PRESSED)
		{
			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				SetValueFromMouse();
			}
			else if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				state = mouseOverSlider ? UIElementState::FOCUSED : UIElementState::NORMAL;
				NotifyObserver();
			}
		}
		else
		{
			if (mouseOverSlider) {
				if (state != UIElementState::FOCUSED && state != UIElementState::PRESSED) Engine::GetInstance().audio->PlayFx(hoverFxId);
				state = UIElementState::FOCUSED;
			}
			else
				state = UIElementState::NORMAL;

			if (mouseOverSlider &&
				Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				Engine::GetInstance().audio->PlayFx(clickFxId);
				state = UIElementState::PRESSED;
			}
		}
	}

	switch (state)
	{
	case UIElementState::DISABLED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorBarDis.r, colorBarDis.g, colorBarDis.b, colorBarDis.a, true, false);
		Engine::GetInstance().render->DrawRectangle(boundsSlider, colorSliderDis.r, colorSliderDis.g, colorSliderDis.b, colorSliderDis.a, true, false);
		break;
	case UIElementState::NORMAL:
		Engine::GetInstance().render->DrawRectangle(bounds, colorBarDef.r, colorBarDef.g, colorBarDef.b, colorBarDef.a, true, false);
		Engine::GetInstance().render->DrawRectangle(boundsSlider, colorSliderDef.r, colorSliderDef.g, colorSliderDef.b, colorSliderDef.a, true, false);
		break;
	case UIElementState::FOCUSED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorBarDef.r, colorBarDef.g, colorBarDef.b, colorBarDef.a, true, false);
		Engine::GetInstance().render->DrawRectangle(boundsSlider, colorSliderHov.r, colorSliderHov.g, colorSliderHov.b, colorSliderHov.a, true, false);
		break;
	case UIElementState::PRESSED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorBarDef.r, colorBarDef.g, colorBarDef.b, colorBarDef.a, true, false);
		Engine::GetInstance().render->DrawRectangle(boundsSlider, colorSliderPre.r, colorSliderPre.g, colorSliderPre.b, colorSliderPre.a, true, false);
		break;
	}

	if (showValue)
	{
		step = fabsf(step);
		int decimals = 0;

		while (step < 1.0f && decimals < 6)
		{
			step *= 10.0f;
			decimals++;
		}

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(decimals) << value;
		
		Engine::GetInstance().render->DrawText(oss.str().c_str(), boundsSlider.x, boundsSlider.y, boundsSlider.w, boundsSlider.h, colorTxt);
	}

	return false;
}
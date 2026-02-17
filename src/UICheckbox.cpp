#include "UICheckbox.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

UICheckbox::UICheckbox(int id, SDL_Rect bounds, int spacing, SDL_Color colorOutDef, SDL_Color colorOutHov, SDL_Color colorOutDis, SDL_Color colorInDef, SDL_Color colorInChk, SDL_Color colorInDis, int hoverFxId, int clickFxId, bool checked) : UIElement(UIElementType::BUTTON, id)
{
	this->hoverFxId = hoverFxId;
	this->clickFxId = clickFxId;
	this->bounds = bounds;
	this->colorOutDef = colorOutDef;
	this->colorOutHov = colorOutHov;
	this->colorOutDis = colorOutDis;
	this->colorInDef = colorInDef;
	this->colorInChk = colorInChk;
	this->colorInDis = colorInDis;
	this->checked = checked;
	this->spacing = spacing;

	innerBounds = { bounds.x + spacing, bounds.y + spacing, bounds.w - spacing * 2, bounds.h - spacing * 2 };

	canClick = true;
	drawBasic = false;
	state = checked ? UIElementState::PRESSED : UIElementState::NORMAL;
}

UICheckbox::~UICheckbox()
{

}

bool UICheckbox::Update(float dt)
{
	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
	bool mouseOver = 
		mousePos.getX() > bounds.x && 
		mousePos.getX() < bounds.x + bounds.w && 
		mousePos.getY() > bounds.y && 
		mousePos.getY() < bounds.y + bounds.h;

	if (state != UIElementState::DISABLED)
	{
		// L16: TODO 3: Update the state of the GUiButton according to the mouse position
		

		//If the position of the mouse if inside the bounds of the button 
		if (mouseOver) {

			if (state != UIElementState::FOCUSED && state != UIElementState::PRESSED) Engine::GetInstance().audio->PlayFx(hoverFxId);
			state = UIElementState::FOCUSED;

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				checked = !checked;
				state = checked ? UIElementState::PRESSED : UIElementState::FOCUSED;
				Engine::GetInstance().audio->PlayFx(clickFxId);
				NotifyObserver();
			}
		}
		else {
			state = checked ? UIElementState::PRESSED : UIElementState::NORMAL;
		}
	}

	switch (state)
	{
	case UIElementState::DISABLED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorOutDis.r, colorOutDis.g, colorOutDis.b, colorOutDis.a, true, false);
		Engine::GetInstance().render->DrawRectangle(innerBounds, colorInDis.r, colorInDis.g, colorInDis.b, colorInDis.a, true, false);
		break;
	case UIElementState::NORMAL:
		Engine::GetInstance().render->DrawRectangle(bounds, colorOutDef.r, colorOutDef.g, colorOutDef.b, colorOutDef.a, true, false);
		Engine::GetInstance().render->DrawRectangle(innerBounds, colorInDef.r, colorInDef.g, colorInDef.b, colorInDef.a, true, false);
		break;
	case UIElementState::FOCUSED:
		SDL_Color in = checked ? colorInChk : colorInDef;
		Engine::GetInstance().render->DrawRectangle(bounds, colorOutHov.r, colorOutHov.g, colorOutHov.b, colorOutHov.a, true, false);
		Engine::GetInstance().render->DrawRectangle(innerBounds, in.r, in.g, in.b, in.a, true, false);
		break;
	case UIElementState::PRESSED:
		SDL_Color out = mouseOver ? colorOutHov : colorOutDef;
		Engine::GetInstance().render->DrawRectangle(bounds, out.r, out.g, out.b, out.a, true, false);
		Engine::GetInstance().render->DrawRectangle(innerBounds, colorInChk.r, colorInChk.g, colorInChk.b, colorInChk.a, true, false);
		break;
	}

	return false;
}

void UICheckbox::SetBounds(SDL_Rect bounds)
{
	this->bounds = bounds;
	innerBounds = { bounds.x + spacing, bounds.y + spacing, bounds.w - spacing * 2, bounds.h - spacing * 2 };
}

void UICheckbox::Disable() {
	state = UIElementState::DISABLED;
	checked = false;
}

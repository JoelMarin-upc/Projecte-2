#include "UIButton.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

UIButton::UIButton(int id, SDL_Rect bounds, const char* text, int horizotalSpacing, int verticalSpacing, SDL_Color colorDef, SDL_Color colorDis, SDL_Color colorHov, SDL_Color colorPre, SDL_Color colorTxt, int hoverFxId, int clickFxId) : UIElement(UIElementType::BUTTON, id)
{
	this->hoverFxId = hoverFxId;
	this->clickFxId = clickFxId;
	this->bounds = bounds;
	this->text = text;
	this->colorDef = colorDef;
	this->colorDis = colorDis;
	this->colorHov = colorHov;
	this->colorPre = colorPre;
	this->colorTxt = colorTxt;
	this->horizotalSpacing = horizotalSpacing;
	this->verticalSpacing = verticalSpacing;

	canClick = true;
	drawBasic = false;
}

UIButton::~UIButton()
{

}

bool UIButton::Update(float dt)
{
	if (state != UIElementState::DISABLED)
	{
		// L16: TODO 3: Update the state of the GUiButton according to the mouse position
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();

		//If the position of the mouse if inside the bounds of the button 
		if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w && mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h) {
			
			if (state != UIElementState::FOCUSED && state != UIElementState::PRESSED) Engine::GetInstance().audio->PlayFx(hoverFxId);

			state = UIElementState::FOCUSED;

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = UIElementState::PRESSED;
			}

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				Engine::GetInstance().audio->PlayFx(clickFxId);
				NotifyObserver();
			}
		}
		else {
			state = UIElementState::NORMAL;
		}
	}

	switch (state)
	{
	case UIElementState::DISABLED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorDis.r, colorDis.g, colorDis.b, colorDis.a, true, false);
		break;
	case UIElementState::NORMAL:
		Engine::GetInstance().render->DrawRectangle(bounds, colorDef.r, colorDef.g, colorDef.b, colorDef.a, true, false);
		break;
	case UIElementState::FOCUSED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorHov.r, colorHov.g, colorHov.b, colorHov.a, true, false);
		break;
	case UIElementState::PRESSED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorPre.r, colorPre.g, colorPre.b, colorPre.a, true, false);
		break;
	}

	Engine::GetInstance().render->DrawText(text.c_str(), bounds.x + horizotalSpacing, bounds.y + verticalSpacing, bounds.w - horizotalSpacing * 2, bounds.h - verticalSpacing * 2, colorTxt);

	return false;
}
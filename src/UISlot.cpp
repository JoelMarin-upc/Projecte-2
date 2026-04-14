#include "UISlot.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

UISlot::UISlot(int id, SDL_Rect bounds, const char* text, int horizotalSpacing, int verticalSpacing, SDL_Color colorDef, SDL_Color colorDis, SDL_Color colorHov, SDL_Color colorPre, SDL_Color colorTxt, int hoverFxId, int clickFxId, InteractableItem* item) : UIElement(UIElementType::IMAGE, id)
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
	this->item = item;

	canClick = true;
	drawBasic = false;
}

UISlot::~UISlot()
{

}

bool UISlot::Update(float dt)
{
	// draw background square
	// draw icon
	// draw name
	// draw amount
	// draw description






	if (state != UIElementState::DISABLED)
	{
		// L16: TODO 3: Update the state of the GUiButton according to the mouse position
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
		float logicalX, logicalY;
		SDL_RenderCoordinatesFromWindow(Engine::GetInstance().render->renderer, mousePos.getX(), mousePos.getY(), &logicalX, &logicalY);

		//If the position of the mouse if inside the bounds of the button 
		if (logicalX > bounds.x && logicalX < bounds.x + bounds.w && logicalY > bounds.y && logicalY < bounds.y + bounds.h) {
			
			if (state != UIElementState::FOCUSED && state != UIElementState::PRESSED && hoverFxId != -1) Engine::GetInstance().audio->PlayFx(hoverFxId);

			state = UIElementState::FOCUSED;

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = UIElementState::PRESSED;
			}

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				if (clickFxId != -1) Engine::GetInstance().audio->PlayFx(clickFxId);
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
		Engine::GetInstance().render->DrawTexture(disabledTex, bounds.x, bounds.y, 0.0f);
		break;
	case UIElementState::NORMAL:
		Engine::GetInstance().render->DrawTexture(normalTex, bounds.x, bounds.y, 0.0f);
		break;
	case UIElementState::FOCUSED:
		Engine::GetInstance().render->DrawTexture(focusedTex, bounds.x, bounds.y, 0.0f);
		break;
	case UIElementState::PRESSED:
		Engine::GetInstance().render->DrawTexture(pressedTex, bounds.x, bounds.y, 0.0f);
		break;
	}

	return false;
}
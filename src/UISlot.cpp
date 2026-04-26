#include "UISlot.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"
#include <string>

UISlot::UISlot(int id, SDL_Rect bounds, const char* text, int horizotalSpacing, int verticalSpacing, SDL_Color colorDef, SDL_Color colorDis, SDL_Color colorHov, SDL_Color colorPre, SDL_Color colorTxt, int hoverFxId, int clickFxId, InteractableItem* item, int amount, bool showPrice) : UIElement(UIElementType::IMAGE, id)
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
	this->amount = amount;
	this->showPrice = showPrice;

	canClick = true;
	drawBasic = false;
}

UISlot::~UISlot()
{

}

bool UISlot::Update(float dt)
{
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
		Draw(colorDis);
		break;
	case UIElementState::NORMAL:
		Draw(colorDef);
		break;
	case UIElementState::FOCUSED:
		Draw(colorHov);
		break;
	case UIElementState::PRESSED:
		Draw(colorPre);
		break;
	}

	return false;
}

void UISlot::Draw(SDL_Color color)
{
	Engine::GetInstance().render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, color.r, color.g, color.b, color.a, true, useCamera);
	if (!item) return;
	Engine::GetInstance().render->DrawTexture(item->icon, bounds.x + bounds.w / 2 - item->icon->w / 2, bounds.y, 0.0f);
	std::string name = item->name;
	if (amount > 1) name += "(" + std::to_string(amount) + ")";
	Engine::GetInstance().render->DrawTextMultiline(name.c_str(), bounds.x, bounds.y + item->icon->h, bounds.w, 20, colorTxt);
	//Engine::GetInstance().render->DrawTextMultiline(std::to_string(amount).c_str(), bounds.x, bounds.y, bounds.w, bounds.h, colorTxt);
	Engine::GetInstance().render->DrawTextMultiline(item->description.c_str(), bounds.x, bounds.y + item->icon->h + 25, bounds.w, bounds.h - (bounds.y + item->icon->h + 25), colorTxt);
	if (showPrice) Engine::GetInstance().render->DrawTextMultiline((std::to_string(item->price) + " gold").c_str(), bounds.x, bounds.y + bounds.h - 20, bounds.w, 20, colorTxt);
}

void UISlot::SetItem(InteractableItem* item, int amount, bool showPrice)
{
	if (!item) amount = 0;
	this->item = item;
	this->amount = amount;
	this->showPrice = showPrice;
}

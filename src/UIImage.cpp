#include "UIImage.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

UIImage::UIImage(int id, SDL_Rect bounds, SDL_Texture* disabledTex, SDL_Texture* normalTex, SDL_Texture* focusedTex, SDL_Texture* pressedTex, int hoverFxId, int clickFxId, SDL_Color* background, bool drawImageOnCenter) : UIElement(UIElementType::IMAGE, id)
{
	this->hoverFxId = hoverFxId;
	this->clickFxId = clickFxId;
	this->bounds = bounds;
	this->disabledTex = disabledTex;
	this->normalTex = normalTex;
	this->focusedTex = focusedTex;
	this->pressedTex = pressedTex;
	this->background = background;
	this->drawImageOnCenter = drawImageOnCenter;

	canClick = true;
	drawBasic = false;
}

UIImage::~UIImage()
{

}

bool UIImage::Update(float dt)
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

	SDL_Texture* texture = nullptr;

	switch (state)
	{
	case UIElementState::DISABLED:
		if (disabledTex) texture = disabledTex;
		break;
	case UIElementState::NORMAL:
		if (normalTex) texture = normalTex;
		break;
	case UIElementState::FOCUSED:
		if (focusedTex) texture = focusedTex;
		break;
	case UIElementState::PRESSED:
		if (pressedTex) texture = pressedTex;
		break;
	}

	SDL_Rect rect = bounds;
	if (texture && drawImageOnCenter) rect = { bounds.x + bounds.w / 2 - texture->w / 2, bounds.y + bounds.h / 2 - texture->h / 2, bounds.w, bounds.h };

	if (background) Engine::GetInstance().render->DrawRectangle(bounds, background->r, background->g, background->b, background->a, true, false);
	if (texture) Engine::GetInstance().render->DrawTexture(texture, rect.x, rect.y, 0.0f);

	return true;
}

void UIImage::SetImage(SDL_Texture* disabledTex, SDL_Texture* normalTex, SDL_Texture* focusedTex, SDL_Texture* pressedTex)
{
	this->disabledTex = disabledTex;
	this->normalTex = normalTex;
	this->focusedTex = focusedTex;
	this->pressedTex = pressedTex;
}

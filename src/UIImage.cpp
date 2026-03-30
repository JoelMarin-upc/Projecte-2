#include "UIImage.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

UIImage::UIImage(int id, SDL_Rect bounds, SDL_Texture* disabledTex, SDL_Texture* normalTex, SDL_Texture* focusedTex, SDL_Texture* pressedTex, int hoverFxId, int clickFxId) : UIElement(UIElementType::IMAGE, id)
{
	this->hoverFxId = hoverFxId;
	this->clickFxId = clickFxId;
	this->bounds = bounds;
	this->disabledTex = disabledTex;
	this->normalTex = normalTex;
	this->focusedTex = focusedTex;
	this->pressedTex = pressedTex;

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

		//If the position of the mouse if inside the bounds of the button 
		if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w && mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h) {
			
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
		Engine::GetInstance().render->DrawTexture(disabledTex, bounds.x, bounds.y);
		break;
	case UIElementState::NORMAL:
		Engine::GetInstance().render->DrawTexture(normalTex, bounds.x, bounds.y);
		break;
	case UIElementState::FOCUSED:
		Engine::GetInstance().render->DrawTexture(focusedTex, bounds.x, bounds.y);
		break;
	case UIElementState::PRESSED:
		Engine::GetInstance().render->DrawTexture(pressedTex, bounds.x, bounds.y);
		break;
	}

	return false;
}
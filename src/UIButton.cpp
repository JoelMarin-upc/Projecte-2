#include "UIButton.h"
#include "Render.h"
#include "Engine.h"
#include "Log.h"
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
		float mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		int gameX = 0;
		int gameY = 0;
		Engine::GetInstance().render->WindowToGameCoords((int)mouseX, (int)mouseY, gameX, gameY);

		if (gameX > bounds.x && gameX < bounds.x + bounds.w &&
			gameY > bounds.y && gameY < bounds.y + bounds.h)
		{
			if (state != UIElementState::FOCUSED && state != UIElementState::PRESSED && hoverFxId != -1)
				Engine::GetInstance().audio->PlayFx(hoverFxId);

			state = UIElementState::FOCUSED;

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = UIElementState::PRESSED;
			}

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				if (clickFxId != -1)
					Engine::GetInstance().audio->PlayFx(clickFxId);

				NotifyObserver();
			}
		}
		else
		{
			state = UIElementState::NORMAL;
		}
	}

	switch (state)
	{
	case UIElementState::DISABLED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorDis.r, colorDis.g, colorDis.b, colorDis.a, true, useCamera);
		break;
	case UIElementState::NORMAL:
		Engine::GetInstance().render->DrawRectangle(bounds, colorDef.r, colorDef.g, colorDef.b, colorDef.a, true, useCamera);
		break;
	case UIElementState::FOCUSED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorHov.r, colorHov.g, colorHov.b, colorHov.a, true, useCamera);
		break;
	case UIElementState::PRESSED:
		Engine::GetInstance().render->DrawRectangle(bounds, colorPre.r, colorPre.g, colorPre.b, colorPre.a, true, useCamera);
		break;
	}

	TTF_Font* font = Engine::GetInstance().render->font;
	if (font && !text.empty())
	{
		int textW = 0, textH = 0;
		TTF_GetStringSize(font, text.c_str(), 0, &textW, &textH);
		int centeredX = bounds.x + (bounds.w - textW) / 2;
		int centeredY = bounds.y + (bounds.h - textH) / 2;
		Engine::GetInstance().render->DrawText(text.c_str(), centeredX, centeredY, 0, 0, colorTxt);
	}

	return false;

}
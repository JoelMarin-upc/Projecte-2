#include "UIManager.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include <string>
#include "tracy/Tracy.hpp"

UIManager::UIManager() :Module()
{
	name = "UIManager";
}

UIManager::~UIManager() {}

bool UIManager::Start()
{
	return true;
}

std::shared_ptr<UIElement> UIManager::CreateUIElement(UIElementType type, int id, SDL_Rect bounds, Module* observer, std::vector<SDL_Color> colors, int hoverFxId, int clickFxId, UIParameters params)
{
	std::shared_ptr<UIElement> uiElement = std::make_shared<UIElement>();

	// L16: TODO 1: Implement CreateUIElement function that instantiates a new UIElement according to the UIElementType and add it to the list of UIElements
	//Call the constructor according to the UIElementType
	SDL_Color c1 = { 0, 0, 0, 0 };
	SDL_Color c2 = { 0, 0, 0, 0 };
	SDL_Color c3 = { 0, 0, 0, 0 };
	SDL_Color c4 = { 0, 0, 0, 0 };
	SDL_Color c5 = { 0, 0, 0, 0 };
	SDL_Color c6 = { 0, 0, 0, 0 };
	SDL_Color c7 = { 0, 0, 0, 0 };

	switch (type)
	{
	case UIElementType::LABEL:
		c1 = { 0, 0, 0, 0 };
		c2 = { 0, 0, 0, 0 };
		if (colors.size() > 0) c1 = colors[0];
		if (colors.size() > 1) c2 = colors[1];
		
		uiElement = std::make_shared<UILabel>(id, bounds, params.text, c1, c2);
		break;

	case UIElementType::BUTTON:
		c1 = { 0, 0, 0, 0 };
		c2 = { 0, 0, 0, 0 };
		c3 = { 0, 0, 0, 0 };
		c4 = { 0, 0, 0, 0 };
		c5 = { 0, 0, 0, 0 };
		if (colors.size() > 0) c1 = colors[0];
		if (colors.size() > 1) c2 = colors[1];
		if (colors.size() > 2) c3 = colors[2];
		if (colors.size() > 3) c4 = colors[3];
		if (colors.size() > 4) c5 = colors[4];
		
		uiElement = std::make_shared<UIButton>(id, bounds, params.text, params.horizotalSpacing, params.verticalSpacing, c1, c2, c3, c4, c5, hoverFxId, clickFxId);
		break;

	case UIElementType::CHECKBOX:
		c1 = { 0, 0, 0, 0 };
		c2 = { 0, 0, 0, 0 };
		c3 = { 0, 0, 0, 0 };
		c4 = { 0, 0, 0, 0 };
		c5 = { 0, 0, 0, 0 };
		c6 = { 0, 0, 0, 0 };
		if (colors.size() > 0) c1 = colors[0];
		if (colors.size() > 1) c2 = colors[1];
		if (colors.size() > 2) c3 = colors[2];
		if (colors.size() > 3) c4 = colors[3];
		if (colors.size() > 4) c5 = colors[4];
		if (colors.size() > 5) c6 = colors[5];

		uiElement = std::make_shared<UICheckbox>(id, bounds, params.spacing, c1, c2, c3, c4, c5, c6, hoverFxId, clickFxId, params.checked);
		break;

	case UIElementType::SLIDER:
		c1 = { 0, 0, 0, 0 };
		c2 = { 0, 0, 0, 0 };
		c3 = { 0, 0, 0, 0 };
		c4 = { 0, 0, 0, 0 };
		c5 = { 0, 0, 0, 0 };
		c6 = { 0, 0, 0, 0 };
		c7 = { 0, 0, 0, 0 };
		if (colors.size() > 0) c1 = colors[0];
		if (colors.size() > 1) c2 = colors[1];
		if (colors.size() > 2) c3 = colors[2];
		if (colors.size() > 3) c4 = colors[3];
		if (colors.size() > 4) c5 = colors[4];
		if (colors.size() > 5) c6 = colors[5];
		if (colors.size() > 6) c7 = colors[6];

		uiElement = std::make_shared<UISlider>(id, bounds, params.showValue, params.min, params.max, params.step, params.value, c1, c2, c3, c4, c5, c6, c7, hoverFxId, clickFxId);
		break;
	}

	//Set the observer
	uiElement->observer = observer;

	// Created GuiControls are add it to the list of controls
	UIElementsList.push_back(uiElement);

	return uiElement;
}

bool UIManager::PostUpdate(float dt)
{
	ZoneScoped;
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug = !debug;

	for (const auto& control : UIElementsList)
	{
		if (control->active) {
			control->Update(dt);
			if (debug) DrawControlDebug(control);
		}
	}

	return true;
}

bool UIManager::CleanUp()
{
	for (const auto& uiElement : UIElementsList)
	{
		uiElement->CleanUp();
	}

	return true;
}

void UIManager::DrawControlDebug(std::shared_ptr<UIElement> control)
{
	SDL_Color color;
	switch (control->state)
	{
	case UIElementState::DISABLED:
		color = { 150, 150, 150, 150 };
		break;
	case UIElementState::NORMAL:
		color = { 255, 255, 255, 150 };
		break;
	case UIElementState::FOCUSED:
		color = { 255, 255, 0, 150 };
		break;
	case UIElementState::PRESSED:
		color = { 255, 0, 0, 150 };
		break;
	default:
		color = { 0, 0, 0, 0 };
		break;
	}
	SDL_Rect rect = control->bounds;
	if (rect.w == 0) rect.w = 20 * control->text.size();
	if (rect.h == 0) rect.h = 30;
	Engine::GetInstance().render->DrawRectangle(rect, color.r, color.g, color.b, color.a, true, false);
}

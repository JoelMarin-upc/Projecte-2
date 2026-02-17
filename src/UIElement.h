#pragma once

#include "Input.h"
#include "Render.h"
#include "Module.h"
#include "Vector2D.h"
#include <SDL3/SDL.h>s

enum class UIElementType
{
	LABEL,
	BUTTON,
	TOGGLE,
	CHECKBOX,
	SLIDER,
	SLIDERBAR,
	COMBOBOX,
	DROPDOWNBOX,
	INPUTBOX,
	VALUEBOX,
	SPINNER
};

enum class UIElementState
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED
};

class UIElement : public std::enable_shared_from_this<UIElement>
{
public:

	UIElement() {}

	// Constructor
	UIElement(UIElementType type, int id) : type(type), id(id), state(UIElementState::NORMAL) {}

	// Constructor
	UIElement(UIElementType type, SDL_Rect bounds, const char* text) :
		type(type),
		state(UIElementState::NORMAL),
		bounds(bounds),
		text(text)
	{
		color.r = 255; color.g = 255; color.b = 255;
		texture = NULL;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// 
	void SetTexture(SDL_Texture* tex)
	{
		texture = tex;
		section = { 0, 0, 0, 0 };
	}

	virtual void SetBounds(SDL_Rect bounds)
	{
		this->bounds = bounds;
	}

	// 
	void SetObserver(Module* module)
	{
		observer = module;
	}

	// 
	void NotifyObserver()
	{
		observer->OnUIMouseClickEvent(this);
	}

	virtual void Disable() {
		state = UIElementState::DISABLED;
	}

	virtual void Enable() {
		state = UIElementState::NORMAL;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Destroy()
	{
		return true;
	}

public:

	bool active;
	int id;
	UIElementType type;
	UIElementState state;

	std::string text;       // UIElement text (if required)
	SDL_Rect bounds;        // Position and size
	SDL_Color color;        // Tint color

	SDL_Texture* texture;   // Texture atlas reference
	SDL_Rect section;       // Texture atlas base section

	Module* observer;        // Observer 

	int hoverFxId;
	int clickFxId;
};
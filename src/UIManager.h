#pragma once

#include "Module.h"
#include "UIElement.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UICheckbox.h"
#include "UISlider.h"
#include "UIImage.h"
#include "UISlot.h"

#include <list>
#include <vector>

class UIParameters {

public:

	static UIParameters Label(const char* text) {
		UIParameters p = UIParameters();
		p.text = text;
		return p;
	}

	static UIParameters Button(const char* text, int horizotalSpacing = 0, int verticalSpacing = 0) {
		UIParameters p = UIParameters();
		p.text = text;
		p.horizotalSpacing = horizotalSpacing;
		p.verticalSpacing = verticalSpacing;
		return p;
	}

	static UIParameters Checkbox(int spacing, bool checked) {
		UIParameters p = UIParameters();
		p.spacing = spacing;
		p.checked = checked;
		return p;
	}

	static UIParameters Slider(float showValue, float min, float max, float step, float value) {
		UIParameters p = UIParameters();
		p.showValue = showValue;
		p.min = min;
		p.max = max;
		p.step = step;
		p.value = value;
		return p;
	}

	static UIParameters Image(SDL_Texture* disabledTex, SDL_Texture* normalTex, SDL_Texture* focusedTex, SDL_Texture* pressedTex, SDL_Color* background = nullptr, bool drawImageOnCenter = false) {
		UIParameters p = UIParameters();
		p.disabledTex = disabledTex;
		p.normalTex = normalTex;
		p.focusedTex = focusedTex;
		p.pressedTex = pressedTex;
		p.background = background;
		p.drawImageOnCenter = drawImageOnCenter;
		return p;
	}

	static UIParameters Slot(SDL_Texture* disabledTex, SDL_Texture* normalTex, SDL_Texture* focusedTex, SDL_Texture* pressedTex, std::shared_ptr<InteractableItem> item, int amount) {
		UIParameters p = UIParameters();
		p.disabledTex = disabledTex;
		p.normalTex = normalTex;
		p.focusedTex = focusedTex;
		p.pressedTex = pressedTex;
		p.item = item;
		p.amount = amount;
		return p;
	}

	static UIParameters Default() {
		UIParameters p = UIParameters();
		p.text = "";
		p.spacing = 5;
		p.checked = false;
		p.showValue = false;
		p.min = 0;
		p.max = 1;
		p.step = 0.1f;
		p.value = 0.5f;
		p.disabledTex = nullptr;
		p.normalTex = nullptr;
		p.focusedTex = nullptr;
		p.pressedTex = nullptr;
		p.item = nullptr;
		p.amount = 0;
		p.background = nullptr;
		p.drawImageOnCenter = false;
		return p;
	}

	const char* text;
	float showValue, min, max, step, value;
	int spacing, horizotalSpacing, verticalSpacing, amount;
	bool checked, drawImageOnCenter;
	std::shared_ptr<InteractableItem> item;
	SDL_Texture* disabledTex;
	SDL_Texture* normalTex;
	SDL_Texture* focusedTex;
	SDL_Texture* pressedTex;
	SDL_Color* background;
};

class UIManager : public Module
{
public:

	// Constructor
	UIManager();

	// Destructor
	virtual ~UIManager();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void DrawControlDebug(std::shared_ptr<UIElement> control);

	// Additional methods
	std::shared_ptr<UIElement> CreateUIElement(UIElementType type, int id, SDL_Rect bounds, Module* observer, std::vector<SDL_Color> colors, int hoverFxId, int clickFxId, UIParameters params = UIParameters::Default(), bool useCamera = false, bool drawOnTop = false);
	void DestroyUIElement(std::shared_ptr<UIElement> element);

public:

	std::list<std::shared_ptr<UIElement>> UIElementsList;
	SDL_Texture* texture;
	int uiLockFrame = -1;

private:
	bool debug = false;

};
#pragma once

#include "Module.h"
#include "UIElement.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UICheckbox.h"
#include "UISlider.h"

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
		return p;
	}

	const char* text;
	float showValue, min, max, step, value;
	int spacing, horizotalSpacing, verticalSpacing;
	bool checked;
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
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void DrawControlDebug(std::shared_ptr<UIElement> control);

	// Additional methods
	std::shared_ptr<UIElement> CreateUIElement(UIElementType type, int id, SDL_Rect bounds, Module* observer, std::vector<SDL_Color> colors, int hoverFxId, int clickFxId, UIParameters params = UIParameters::Default());

public:

	std::list<std::shared_ptr<UIElement>> UIElementsList;
	SDL_Texture* texture;

private:
	bool debug = false;

};
#pragma once

#include "Module.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_gamepad.h>
#include "Vector2D.h"

#define NUM_MOUSE_BUTTONS 5

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input : public Module
{

public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	Vector2D GetMousePosition();
	Vector2D GetMouseMotion();

	KeyState GetGamepadButton(SDL_GamepadButton button) const
	{
		return gamepadButtons[button];
	}

	float GetLeftStickX() const { return leftX; }
	float GetLeftStickY() const { return leftY; }

	float GetRightStickX() const { return rightX; }
	float GetRightStickY() const { return rightY; }

	float GetLeftTrigger() const { return leftTrigger; }
	float GetRightTrigger() const { return rightTrigger; }

private:
	bool windowEvents[WE_COUNT];
	KeyState* keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;

	SDL_Gamepad* gamepad = nullptr;

	static const int NUM_GAMEPAD_BUTTONS = SDL_GAMEPAD_BUTTON_COUNT;

	KeyState gamepadButtons[NUM_GAMEPAD_BUTTONS];

	float leftX = 0.0f;
	float leftY = 0.0f;

	float rightX = 0.0f;
	float rightY = 0.0f;

	float leftTrigger = 0.0f;
	float rightTrigger = 0.0f;

	float virtualMouseSpeed = 1.0f;

	bool usingGamepadMouse = false;
};

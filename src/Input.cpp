#include "Engine.h"
#include "Input.h"
#include "Window.h"
#include "Log.h"

#define MAX_KEYS 300

Input::Input() : Module()
{
	name = "input";

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
	memset(windowEvents, 0, sizeof(windowEvents));
	memset(gamepadButtons, KEY_IDLE, sizeof(KeyState) * NUM_GAMEPAD_BUTTONS);
	mouseMotionX = mouseMotionY = mouseX = mouseY = 0;
}

// Destructor
Input::~Input()
{
	delete[] keyboard;
}

// Called before render is available
bool Input::Awake()
{
	LOG("Init SDL input event system");
	bool ret = true;

	if (SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_GAMEPAD) != true)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Input::Start()
{
	SDL_StopTextInput(Engine::GetInstance().window->window);
	
	int numGamepads = 0;
	SDL_JoystickID* gamepads = SDL_GetGamepads(&numGamepads);

	if (numGamepads > 0)
	{
		gamepad = SDL_OpenGamepad(gamepads[0]);

		if (gamepad)
		{
			LOG("Gamepad connected: %s", SDL_GetGamepadName(gamepad));
		}
	}

	SDL_free(gamepads);

	return true;
}

// Called each loop iteration
bool Input::PreUpdate()
{
	static SDL_Event event;

	int numKeys = 0;
	const bool* keys = SDL_GetKeyboardState(&numKeys);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if (mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	for (int i = 0; i < NUM_GAMEPAD_BUTTONS; ++i)
	{
		if (gamepadButtons[i] == KEY_DOWN)
			gamepadButtons[i] = KEY_REPEAT;

		if (gamepadButtons[i] == KEY_UP)
			gamepadButtons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_EVENT_WINDOW_HIDDEN:
		case SDL_EVENT_WINDOW_MINIMIZED:
		case SDL_EVENT_WINDOW_FOCUS_LOST:
			windowEvents[WE_HIDE] = true;
			break;

		case SDL_EVENT_WINDOW_SHOWN:
		case SDL_EVENT_WINDOW_FOCUS_GAINED:
		case SDL_EVENT_WINDOW_MAXIMIZED:
		case SDL_EVENT_WINDOW_RESTORED:
			windowEvents[WE_SHOW] = true;
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (event.button.button >= 1 && event.button.button <= NUM_MOUSE_BUTTONS)
				mouseButtons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (event.button.button >= 1 && event.button.button <= NUM_MOUSE_BUTTONS)
				mouseButtons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_EVENT_MOUSE_MOTION:
		{
			int scale = Engine::GetInstance().window->GetScale();
			mouseMotionX = (int) (event.motion.xrel / scale);
			mouseMotionY = (int)(event.motion.yrel / scale);
			mouseX = (int)(event.motion.x / scale);
			mouseY = (int)(event.motion.y / scale);
		}
		break;
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		{
			SDL_GamepadButton button =
				(SDL_GamepadButton)event.gbutton.button;

			gamepadButtons[button] = KEY_DOWN;

			if (button == SDL_GAMEPAD_BUTTON_SOUTH)
			{
				mouseButtons[0] = KEY_DOWN;
			}
			if (button == SDL_GAMEPAD_BUTTON_EAST)
			{
				mouseButtons[1] = KEY_DOWN;
			}
		}
		break;
		case SDL_EVENT_GAMEPAD_BUTTON_UP:
		{
			SDL_GamepadButton button =
				(SDL_GamepadButton)event.gbutton.button;

			gamepadButtons[button] = KEY_UP;

			if (button == SDL_GAMEPAD_BUTTON_SOUTH)
			{
				mouseButtons[0] = KEY_UP;
			}
			if (button == SDL_GAMEPAD_BUTTON_EAST)
			{
				mouseButtons[1] = KEY_UP;
			}
		}
		break;
		}
	}

	if (gamepad)
	{
		const float DEADZONE = 8000.0f;

		auto NormalizeAxis = [&](Sint16 value) -> float
			{
				if (abs(value) < DEADZONE)
					return 0.0f;

				return value / 32767.0f;
			};

		leftX = NormalizeAxis(
			SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX));

		leftY = NormalizeAxis(
			SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY));

		rightX = NormalizeAxis(
			SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX));

		rightY = NormalizeAxis(
			SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY));

		leftTrigger =
			SDL_GetGamepadAxis(gamepad,
				SDL_GAMEPAD_AXIS_LEFT_TRIGGER) / 32767.0f;

		rightTrigger =
			SDL_GetGamepadAxis(gamepad,
				SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) / 32767.0f;

		float rightStickX = GetRightStickX();
		float rightStickY = GetRightStickY();

		const float MOUSE_DEADZONE = 0.15f;

		if (fabs(rightStickX) > MOUSE_DEADZONE ||
			fabs(rightStickY) > MOUSE_DEADZONE)
		{
			usingGamepadMouse = true;

			float dt = Engine::GetInstance().GetDt();

			mouseX += (int)(rightStickX * virtualMouseSpeed * dt);
			mouseY += (int)(rightStickY * virtualMouseSpeed * dt);

			mouseMotionX = (int)(rightStickX * virtualMouseSpeed * dt);
			mouseMotionY = (int)(rightStickY * virtualMouseSpeed * dt);

			// Clamp to window size
			int w, h;
			SDL_GetWindowSize(
				Engine::GetInstance().window->window,
				&w,
				&h);

			if (mouseX < 0) mouseX = 0;
			if (mouseY < 0) mouseY = 0;
			if (mouseX > w) mouseX = w;
			if (mouseY > h) mouseY = h;

			// Move actual OS cursor
			SDL_WarpMouseInWindow(
				Engine::GetInstance().window->window,
				mouseX,
				mouseY);
		}
	}

	return true;
}

// Called before quitting
bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	if (gamepad)
	{
		SDL_CloseGamepad(gamepad);
		gamepad = nullptr;
	}
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

Vector2D Input::GetMousePosition()
{
	return Vector2D((float)mouseX, (float)mouseY);
}

Vector2D Input::GetMouseMotion()
{
	return Vector2D((float)mouseMotionX, (float)mouseMotionY);
}

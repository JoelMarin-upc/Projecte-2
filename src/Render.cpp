#include "Engine.h"
#include "Window.h"
#include "Render.h"
#include "Map.h"
#include "Log.h"
#include <sstream>
#include <SDL3_image/SDL_image.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Render::Render() : Module()
{
	name = "render";
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 255;
}

// Destructor
Render::~Render()
{
}

// Called before render is available
bool Render::Awake()
{
	LOG("Create SDL rendering context");
	bool ret = true;

	int scale = Engine::GetInstance().window->GetScale();
	SDL_Window* window = Engine::GetInstance().window->window;

	//L05 TODO 5 - Load the configuration of the Render module
	
	// SDL3: no flags; create default renderer and set vsync separately
	renderer = SDL_CreateRenderer(window, nullptr);

	if (renderer == nullptr)
	{
		LOG("Could not create the renderer! SDL_Error: %s", SDL_GetError());
		return false;
	}

	if (configParameters.child("vsync").attribute("value").as_bool())
	{
		if (!SDL_SetRenderVSync(renderer, 1))
		{
			LOG("Warning: could not enable vsync: %s", SDL_GetError());
		}
		else
		{
			LOG("Using vsync");
		}
	}

	const int baseW = Engine::GetInstance().window->width;   // 1280
	const int baseH = Engine::GetInstance().window->height;  // 720

	gameTarget = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		baseW,
		baseH
	);

	if (gameTarget == nullptr)
	{
		LOG("Could not create gameTarget! SDL_Error: %s", SDL_GetError());
		return false;
	}

	SDL_SetTextureScaleMode(gameTarget, SDL_SCALEMODE_NEAREST);

	camera.w = baseW;
	camera.h = baseH;
	camera.x = 0;
	camera.y = 0;

	TTF_Init();
	font = TTF_OpenFont("Assets/Fonts/Leander.ttf", 25);

	return true;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	if (!SDL_GetRenderViewport(renderer, &viewport))
	{
		LOG("SDL_GetRenderViewport failed: %s", SDL_GetError());
	}
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	// Renderizamos primero al buffer interno del juego
	SDL_SetRenderTarget(renderer, gameTarget);
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate(float dt)
{
	// Volvemos a la pantalla real
	SDL_SetRenderTarget(renderer, nullptr);

	int screenW = 0;
	int screenH = 0;
	if (!SDL_GetRenderOutputSize(renderer, &screenW, &screenH))
	{
		LOG("SDL_GetRenderOutputSize failed: %s", SDL_GetError());
		return false;
	}

	SDL_FRect dst;
	dst.x = 0.0f;
	dst.y = 0.0f;
	dst.w = (float)screenW;
	dst.h = (float)screenH;

	// Limpiamos pantalla real
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Escalamos TODO el frame de golpe
	if (!SDL_RenderTexture(renderer, gameTarget, nullptr, &dst))
	{
		LOG("SDL_RenderTexture failed: %s", SDL_GetError());
		return false;
	}
	SDL_RenderPresent(renderer);
	CameraFollow();
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");

	if (font != nullptr)
	{
		TTF_CloseFont(font);
		font = nullptr;
	}

	if (gameTarget != nullptr)
	{
		SDL_DestroyTexture(gameTarget);
		gameTarget = nullptr;
	}

	if (renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	TTF_Quit();

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::CameraFollow()
{
	if (!follow) return;

	const float deadZoneWidth = 0.0f;
	const float deadZoneHeight = 0.0f;

	float leftBound = -camera.x + (camera.w - deadZoneWidth) / 2.0f;
	float rightBound = -camera.x + (camera.w + deadZoneWidth) / 2.0f;
	float topBound = -camera.y + (camera.h - deadZoneHeight) / 2.0f;
	float bottomBound = -camera.y + (camera.h + deadZoneHeight) / 2.0f;

	float playerX = follow->position.getX();
	float playerY = follow->position.getY();

	if (playerX < leftBound)
		camera.x = (int)roundf(-(playerX - (camera.w - deadZoneWidth) / 2.0f));
	else if (playerX > rightBound)
		camera.x = (int)roundf(-(playerX - (camera.w + deadZoneWidth) / 2.0f));

	if (playerY < topBound)
		camera.y = (int)roundf(-(playerY - (camera.h - deadZoneHeight) / 2.0f));
	else if (playerY > bottomBound)
		camera.y = (int)roundf(-(playerY - (camera.h + deadZoneHeight) / 2.0f));

	camera.x = (int)roundf((float)camera.x);
	camera.y = (int)roundf((float)camera.y);
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_SetRenderViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_SetRenderViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, float speed, const SDL_Rect* section, bool facingRight, double angle, int pivotX, int pivotY, float tileScale) const
{
	bool ret = true;

	SDL_FRect rect;
	rect.x = (float)((int)roundf((float)camera.x * speed) + x);
	rect.y = (float)((int)roundf((float)camera.y * speed) + y);

	if (section != nullptr)
	{
		rect.w = (float)((int)roundf((float)section->w * tileScale));
		rect.h = (float)((int)roundf((float)section->h * tileScale));
	}
	else
	{
		float tw = 0.0f, th = 0.0f;
		if (!SDL_GetTextureSize(texture, &tw, &th))
		{
			return false;
		}
		rect.w = tw;
		rect.h = th;
	}

	const SDL_FRect* src = nullptr;
	SDL_FRect srcRect;
	if (section != nullptr)
	{
		srcRect.x = (float)section->x;
		srcRect.y = (float)section->y;
		srcRect.w = (float)section->w;
		srcRect.h = (float)section->h;
		src = &srcRect;
	}

	SDL_FPoint* p = nullptr;
	SDL_FPoint pivot;
	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = (float)pivotX;
		pivot.y = (float)pivotY;
		p = &pivot;
	}

	if (!SDL_RenderTextureRotated(renderer, texture, src, &rect, angle, p, facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL))
	{
		LOG("Cannot blit to screen. SDL_RenderTextureRotated error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	int scale = Engine::GetInstance().window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_FRect rec;
	if (use_camera)
	{
		rec.x = (float)((int)(camera.x + rect.x * scale));
		rec.y = (float)((int)(camera.y + rect.y * scale));
		rec.w = (float)(rect.w * scale);
		rec.h = (float)(rect.h * scale);
	}
	else
	{
		rec.x = (float)(rect.x * scale);
		rec.y = (float)(rect.y * scale);
		rec.w = (float)(rect.w * scale);
		rec.h = (float)(rect.h * scale);
	}

	int result = (filled ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderRect(renderer, &rec)) ? 0 : -1;

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect/SDL_RenderRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	int scale = Engine::GetInstance().window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	float X1, Y1, X2, Y2;

	if (use_camera)
	{
		X1 = (float)(camera.x + x1 * scale);
		Y1 = (float)(camera.y + y1 * scale);
		X2 = (float)(camera.x + x2 * scale);
		Y2 = (float)(camera.y + y2 * scale);
	}
	else
	{
		X1 = (float)(x1 * scale);
		Y1 = (float)(y1 * scale);
		X2 = (float)(x2 * scale);
		Y2 = (float)(y2 * scale);
	}

	int result = SDL_RenderLine(renderer, X1, Y1, X2, Y2) ? 0 : -1;

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderLine error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_FPoint points[360];
	float factor = (float)M_PI / 180.0f;

	float cx = (float)((use_camera ? camera.x : 0) + x);
	float cy = (float)((use_camera ? camera.y : 0) + y);

	for (int i = 0; i < 360; ++i)
	{
		points[i].x = cx + (float)(radius * cos(i * factor));
		points[i].y = cy + (float)(radius * sin(i * factor));
	}

	int result = SDL_RenderPoints(renderer, points, 360) ? 0 : -1;

	if (result != 0)
	{
		LOG("Cannot draw circle to screen. SDL_RenderPoints error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawText(const char* text, int x, int y, int w, int h, SDL_Color color) const
{
	if (!font || !renderer || !text)
	{
		LOG("DrawText: invalid font/renderer/text");
		return false;
	}

	SDL_Surface* surface = TTF_RenderText_Solid(font, text, 0, color);
	if (!surface)
	{
		LOG("DrawText: TTF_RenderText_Solid failed: %s", SDL_GetError());
		return false;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
	{
		LOG("DrawText: SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
		SDL_DestroySurface(surface);
		return false;
	}

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	float fw = (float)surface->w;
	float fh = (float)surface->h;

	SDL_FRect dstrect = { (float)x, (float)y, fw, fh };

	if (!SDL_RenderTexture(renderer, texture, nullptr, &dstrect))
	{
		LOG("DrawText: SDL_RenderTexture failed: %s", SDL_GetError());
	}

	SDL_DestroyTexture(texture);
	SDL_DestroySurface(surface);

	return true;
}

bool Render::DrawTextMultiline(const char* text, int x, int y, int w, int h, SDL_Color color, int lineHeight) const
{
	std::string full(text);
	std::istringstream stream(full);
	std::string line;
	int offsetY = 0;

	while (std::getline(stream, line, '\n'))
	{
		if (!line.empty())
			DrawText(line.c_str(), x, y + offsetY, w, lineHeight, color);
		offsetY += lineHeight;
	}
	return true;
}

bool Render::IsOnScreenWorldRect(float x, float y, float w, float h, int margin) const
{
	bool result = false;

	// --- 1. Compute camera rectangle in WORLD space ---

	// camera.x and camera.y are render offsets (negative world position),
	// so we negate them to get the camera position in world coordinates.
	// The margin expands the camera rectangle to activate objects earlier.
	float camLeft = -camera.x - margin;
	float camTop = -camera.y - margin;

	// Right and bottom edges of the camera rectangle
	float camRight = camLeft + camera.w + margin * 2;
	float camBottom = camTop + camera.h + margin * 2;

	// --- 2. Compute object rectangle in WORLD space ---

	float objLeft = x;
	float objTop = y;
	float objRight = x + w;
	float objBottom = y + h;

	// --- 3. Axis-Aligned Bounding Box (AABB) overlap test ---

	result = objRight >= camLeft &&
		objLeft <= camRight &&
		objBottom >= camTop &&
		objTop <= camBottom;

	return result;
}

void Render::SetCursorTexture(const char* imagePath)
{
	SDL_Surface* surface = IMG_Load(imagePath);
	SDL_Cursor* cursor = SDL_CreateColorCursor(surface, 0, 0);
	SDL_SetCursor(cursor);
}

bool Render::WindowToGameCoords(int windowX, int windowY, int& gameX, int& gameY) const
{
	int screenW = 0;
	int screenH = 0;

	if (!SDL_GetRenderOutputSize(renderer, &screenW, &screenH))
	{
		LOG("WindowToGameCoords: SDL_GetRenderOutputSize failed: %s", SDL_GetError());
		return false;
	}

	const int baseW = Engine::GetInstance().window->width;
	const int baseH = Engine::GetInstance().window->height;

	gameX = (int)((float)windowX * (float)baseW / (float)screenW);
	gameY = (int)((float)windowY * (float)baseH / (float)screenH);

	return true;
}

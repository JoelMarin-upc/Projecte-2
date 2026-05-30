#include "ParticleSystem.h"
#include "Engine.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"
#include <cmath>
#include <random>
#include <algorithm>

float ParticleSystem::random_float(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

int ParticleSystem::random_int(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

void ParticleSystem::LoadTextures()
{
    if (texturesLoaded) return;

    texBlood = Engine::GetInstance().textures->Load("Assets/Textures/Blood.png");
    texCross = Engine::GetInstance().textures->Load("Assets/Textures/Cross.png");
    texArrow = Engine::GetInstance().textures->Load("Assets/Textures/Arrow.png");

    texturesLoaded = true;
}

Particle& ParticleSystem::AllocParticle()
{
    for (auto& p : pool)
        if (!p.alive) return p;

    Particle* oldest = &pool[0];
    for (auto& p : pool)
        if (p.life < oldest->life) oldest = &p;
    return *oldest;
}

void ParticleSystem::SpawnOne(ParticleEffectType type, float cx, float cy)
{
    Particle& p = AllocParticle();
    p.alive = true;

    p.x = cx + random_float(-55.0f, 55.0f);
    p.y = cy + random_float(-45.0f, 45.0f);

    switch (type)
    {
    case ParticleEffectType::HEAL:
        p.texture = texCross;
        p.vx = random_float(-15.0f, 15.0f);
        p.vy = random_float(-65.0f, -35.0f);
        p.maxLife = random_float(0.8f, 1.4f);
        p.scale = random_float(0.8f, 1.3f);
        p.scaleDelta = random_float(-0.15f, 0.05f);
        p.r = 80;  p.g = 220; p.b = 80;
        break;

    case ParticleEffectType::BUFF:
        p.texture = texArrow;
        p.vx = random_float(-10.0f, 10.0f);
        p.vy = random_float(-80.0f, -50.0f);
        p.maxLife = random_float(0.6f, 1.1f);
        p.scale = random_float(0.9f, 1.4f);
        p.scaleDelta = 0.0f;
        p.r = 80;  p.g = 160; p.b = 255;
        break;

    case ParticleEffectType::BLOOD:
        p.texture = texBlood;
        p.vx = random_float(-150.0f, 150.0f);
        p.vy = random_float(-180.0f, -50.0f);
        p.maxLife = random_float(0.4f, 0.9f);
        p.scale = random_float(0.8f, 1.6f);
        p.scaleDelta = -0.5f;
        p.r = 200; p.g = 0; p.b = 20;
        break;
    }

    p.life = p.maxLife;
}

void ParticleSystem::Emit(ParticleEffectType type, float cx, float cy)
{
    int count = 0;
    switch (type) {
        case ParticleEffectType::HEAL:  count = 18; break;
        case ParticleEffectType::BUFF:  count = 14; break;
        case ParticleEffectType::BLOOD: count = 22; break;
    }

    for (int i = 0; i < count; ++i) {
        SpawnOne(type, cx, cy);
    }
}

void ParticleSystem::Update(float dt)
{
    float dtSec = dt / 1000.0f;
    for (auto& p : pool) {
        if (!p.alive) continue;

        p.life -= dtSec;
        if (p.life <= 0.0f) {
            p.alive = false;
            continue;
        }

        p.vy += 130.0f * dtSec;
        p.x += p.vx * dtSec;
        p.y += p.vy * dtSec;
        p.scale = std::max(0.01f, p.scale + p.scaleDelta * dtSec);
    }
}

void ParticleSystem::Draw() const
{
    auto& render = *Engine::GetInstance().render;

    for (const auto& p : pool)
    {
        if (!p.alive || !p.texture) continue;

        float lifeRatio = p.life / p.maxLife;
        Uint8 alpha = (Uint8)(lifeRatio * 255.0f);

        float tw = 0.0f, th = 0.0f;
        SDL_GetTextureSize(p.texture, &tw, &th);

        int drawX = (int)p.x - (int)(tw * p.scale * 0.5f);
        int drawY = (int)p.y - (int)(th * p.scale * 0.5f);

        SDL_SetTextureColorMod(p.texture, p.r, p.g, p.b);
        SDL_SetTextureAlphaMod(p.texture, alpha);
        SDL_SetTextureBlendMode(p.texture, SDL_BLENDMODE_BLEND);

        SDL_Rect section = { 0, 0, (int)tw, (int)th };

        render.DrawTexture(p.texture, drawX, drawY, 1.0f, &section, true, 0.0, INT_MAX, INT_MAX, p.scale);
    }

    for (const auto& p : pool) {
        if (!p.texture) continue;
        SDL_SetTextureColorMod(p.texture, 255, 255, 255);
        SDL_SetTextureAlphaMod(p.texture, 255);
    }
}

void ParticleSystem::Clear()
{
    for (auto& p : pool)
        p.alive = false;
}
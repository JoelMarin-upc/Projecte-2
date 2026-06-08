#pragma once
#include <vector>
#include <string>
#include <SDL3/SDL.h>

enum class ParticleEffectType {
    HEAL,
    BUFF,
    BLOOD,
};

struct Particle {
    float x = 0.0f, y = 0.0f;
    float vx = 0.0f, vy = 0.0f;
    float life = 0.0f;
    float maxLife = 0.0f;
    float scale = 1.0f;
    float scaleDelta = 0.0f;
    Uint8 r = 255, g = 255, b = 255;
    SDL_Texture* texture = nullptr;
    bool alive = false;
};

class ParticleSystem
{
public:
    ParticleSystem() = default;
    ~ParticleSystem() = default;

    void LoadTextures();

    void Emit(ParticleEffectType type, float cx, float cy);

    void Update(float dt);

    void Draw() const;

    void Clear();

private:
    Particle& AllocParticle();
    void SpawnOne(ParticleEffectType type, float cx, float cy);

    static float random_float(float min, float max);
    static int random_int(int min, int max);

    static constexpr int MAX_PARTICLES = 512;
    Particle pool[MAX_PARTICLES] = {};

    SDL_Texture* texBlood = nullptr;
    SDL_Texture* texCross = nullptr;
    SDL_Texture* texArrow = nullptr;

    bool texturesLoaded = false;
};
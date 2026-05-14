#pragma once

#include "Module.h"
#include <SDL3/SDL.h>
#include <vector>
#include <string>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake();

	// Called before quitting
	bool CleanUp();

    bool Update(float dt);
	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(int fx, int repeat = 0);

    void SetMusicVolume(float volume);
    void SetFxVolume(float volume);
    void UpdateMusicVolume();

    float GetMusicVolume() const { return musicVolume; }
    float GetFxVolume() const { return fxVolume; }
    float GetTargetMusicVolume() const { return targetVolume; }
    bool MusicPlaying();
    bool EnsureStreams();
    void StopFx() { if (sfx_stream_) SDL_ClearAudioStream(sfx_stream_);}
    std::string GetAudioPath(const char* node, const char* audioName);

    float pauseMultiplier = 1.0f; // 1.0 = normal, 0.3 = paused
private:
    
    float musicVolume = 1.f;
    float fxVolume = 1.f;
    //used for the pause menu, temporary volume (musicVolume used as the actual volume used at any times, target volume is the "real" volume)
    float targetVolume = 1.f;

    //fading
    float fadeSpeed = 0.0f;
    float fadeFactor;
    bool isFading = false;

    struct SoundData {
        SDL_AudioSpec spec{};  // source format
        Uint8* buf{ nullptr };
        Uint32 len{ 0 };  // bytes
    };

    // Device and default output format
    SDL_AudioDeviceID device_{ 0 };
    SDL_AudioSpec     device_spec_{};

    // Streams
    SDL_AudioStream* music_stream_{ nullptr }; // for background music (single)
    SDL_AudioStream* sfx_stream_{ nullptr };   // simple shared SFX stream

    // Loaded sounds
    SoundData music_data_{};
    std::vector<SoundData> sfx_; // 1-based indexing outwardly

    // helpers
    bool LoadWavFile(const char* path, SoundData& out);
    void FreeSound(SoundData& s);
    bool EnsureDeviceOpen();

};

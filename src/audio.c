#include "audio.h"
#include "engine.h"
#include "globals.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

Mix_Chunk *LoadSound(const char *filename) {
    char path[256];
    sprintf(path, "assets/sounds/%s", filename);
    Mix_Chunk *chunk = Mix_LoadWAV(path);
    if (!chunk) {
        SDL_Log("Mix_LoadWAV: %s", Mix_GetError());
        QuitEngine();
    }
    return chunk;
}

Mix_Music *LoadMusic(const char *filename) {
    char path[256];
    sprintf(path, "assets/musics/%s", filename);
    Mix_Music *music = Mix_LoadMUS(path);
    if (!music) {
        SDL_Log("Mix_LoadMUS: %s", Mix_GetError());
        QuitEngine();
    }
    return music;
}

void PlaySound(Mix_Chunk *sound) {
    if (sound) {
        if (Mix_PlayChannel(-1, sound, 0) < 0) {
            SDL_Log("Mix_PlayChannel: %s", Mix_GetError());
            QuitEngine();
        }
    }
}

void ChangeVolumeSound(Mix_Chunk *sound, const int volume) {
    if (Mix_VolumeChunk(sound, volume) < 0) {
        SDL_Log("Mix_VolumeChunk: %s", Mix_GetError());
        QuitEngine();
    }
}

void IncreaseVolumeSound(Mix_Chunk *sound, const int volume) {
    const int current = Mix_VolumeChunk(sound, -1);
    ChangeVolumeSound(sound, current + volume);
}

void DecreaseVolumeSound(Mix_Chunk *sound, const int volume) {
    const int current = Mix_VolumeChunk(sound, -1);
    ChangeVolumeSound(sound, current - volume);
}

void PlayMusic(Mix_Music *music, const int loops) {
    if (music) {
        if (Mix_PlayMusic(music, loops) < 0) {
            SDL_Log("Mix_PlayMusic: %s", Mix_GetError());
            QuitEngine();
        }
    }
}

void ChangeVolumeMusic(const int volume) {
    if (Mix_VolumeMusic(volume) < 0) {
        SDL_Log("Mix_VolumeMusic: %s", Mix_GetError());
        QuitEngine();
    }
}

void IncreaseVolumeMusic(const int volume) {
    ChangeVolumeMusic(Mix_VolumeMusic(-1) + volume);
}

void DecreaseVolumeMusic(const int volume) {
    ChangeVolumeMusic(Mix_VolumeMusic(-1) - volume);
}

void ResetVolumeMusic() {
    ChangeVolumeMusic(128);
}

void UnloadSound(Mix_Chunk *sound) {
    if (sound)
        Mix_FreeChunk(sound);
}

void UnloadMusic(Mix_Music *music) {
    if (music)
        Mix_FreeMusic(music);
}


#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>

Mix_Chunk *LoadSound(const char *filename);
Mix_Music *LoadMusic(const char *filename);

void PlaySound(Mix_Chunk *sound);
void ChangeVolumeSound(Mix_Chunk *sound, int volume);
void IncreaseVolumeSound(Mix_Chunk *sound, int volume);
void DecreaseVolumeSound(Mix_Chunk *sound, int volume);

void PlayMusic(Mix_Music *music, int loops);
void ChangeVolumeMusic(int volume);
void IncreaseVolumeMusic(int volume);
void DecreaseVolumeMusic(int volume);
void ResetVolumeMusic();

void UnloadSound(Mix_Chunk *sound);
void UnloadMusic(Mix_Music *music);

#endif // AUDIO_H


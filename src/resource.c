#include <stdio.h>
#include "constants.h"
#include "engine.h"
#include "resource.h"

TexturePack *CreateTexturePack(char *name, const int n, const _Bool mode, const SDL_Color color){
    TexturePack *texture_pack = malloc(sizeof(TexturePack));
    texture_pack->frames = malloc(n * sizeof(SDL_Texture*));
    texture_pack->frames_highlighted = NULL;
    texture_pack->frame_dead = NULL;
    texture_pack->num_frames = n;
    texture_pack->reverse_frame_mode = mode;
    for (int i = 0; i < n; i++) {
        char filename[256];
        sprintf(filename, "%s%d.png", name, i + 1);
        SDL_Surface *tmp = LoadImage(filename);
        ReplaceColor(tmp, GREEN, color);
        texture_pack->frames[i] = LoadTextureFromImage(tmp);
    }
    return texture_pack;
}

TexturePack *CreateTexturePackEXT(char *name, const int n, const _Bool mode, const SDL_Color color){
    TexturePack *texture_pack = malloc(sizeof(TexturePack));
    texture_pack->frames = malloc(n * sizeof(SDL_Texture*));
    texture_pack->frames_highlighted = malloc(n * sizeof(SDL_Texture*));
    texture_pack->num_frames = n;
    texture_pack->reverse_frame_mode = mode;
    char filename[256]; //= malloc((int)strlen(name) + 1);
    for (int i = 0; i < n; i++) {
        sprintf(filename, "%s%d.png", name, i + 1);
        SDL_Surface *tmp = LoadImage(filename);
        ReplaceColor(tmp, GREEN, color);
        texture_pack->frames[i] = LoadTextureFromImage(tmp);
        tmp = LoadImage(filename);
        ReplaceColor(tmp, GREEN, color);
        HighlightFilter(tmp, 127);
        texture_pack->frames_highlighted[i] = LoadTextureFromImage(tmp);
    }
    sprintf(filename, "%s_dead.png", name);
    texture_pack->frame_dead = LoadTexture(filename);
    return texture_pack;
}

void AssignTexturePackToEntity(Entity *entity, const TexturePack *texture_pack){
    entity->frames = texture_pack->frames;
    entity->frames_highlighted = texture_pack->frames_highlighted;
    entity->frame_dead = texture_pack->frame_dead;
    entity->num_frames = texture_pack->num_frames;
    entity->reverse_frame_mode = texture_pack->reverse_frame_mode;
    entity->frame_index = 0;
}

void DestroyTexturePack(TexturePack *texture_pack){
    if (texture_pack->frames){
        for (int i = 0; i < texture_pack->num_frames; i++)
            if (texture_pack->frames[i])
                UnloadTexture(texture_pack->frames[i]);
        free(texture_pack->frames);
    }
    if (texture_pack->frames_highlighted){
        for (int i = 0; i < texture_pack->num_frames; i++) {
            if (texture_pack->frames_highlighted[i])
                UnloadTexture(texture_pack->frames_highlighted[i]);
        }
        free(texture_pack->frames_highlighted);
    }
    if (texture_pack->frame_dead)
        UnloadTexture(texture_pack->frame_dead);
    free(texture_pack);
}

SoundPack *CreateSoundPack(char *name, const int n){
    SoundPack *sound_pack = malloc(sizeof(SoundPack));
    sound_pack->sounds = malloc(n * sizeof(Mix_Chunk*));
    sound_pack->num_sounds = n;
    for (int i = 0; i < n; i++) {
        char filename[256];
        sprintf(filename, "%s%d.wav", name, i + 1);
        Mix_Chunk *tmp = LoadSound(filename);
        DecreaseVolumeSound(tmp, 77);
        sound_pack->sounds[i] = tmp;
    }
    return sound_pack;
}

void AssignSoundPackToEntity(Entity *entity, const SoundPack *sound_pack){
    entity->sounds = sound_pack->sounds;
    entity->num_sounds = sound_pack->num_sounds;
}

void DestroySoundPack(SoundPack *sound_pack){
    if (sound_pack->sounds){
        for (int i = 0; i < sound_pack->num_sounds; i++)
            if (sound_pack->sounds[i]) UnloadSound(sound_pack->sounds[i]);
        free(sound_pack->sounds);
    }
    free(sound_pack);
}


#ifndef RESOURCE_H
#define RESOURCE_H

#include "structures.h"

TexturePack *CreateTexturePack(char *name, int n, _Bool mode, SDL_Color color);
TexturePack *CreateTexturePackEXT(char *name, int n, _Bool mode, SDL_Color color);
void AssignTexturePackToEntity(Entity *entity, const TexturePack *texture_pack);
void DestroyTexturePack(TexturePack *texture_pack);
SoundPack *CreateSoundPack(char *name, int n);
void AssignSoundPackToEntity(Entity *entity, const SoundPack *sound_pack);
void DestroySoundPack(SoundPack *sound_pack);

#endif /* RESOURCE_H */

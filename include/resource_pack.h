#ifndef RESOURCE_H
#define RESOURCE_H

#include "structures.h"
#include <SDL2/SDL.h>

// Fonctions pour la gestion d'un pack de textures (pour les animations d'entités)
AnimationPack* CreateAnimationPack(const char *base_name, const char *anim_name, int mode, SDL_Color color, int generate_highlight);
TexturePack* CreateTexturePack(const char *base_name, SDL_Color color, int generate_highlight, int has_death_animation);
void DestroyAnimationPack(AnimationPack *animPack);
void DestroyTexturePack(TexturePack *texture_pack);
void AssignTexturePackToEntity(Entity *entity, TexturePack *texture_pack);

// Fonctions pour la gestion d'un pack de sons
SoundPack *CreateSoundPack(char *name, int n);
void AssignSoundPackToEntity(Entity *entity, const SoundPack *sound_pack);
void DestroySoundPack(SoundPack *sound_pack);

#endif // RESOURCE_H


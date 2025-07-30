#include "resource_pack.h"
#include "images.h"
#include "audio.h"
#include "constants.h"
#include "graphics.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

AnimationPack* CreateAnimationPack(const char *base_name, const char *anim_name,
                                   const int mode, const SDL_Color color,
                                   const int generate_highlight) {
    AnimationPack *animPack = malloc(sizeof(AnimationPack));
    if (!animPack) {
        fprintf(stderr, "Erreur d'allocation pour AnimationPack\n");
        return NULL;
    }

    animPack->reverse_frame_mode = mode;
    animPack->specific_texture = NULL;
    animPack->frames = NULL;
    animPack->frames_highlighted = NULL;
    animPack->num_frames = 0;

    char filename[256];
    int count = 0;

    if (strcmp(anim_name, "death") == 0) {
        sprintf(filename, "%s_death.png", base_name);
        SDL_Surface *tmp = LoadImage(filename);
        if (!tmp) {
            fprintf(stderr, "Erreur de chargement de l'image de mort : %s\n", filename);
        } else {
            if (generate_highlight) {
                ReplaceColor(tmp, GREEN, color);
            }
            animPack->specific_texture = LoadTextureFromImage(tmp);
            // Aucun SDL_FreeSurface(tmp) ici puisque LoadTextureFromImage() s'en charge
        }
        animPack->num_frames = 1;
        return animPack;
    }

    /* Recherche des frames nommées "assets/images/<base_name>1.png", "assets/images/<base_name>2.png", etc. */
    while (1) {
        sprintf(filename, "assets/images/%s%d.png", base_name, count + 1);
        if (!file_exists(filename))
            break;
        count++;
    }

    if (count == 0) {
        fprintf(stderr, "Aucune frame trouvée pour l'animation \"%s\" à partir de %s\n", anim_name, filename);
        free(animPack);
        return NULL;
    }

    animPack->num_frames = count;

    /* Allocation des tableaux de textures.
     * Si generate_highlight est faux, seule la version de frames de base est allouée.
     */
    animPack->frames = malloc(count * sizeof(SDL_Texture *));
    if (generate_highlight)
        animPack->frames_highlighted = malloc(count * sizeof(SDL_Texture *));
    else
        animPack->frames_highlighted = NULL;

    if (!animPack->frames || (generate_highlight && !animPack->frames_highlighted)) {
        fprintf(stderr, "Erreur d'allocation pour les tableaux de frames\n");
        if (animPack->frames)
            free(animPack->frames);
        free(animPack);
        return NULL;
    }

    /* Chargement des images pour chaque frame */
    for (int i = 0; i < count; i++) {
        sprintf(filename, "%s%d.png", base_name, i + 1);
        SDL_Surface *tmp = LoadImage(filename);
        if (!tmp) {
            fprintf(stderr, "Erreur de chargement de l'image %s\n", filename);
            animPack->frames[i] = NULL;
            if (generate_highlight)
                animPack->frames_highlighted[i] = NULL;
        } else {
            if (generate_highlight) {

                /* Version normale */
                SDL_Surface *tmp_normal = SDL_ConvertSurface(tmp, tmp->format, 0);
                if (tmp_normal) {
                    ReplaceColor(tmp_normal, GREEN, color);
                    animPack->frames[i] = LoadTextureFromImage(tmp_normal);
                    // Ne pas appeler SDL_FreeSurface(tmp_normal) ici (LoadTextureFromImage s'en charge)
                } else {
                    animPack->frames[i] = NULL;
                }

                /* Version highlighted */
                SDL_Surface *tmp_highlight = SDL_ConvertSurface(tmp, tmp->format, 0);
                if (tmp_highlight) {
                    ReplaceColor(tmp_highlight, GREEN, color);
                    HighlightFilter(tmp_highlight, 127);
                    animPack->frames_highlighted[i] = LoadTextureFromImage(tmp_highlight);
                    // Ne pas appeler SDL_FreeSurface(tmp_highlight) ici
                } else {
                    animPack->frames_highlighted[i] = NULL;
                }
                SDL_FreeSurface(tmp);
            } else {
                /* Pour les entités (comme les projectiles) sans surlignage */
                animPack->frames[i] = LoadTextureFromImage(tmp);
            }
            // Ne pas appeler SDL_FreeSurface(tmp) ici non plus, puisque c'est géré par LoadTextureFromImage
        }
        /*if (tmp)
            SDL_FreeSurface(tmp);*/
    }

    return animPack;
}

/* ----------------------------------------------------------------------------
 * CreateTexturePackGeneral
 *
 * Fonction généralisée pour créer un TexturePack.
 *
 * Paramètres :
 *   - base_name           : la base du nom des fichiers (ex. "drone" ou "bullet")
 *   - color               : couleur de remplacement (utile pour les entités qui le nécessitent)
 *   - generate_highlight  : 1 pour générer les frames highlighted, 0 sinon.
 *   - has_death_animation : 1 si une animation de mort doit être chargée.
 *   - has_attack_animation et has_moving_animation : gestion optionnelle d'autres animations.
 *
 * Exemple :
 *   Pour un drone (animation idle, mort + highlighted) :
 *      CreateTexturePackGeneral("drone", droneColor, 1, 1, 0, 0);
 *
 *   Pour un projectile (seulement idle, sans highlighted ni mort) :
 *      CreateTexturePackGeneral("bullet", (SDL_Color){0,0,0,0}, 0, 0, 0, 0);
 * ----------------------------------------------------------------------------
 */
TexturePack* CreateTexturePack(const char *base_name, const SDL_Color color,
                                      const int generate_highlight, const int has_death_animation/*,
                                      const int has_attack_animation, const int has_moving_animation*/) {
    if (!base_name)
        return NULL;

    TexturePack *pack = malloc(sizeof(TexturePack));
    if (!pack) {
        fprintf(stderr, "Erreur d'allocation pour TexturePack\n");
        return NULL;
    }
    //pack->name = strdup(base_name);
    //pack->animations = malloc(NUM_ANIMATION_TYPES * sizeof(AnimationPack*));
    /* L'animation idle est toujours présente */
    pack->animations[ANIM_IDLE] = CreateAnimationPack(base_name, "idle", 0, color, generate_highlight);

    /* Les autres animations sont chargées selon les flags */
    /*if (has_attack_animation)
        pack->animations[ANIM_ATTACK] = CreateAnimationPack(base_name, "attack", 0, color, generate_highlight);
    else
        pack->animations[ANIM_ATTACK] = NULL;

    if (has_moving_animation)
        pack->animations[ANIM_MOVING] = CreateAnimationPack(base_name, "moving", 0, color, generate_highlight);
    else
        pack->animations[ANIM_MOVING] = NULL;*/

    if (has_death_animation)
        pack->animations[ANIM_DEATH] = CreateAnimationPack(base_name, "death", 0, color, generate_highlight);
    else
        pack->animations[ANIM_DEATH] = NULL;

    return pack;
}

/* ----------------------------------------------------------------------------
 * DestroyAnimationPack
 * Libère les ressources associées à une AnimationPack.
 * ----------------------------------------------------------------------------
 */
void DestroyAnimationPack(AnimationPack *animPack) {
    if (!animPack)
        return;

    if (animPack->frames) {
        for (int i = 0; i < animPack->num_frames; i++) {
            if (animPack->frames[i])
                UnloadTexture(animPack->frames[i]);
        }
        free(animPack->frames);
    }

    if (animPack->frames_highlighted) {
        for (int i = 0; i < animPack->num_frames; i++) {
            if (animPack->frames_highlighted[i])
                UnloadTexture(animPack->frames_highlighted[i]);
        }
        free(animPack->frames_highlighted);
    }

    if (animPack->specific_texture)
        UnloadTexture(animPack->specific_texture);

    free(animPack);
}

/* ----------------------------------------------------------------------------
 * DestroyTexturePack
 * Libère toutes les ressources associées à un TexturePack.
 * ----------------------------------------------------------------------------
 */
void DestroyTexturePack(TexturePack *texture_pack) {
    if (!texture_pack)
        return;

    //if (texture_pack->animations){
    for (int i = 0; i < NUM_ANIMATION_TYPES; i++) {
        if (texture_pack->animations[i])
            DestroyAnimationPack(texture_pack->animations[i]);
    }
        //free(texture_pack->animations);
    //}

    //free(texture_pack->name);
    free(texture_pack);
}

void AssignTexturePackToEntity(Entity *entity, TexturePack *texture_pack) {
    if (!entity || !texture_pack) {
        fprintf(stderr, "AssignTexturePackToEntity: paramètre NULL.\n");
        return;
    }
    entity->texture_pack = texture_pack;
    entity->frame_index = 0;
    entity->current_animation = ANIM_IDLE;
}

SoundPack *CreateSoundPack(char *name, const int n) {
    SoundPack *sound_pack = malloc(sizeof(SoundPack));
    sound_pack->sounds = malloc(n * sizeof(Mix_Chunk *));
    sound_pack->num_sounds = n;
    sound_pack->name = name;//strdup(name);
    for (int i = 0; i < n; i++) {
        char filename[256];
        sprintf(filename, "%s%d.wav", name, i + 1);
        Mix_Chunk *tmp = LoadSound(filename);
        DecreaseVolumeSound(tmp, 77);
        sound_pack->sounds[i] = tmp;
    }
    return sound_pack;
}

void AssignSoundPackToEntity(Entity *entity, const SoundPack *sound_pack) {
    entity->sounds = sound_pack->sounds;
    entity->num_sounds = sound_pack->num_sounds;
}

void DestroySoundPack(SoundPack *sound_pack) {
    if (sound_pack->sounds) {
        for (int i = 0; i < sound_pack->num_sounds; i++) {
            if (sound_pack->sounds[i])
                UnloadSound(sound_pack->sounds[i]);
        }
        free(sound_pack->sounds);
    }
    //free(sound_pack->name);
    free(sound_pack);
}


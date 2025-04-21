#include "audio.h"
#include "constants.h"
#include "entity.h"
#include "entity_list.h"
#include "globals.h"
#include "graphics.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Entity *CreateEntity(void) {
    Entity *entity = malloc(sizeof(Entity));
    //if (!entity)
    //    return NULL;
    entity->AI = 0;
    entity->AI_lifetime = 0;
    entity->ID = entity_count++;
    entity->frames = NULL;
    entity->frames_highlighted = NULL;
    entity->frame_dead = NULL;
    entity->mark = NULL;
    entity->sounds = NULL;
    entity->projectiles = NULL;
    return entity;
}

void InitEntity(Entity *entity, const float width, const float height, const int category, const double HP, const double damage, const double m_damage, const double speed, const int rate_min, const int rate_max, const int precision, const int stability) {
    entity->hitpoints = HP;
    entity->hitpoints_max = HP;
    entity->damage = damage;
    entity->melee_damage = m_damage;
    entity->speed = speed;
    entity->velocity_x = 0;
    entity->velocity_y = 0;
    entity->shoot_rate_min = rate_min;
    entity->shoot_rate_max = rate_max;
    entity->shoot_delay = 0;
    entity->aim_delay = 0;
    entity->precision = precision;
    entity->stability = stability;
    entity->aiming = 0;
    entity->elementary = 0;
    entity->effect = 0;
    entity->highlight = 0;
    entity->category = category;
    entity->prestige = 0;
    entity->frame_time = (int)(REFRESH_RATE / 20.0); // TIME = 20 (par exemple)
    entity->frame_delay = 0;
    entity->hitbox.x = 0;
    entity->hitbox.y = 0;
    entity->hitbox.w = (float)(width / 1920.0 * WIDTH);
    entity->hitbox.h = (float)(height / 1080.0 * HEIGHT);
    entity->target.x = 0;
    entity->target.y = 0;
    entity->target.w = 0;
    entity->target.h = 0;
    entity->projectiles = CreateEntityList();
}

void CloneEntity(Entity *clone, const Entity *entity) {
    clone->hitpoints = entity->hitpoints;
    clone->hitpoints_max = entity->hitpoints_max;
    clone->damage = entity->damage;
    clone->melee_damage = entity->melee_damage;
    clone->speed = entity->speed;
    clone->velocity_x = entity->velocity_x;
    clone->velocity_y = entity->velocity_y;
    clone->shoot_rate_min = entity->shoot_rate_min;
    clone->shoot_rate_max = entity->shoot_rate_max;
    clone->shoot_delay = entity->shoot_delay;
    clone->aim_delay = entity->aim_delay;
    clone->precision = entity->precision;
    clone->stability = entity->stability;
    clone->aiming = entity->aiming;
    clone->elementary = entity->elementary;
    clone->effect = entity->effect;
    clone->highlight = entity->highlight;
    clone->category = entity->category;
    clone->prestige = entity->prestige;
    clone->frame_time = entity->frame_time;
    clone->frame_delay = entity->frame_delay;
    clone->frame_index = entity->frame_index;
    clone->reverse_frame_mode = entity->reverse_frame_mode;
    clone->num_frames = entity->num_frames;
    clone->num_sounds = entity->num_sounds;
    clone->extended_frames = entity->extended_frames;
    clone->hitbox = entity->hitbox;
    clone->target = entity->target;
    clone->frames = entity->frames;
    clone->frames_highlighted = entity->frames_highlighted;
    clone->frame_dead = entity->frame_dead;
    clone->mark = entity->mark;
    clone->sounds = entity->sounds;
    clone->projectiles = entity->projectiles;
}

void DestroyEntity(Entity *entity) {
    if (entity->projectiles)
        DestroyEntityList(entity->projectiles);
    free(entity);
}

void UpdateEntityState(Entity *entity) {
    if (entity->AI) {
        if (!entity->AI_lifetime)
            DisableEntityAI(entity);
        else if (entity->AI_lifetime > 0)
            entity->AI_lifetime--;
    }
    if (!entity->frame_delay) {
        entity->frame_delay = entity->frame_time;
        if (entity->extended_frames)
            entity->frame_delay += (int)(0.5 * entity->frame_time * entity->velocity_y);
        if (entity->reverse_frame_mode == 0 && ++entity->frame_index == entity->num_frames)
            entity->frame_index = 0;
        else if (entity->reverse_frame_mode > 0 && ++entity->frame_index == entity->num_frames) {
            entity->frame_index--;
            entity->reverse_frame_mode *= -1;
        } else if (entity->reverse_frame_mode < 0 && --entity->frame_index == -1) {
            entity->frame_index++;
            entity->reverse_frame_mode *= -1;
        }
    } else {
        entity->frame_delay--;
    }
    if (entity->shoot_delay > 0)
        entity->shoot_delay--;
    if (entity->effect > 0)
        entity->effect--;
    if (entity->highlight > 0)
        entity->highlight--;
}

void DisplayEntity(const Entity *entity) {
    // Vérifier que l'entité n'est pas NULL pour éviter d'accéder à un pointeur invalide.
    if (!entity)
        return;
    // Calculer une seule fois la conversion de la hitbox pour éviter des appels répétés.
    const SDL_Rect rect = FRectToRect(entity->hitbox);
    // S'assurer que frame_index est dans les bornes pour éviter un accès hors-limites.
    int idx = entity->frame_index;
    if (idx < 0 || idx >= entity->num_frames)
        idx = 0;  // On peut opter pour un index par défaut ou logger l'erreur.
    // Début du dessin en fonction de l'état de l'entité.
    if (IsEntityAlive(entity)) {
        // Si l'entité utilise des extended_frames et qu'elle doit être surlignée.
        if (entity->extended_frames && entity->highlight) {
            // Vérifier que le tableau des textures surlignées est bien alloué et que la texture existe.
            if (entity->frames_highlighted && entity->frames_highlighted[idx]) {
                DrawTexture(entity->frames_highlighted[idx], rect);
            }
            // Sinon, on retombe sur le tableau classique, en s'assurant qu'il est valide.
            else if (entity->frames && entity->frames[idx]) {
                DrawTexture(entity->frames[idx], rect);
            }
            // Si aucune texture n'est valide, dessiner un rectangle de fallback.
            else {
                DrawRectF(entity->hitbox, 255, 255, 255, 255);
            }
        }
        // Si l'entité n'est pas en extended_frames ou ne demande pas de surlignage.
        else if (entity->frames && entity->frames[idx]) {
            DrawTexture(entity->frames[idx], rect);
        }
        else {
            DrawRectF(entity->hitbox, 255, 255, 255, 255);
        }
    }
    // Cas où l'entité n'est pas vivante.
    else if (entity->extended_frames) {
        if (entity->frame_dead)
            DrawTexture(entity->frame_dead, rect);
        else
            DrawRectF(entity->hitbox, 255, 255, 255, 255);
    }
    // Dessiner la marque associée, si présente.
    if (entity->mark)
        DrawTexture(entity->mark, rect);
}


void MoveEntity(Entity *entity, const _Bool pass) {
    double x = entity->velocity_x * entity->speed, y = entity->velocity_y * entity->speed;
    if (pass || (entity->hitbox.x + x >= 0 && entity->hitbox.x + entity->hitbox.w + x < WIDTH)) {
        if (entity->effect) x /= 2.0;
        entity->hitbox.x += (float)x;
    }
    if (pass || (entity->hitbox.y + y >= 0 && entity->hitbox.y + entity->hitbox.h + y < HEIGHT)) {
        if (entity->effect) y /= 2.0;
        entity->hitbox.y += (float)y;
    }
}

void MoveEntityAndCursor(Entity *entity, const _Bool pass, const int cursor_x, const int cursor_y) {
    double x = entity->velocity_x * entity->speed, y = entity->velocity_y * entity->speed;
    if (pass || (entity->hitbox.x + x >= 0 && entity->hitbox.x + entity->hitbox.w + x < WIDTH)) {
        if (entity->effect) x /= 2.0;
        entity->hitbox.x += (float)x;
    }
    if (pass || (entity->hitbox.y + y >= 0 && entity->hitbox.y + entity->hitbox.h + y < HEIGHT)) {
        if (entity->effect) y /= 2.0;
        entity->hitbox.y += (float)y;
    }
    SDL_WarpMouseInWindow(window, cursor_x + (int)x, cursor_y + (int)y);
}

void SummonEntity(Entity *entity, const int lifetime, const float x, const float y) {
    EnableEntityAI(entity, lifetime);
    SetPositionF(&entity->hitbox, x, y);
    entity->hitpoints = entity->hitpoints_max;
    entity->velocity_x = 0;
    entity->velocity_y = 0;
}

void DisplayEntityLaserBeam(const Entity *entity){
    if (entity->aiming){
        const int xa = (int)(entity->hitbox.x + entity->hitbox.w / 2.0);
        const int ya = (int)(entity->hitbox.y + entity->hitbox.h / 2.0);
        const int xb = (int)(entity->target.x + entity->target.w / 2.0);
        const int yb = (int)(entity->target.y + entity->target.h / 2.0);
        for (int i = -1; i <= 1; i++) {
            DrawLine(xa + i, ya, xb + i, yb, 255, 0, 0, 255);
            DrawLine(xa, ya + i, xb, yb + i, 255, 0, 0, 255);
        }
    }
}

void EntityCease(Entity *entity){
    if (entity->aiming){
        entity->aiming = 0;
        entity->aim_delay = TimeToFrames(1);
    }
}

void EntityAim(Entity *entity, const float x, const float y, const float w, const float h) {
    if (!entity->shoot_delay && entity->aim_delay){
        entity->aiming = 1;
        entity->target.x = x;
        entity->target.y = y;
        entity->target.w = w;
        entity->target.h = h;
        entity->aim_delay--;
        if (!entity->aim_delay)
            EntityCease(entity);
    }

}

void EntityShoot(Entity *entity, EntityList *projectiles, const Entity *ammo, const float x, const float y, const float w, const float h) {
    if (!entity->aiming && !entity->shoot_delay) {
        Entity *projectile = CreateEntity();
        CloneEntity(projectile, ammo);
        EnableEntityAI(projectile, TimeToFrames(5));
        projectile->hitpoints = 1;
        projectile->melee_damage = entity->damage;
        const double xa = entity->hitbox.x + entity->hitbox.w / 2.0;
        const double ya = entity->hitbox.y + entity->hitbox.h / 2.0;
        const double xb = x + w / 2.0;
        const double yb = y + h / 2.0;
        const double alpha = atan2(yb - ya, xb - xa);
        projectile->velocity_x = SetVelocityX(cos(alpha) * (1 + randint(-100 + entity->precision, 100 - entity->precision) / 100.0));
        projectile->velocity_y = SetVelocityY(sin(alpha) * (1 + randint(-100 + entity->precision, 100 - entity->precision) / 100.0));
        projectile->hitbox.x = (float)(entity->hitbox.x + entity->hitbox.w / 2.0 - projectile->hitbox.w / 2.0);
        projectile->hitbox.y = (float)(entity->hitbox.y + entity->hitbox.h / 2.0 - projectile->hitbox.h / 2.0);
        entity->shoot_delay = TimeToFrames(randint(entity->shoot_rate_min, entity->shoot_rate_max) / 60.0);
        if (entity->effect) entity->shoot_delay *= 2;
        //if (entity->category == 3) entity->aim_delay = TimeToFrames(1);
        if (entity->sounds)
            PlaySound(entity->sounds[randint(0, entity->num_sounds - 1)]);
        AddEntity(projectiles, projectile);
    }
}

void EntityIntercept(Entity *entity, const float x, const float y, const float w, const float h) {
    const double xa = entity->hitbox.x + entity->hitbox.w / 2.0;
    const double ya = entity->hitbox.y + entity->hitbox.h / 2.0;
    const double xb = x + w / 2.0;
    const double yb = y + h / 2.0;
    const double alpha = atan2(yb - ya, xb - xa);
    entity->velocity_x = SetVelocityX(cos(alpha) * (1 + randint(-100 + entity->stability, 100 - entity->stability) / 100.0));
    entity->velocity_y = SetVelocityY(sin(alpha) * (1 + randint(-100 + entity->stability, 100 - entity->stability) / 100.0));
}

void HandleCollision(Entity **entity1, Entity **entity2) {
    if (Intersecting((*entity1)->hitbox, (*entity2)->hitbox)) {
        (*entity1)->hitpoints -= (*entity2)->melee_damage;
        (*entity2)->hitpoints -= (*entity1)->melee_damage;
        (*entity1)->highlight = TimeToFrames(0.1);
        (*entity2)->highlight = TimeToFrames(0.1);
        if ((*entity1)->elementary == 1)
            (*entity2)->effect = TimeToFrames(1.5);
        if ((*entity2)->elementary == 1)
            (*entity1)->effect = TimeToFrames(1.5);
    }
}

void DisableEntityAI(Entity *entity) {
    entity->AI = 0;
}

void EnableEntityAI(Entity *entity, const int lifetime) {
    entity->AI = 1;
    entity->AI_lifetime = lifetime;
}

_Bool AreEntitiesNear(const Entity *entity1, const Entity *entity2, const double threshold) {
    return distance(entity1->hitbox.x, entity1->hitbox.y, entity2->hitbox.x, entity2->hitbox.y) < threshold;
}

_Bool AreEntitiesFar(const Entity *entity1, const Entity *entity2, const double threshold) {
    return distance(entity1->hitbox.x, entity1->hitbox.y, entity2->hitbox.x, entity2->hitbox.y) > threshold;
}

_Bool IsEntityAIEnabled(const Entity *entity) {
    return entity->AI;
}

_Bool IsEntityAIDisabled(const Entity *entity) {
    return !entity->AI;
}

_Bool IsEntityAlive(const Entity *entity) {
    return entity->hitpoints > 0;
}

_Bool IsEntityDead(const Entity *entity) {
    return entity->hitpoints <= 0;
}


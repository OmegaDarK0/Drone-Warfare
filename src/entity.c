#include <math.h>
#include <stdio.h>
#include "entity.h"
#include "constants.h"
#include "engine.h"
#include "entity_list.h"
#include "globals.h"
#include "utils.h"

Entity *CreateEntity(){
    Entity *entity = malloc(sizeof(Entity));
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

void InitEntity(Entity *entity, const float width, const float height, const int category, const double HP,
                const double damage, const double m_damage, const double speed, const int rate_min, const int rate_max,
                const int precision, const int stability){
    // FRONT
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
    // MIDDLE
    entity->elementary = 0;
    entity->effect = 0;
    entity->highlight = 0;
    entity->category = category;    // UNUSED
    entity->prestige = 0;
    // BACK
    entity->frame_time = (int)(REFRESH_RATE / TIME);
    entity->frame_delay = 0;
    //entity->frame_index = 0;
    //entity->reverse_frame_mode = 0;
    //entity->num_frames = 0;
    //entity->num_sounds = 0;
    entity->hitbox.x = 0;
    entity->hitbox.y = 0;
    entity->hitbox.w = (float)(width / 1920. * WIDTH);
    entity->hitbox.h = (float)(height / 1080. * HEIGHT);
    //entity->frames;
    //entity->sounds;
    entity->projectiles = CreateEntityList();
}

void CloneEntity(Entity *clone, const Entity *entity){
    // FRONT
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
    // MIDDLE
    clone->elementary = entity->elementary;
    clone->effect = entity->effect;
    clone->highlight = entity->highlight;
    clone->category = entity->category;
    clone->prestige = entity->prestige;
    // BACK
    clone->frame_time = entity->frame_time;
    clone->frame_delay = entity->frame_delay;
    clone->frame_index = entity->frame_index;
    clone->reverse_frame_mode = entity->reverse_frame_mode;
    clone->num_frames = entity->num_frames;
    clone->num_sounds = entity->num_sounds;
    clone->hitbox = entity->hitbox;
    clone->frames = entity->frames;
    clone->frames_highlighted = entity->frames_highlighted;
    clone->frame_dead = entity->frame_dead;
    clone->mark = entity->mark;
    clone->sounds = entity->sounds;
    clone->projectiles = entity->projectiles;
}

void DestroyEntity(Entity *entity){
    if (entity->projectiles)
        DestroyEntityList(entity->projectiles);
    free(entity);
}

void UpdateEntityState(Entity *entity){
    if (entity->AI){
        if (!entity->AI_lifetime) DisableEntityAI(entity);
        else if (entity->AI_lifetime > 0) entity->AI_lifetime--;
    }
    if (!entity->frame_delay){
        entity->frame_delay = entity->frame_time;
        if (entity->reverse_frame_mode == 0 && ++entity->frame_index == entity->num_frames)
            entity->frame_index = 0;
        else if (entity->reverse_frame_mode > 0 && ++entity->frame_index == entity->num_frames){
                entity->frame_index--;
                entity->reverse_frame_mode *= -1;
        }
        else if (entity->reverse_frame_mode < 0 && --entity->frame_index == -1){
                entity->frame_index++;
                entity->reverse_frame_mode *= -1;
        }
    }
    else entity->frame_delay--;
    if (entity->shoot_delay > 0) entity->shoot_delay--;
    if (entity->effect > 0) entity->effect--;
    if (entity->highlight > 0) entity->highlight--;
}

void DisplayEntity(const Entity *entity){
    // ENTITY ALIVE
    if (IsEntityAlive(entity)) {
        if (entity->highlight && entity->frames_highlighted && entity->frames_highlighted[entity->frame_index])
            DrawTexture(entity->frames_highlighted[entity->frame_index], FRectToRect(entity->hitbox));
        else if (entity->frames[entity->frame_index])
            DrawTexture(entity->frames[entity->frame_index], FRectToRect(entity->hitbox));
        else DrawRectF(entity->hitbox, 255, 255, 255, 255);
    }
    // ENTITY DEAD
    else {
        if (entity->frame_dead)
            DrawTexture(entity->frame_dead, FRectToRect(entity->hitbox));
        else DrawRectF(entity->hitbox, 255, 255, 255, 255);
    }
    // ENTITY MARK
    if (entity->mark)
        DrawTexture(entity->mark, FRectToRect(entity->hitbox));
    if (debug){
        char text[16];
        sprintf(text, "%d", entity->ID);
        SDL_Texture *ID = LoadTextureFromText(font_medium, text, WHITE);
        const SDL_Rect rect = {(int)entity->hitbox.x, (int)entity->hitbox.y, 32, 32};
        DrawTexture(debug_hitbox, FRectToRect(entity->hitbox));
        DrawTexture(ID, rect);
        UnloadTexture(ID);
    }
}

void MoveEntity(Entity *entity, const _Bool pass){
    double x = entity->velocity_x * entity->speed, y = entity->velocity_y * entity->speed;
    if (pass || (entity->hitbox.x + x >= 0 && entity->hitbox.x + entity->hitbox.w + x < WIDTH)){
        if (entity->effect) x /= 2.;
        entity->hitbox.x += (float)x;
    }
    if (pass || (entity->hitbox.y + y >= 0 && entity->hitbox.y + entity->hitbox.h + y < HEIGHT)) {
        if (entity->effect) y /= 2.;
        entity->hitbox.y += (float)y;
    }
}

void MoveEntityAndCursor(Entity *entity, const _Bool pass, const int cursor_x, const int cursor_y){
    double x = entity->velocity_x * entity->speed, y = entity->velocity_y * entity->speed;
    if (pass || (entity->hitbox.x + x >= 0 && entity->hitbox.x + entity->hitbox.w + x < WIDTH)){
        if (entity->effect) x /= 2.;
        entity->hitbox.x += (float)x;
    }
    if (pass || (entity->hitbox.y + y >= 0 && entity->hitbox.y + entity->hitbox.h + y < HEIGHT)) {
        if (entity->effect) y /= 2.;
        entity->hitbox.y += (float)y;
    }
    SDL_WarpMouseInWindow(window, (int)(cursor_x + x), (int)(cursor_y + y));
}

void SummonEntity(Entity *entity, const int lifetime, const float x, const float y){
    EnableEntityAI(entity, lifetime);
    SetPositionF(&entity->hitbox, x, y);
    entity->hitpoints = entity->hitpoints_max;
    entity->velocity_x = 0;
    entity->velocity_y = 0;
}

void EntityShoot(Entity *entity, EntityList *projectiles, const Entity *ammo, const float x, const float y, const float w, const float h){
    if (!entity->shoot_delay && !entity->aim_delay){
        Entity *projectile = CreateEntity();
        CloneEntity(projectile, ammo);
        EnableEntityAI(projectile, TimeToFrames(5));
        projectile->hitpoints = 1;
        projectile->melee_damage = entity->damage;
        const double xa = entity->hitbox.x + entity->hitbox.w / 2.;
        const double ya = entity->hitbox.y + entity->hitbox.h / 2.;
        const double xb = x + w / 2.;
        const double yb = y + h / 2.;
        const double alpha = atan2(yb - ya, xb - xa);
        projectile->velocity_x = SetVelocityX(cos(alpha) * (1 + randint(-100 + entity->precision, 100 - entity->precision) / 100.));
        projectile->velocity_y = SetVelocityY(sin(alpha) * (1 + randint(-100 + entity->precision, 100 - entity->precision) / 100.));
        projectile->hitbox.x =
                (float) (entity->hitbox.x + entity->hitbox.w / 2. - projectile->hitbox.w / 2.);
        projectile->hitbox.y =
                (float) (entity->hitbox.y + entity->hitbox.h / 2. - projectile->hitbox.h / 2.);
        entity->shoot_delay = TimeToFrames(randint(entity->shoot_rate_min, entity->shoot_rate_max) / 60.);
        if (entity->effect) entity->shoot_delay *= 2;
        if (entity->category == 3) entity->aim_delay = TimeToFrames(1);
        if (entity->sounds) PlaySound(entity->sounds[randint(0, entity->num_sounds - 1)]);
        AddEntity(projectiles, projectile);
    }
}

void EntityAim(Entity *entity, const float x, const float y, const float w, const float h) {
    if (!entity->shoot_delay && entity->aim_delay) {
        const int xa = (int) (entity->hitbox.x + entity->hitbox.w / 2.);
        const int ya = (int) (entity->hitbox.y + entity->hitbox.h / 2.);
        const int xb = (int) (x + w / 2.);
        const int yb = (int) (y + h / 2.);
        DrawLine(xa, ya, xb, yb, 255, 0, 0, 255);
        DrawLine(xa + 1, ya, xb + 1, yb, 255, 0, 0, 255);
        DrawLine(xa - 1, ya, xb - 1, yb, 255, 0, 0, 255);
        DrawLine(xa, ya + 1, xb, yb + 1, 255, 0, 0, 255);
        DrawLine(xa, ya - 1, xb, yb - 1, 255, 0, 0, 255);
        entity->aim_delay--;
    }
}

void EntityIntercept(Entity *entity, const float x, const float y, const float w, const float h){
    const double xa = entity->hitbox.x + entity->hitbox.w / 2.;
    const double ya = entity->hitbox.y + entity->hitbox.h / 2.;
    const double xb = x + w / 2.;
    const double yb = y + h / 2.;
    const double alpha = atan2(yb - ya, xb - xa);
    entity->velocity_x = SetVelocityX(cos(alpha) * (1 + randint(-100 + entity->stability, 100 - entity->stability) / 100.));
    entity->velocity_y = SetVelocityY(sin(alpha) * (1 + randint(-100 + entity->stability, 100 - entity->stability) / 100.));
}

void HandleCollision(Entity **entity1, Entity **entity2){
    if (Intersecting((*entity1)->hitbox, (*entity2)->hitbox)){
        (*entity1)->hitpoints -= (*entity2)->melee_damage;
        (*entity2)->hitpoints -= (*entity1)->melee_damage;
        (*entity1)->highlight = TimeToFrames(0.1);
        (*entity2)->highlight = TimeToFrames(0.1);
        if ((*entity1)->elementary == 1) (*entity2)->effect = TimeToFrames(1.5);
        if ((*entity2)->elementary == 1) (*entity1)->effect = TimeToFrames(1.5);
    }
}

void DisableEntityAI(Entity *entity){
    entity->AI = 0;
}

void EnableEntityAI(Entity *entity, const int lifetime){
    entity->AI = 1;
    entity->AI_lifetime = lifetime;
}

_Bool AreEntitiesNear(const Entity *entity1, const Entity *entity2, const double threshold) {
    return distance(entity1->hitbox.x, entity1->hitbox.y, entity2->hitbox.x, entity2->hitbox.y) < threshold;
}

_Bool AreEntitiesFar(const Entity *entity1, const Entity *entity2, const double threshold) {
    return distance(entity1->hitbox.x, entity1->hitbox.y, entity2->hitbox.x, entity2->hitbox.y) > threshold;
}

_Bool IsEntityAIEnabled(const Entity *entity){
    return entity->AI;
}

_Bool IsEntityAIDisabled(const Entity *entity){
    return !entity->AI;
}

_Bool IsEntityAlive(const Entity *entity){
    return entity->hitpoints > 0;
}

_Bool IsEntityDead(const Entity *entity){
    return entity->hitpoints <= 0;
}

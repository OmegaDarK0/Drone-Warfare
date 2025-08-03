#include "audio.h"
#include "constants.h"
#include "entity.h"
#include "entity_list.h"
#include "globals.h"
#include "graphics.h"
#include "utils.h"
#include "hud.h"
#include "resource_pack.h"
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
    /*entity->frames = NULL;
    entity->frames_highlighted = NULL;
    entity->frame_dead = NULL;*/
    entity->texture_pack = NULL;
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
    /*entity->hit_cooldown = 0;
    entity->has_hit = 0;
    entity->is_projectile = 0;*/
    entity->elementary = 0;
    entity->effect = 0;
    entity->highlight = 0;
    entity->type = category;
    entity->prestige = 0;
    entity->frame_time = 0.05;
    entity->frame_delay = 0;
    entity->hitbox.x = 0;
    entity->hitbox.y = 0;
    entity->hitbox.w = (float)(width / 1920.0 * WIDTH);
    entity->hitbox.h = (float)(height / 1080.0 * HEIGHT);
    entity->target.x = 0;
    entity->target.y = 0;
    entity->target.w = 0;
    entity->target.h = 0;
    if (!entity->projectiles)
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
    /*clone->hit_cooldown = entity->hit_cooldown;
    clone->has_hit = entity->has_hit;
    clone->is_projectile = entity->is_projectile;*/
    clone->elementary = entity->elementary;
    clone->effect = entity->effect;
    clone->highlight = entity->highlight;
    clone->type = entity->type;
    clone->prestige = entity->prestige;
    clone->frame_time = entity->frame_time;
    clone->frame_delay = entity->frame_delay;
    clone->frame_index = entity->frame_index;
    //clone->reverse_frame_mode = entity->reverse_frame_mode;
    //clone->num_frames = entity->num_frames;
    clone->current_animation = entity->current_animation;
    clone->num_sounds = entity->num_sounds;
    //clone->extended_frames = entity->extended_frames;
    clone->hitbox = entity->hitbox;
    clone->target = entity->target;
    //clone->frames = entity->frames;
    //clone->frames_highlighted = entity->frames_highlighted;
    //clone->frame_dead = entity->frame_dead;
    clone->texture_pack = entity->texture_pack;
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
        if (entity->AI_lifetime != -7 && entity->AI_lifetime <= 0)
            DisableEntityAI(entity);
        else if (entity->AI_lifetime > 0)
            entity->AI_lifetime -= DELTA_TIME;
    }
    if (entity->frame_delay <= 0) {
        entity->frame_delay += entity->frame_time;
        if (entity->texture_pack->animations[entity->current_animation]->reverse_frame_mode == 0 && ++entity->frame_index == entity->texture_pack->animations[entity->current_animation]->num_frames)
            entity->frame_index = 0;
        else if (entity->texture_pack->animations[entity->current_animation]->reverse_frame_mode > 0 && ++entity->frame_index == entity->texture_pack->animations[entity->current_animation]->num_frames) {
            entity->frame_index--;
            entity->texture_pack->animations[entity->current_animation]->reverse_frame_mode *= -1;
        } else if (entity->texture_pack->animations[entity->current_animation]->reverse_frame_mode < 0 && --entity->frame_index == -1) {
            entity->frame_index++;
            entity->texture_pack->animations[entity->current_animation]->reverse_frame_mode *= -1;
        }
    } else {
        entity->frame_delay -= DELTA_TIME;
    }
    if (entity->shoot_delay > 0)
        entity->shoot_delay -= DELTA_TIME;
    if (entity->effect > 0)
        entity->effect -= DELTA_TIME;
    if (entity->highlight > 0)
        entity->highlight -= DELTA_TIME;
    /*if (entity->hit_cooldown > 0)
        entity->hit_cooldown -= DELTA_TIME;*/
}

/* ----------------------------------------------------------------------------
 * RenderEntity
 *
 * Détermine la texture à afficher :
 *   - Si l'entité n'est PAS vivante et qu'une animation de mort existe, on affiche
 *     la texture spécifique de mort.
 *   - Sinon, on affiche l'animation courante :
 *       * Si le flag 'highlight' est activé et que des frames highlighted existent,
 *         on les affiche.
 *       * Sinon on affiche la frame de base.
 * ----------------------------------------------------------------------------
 */
SDL_Texture* RenderEntity(const Entity *entity) {
    if (!entity || !entity->texture_pack) {
        fprintf(stderr, "RenderEntity: entity ou texturePack est NULL.\n");
        return NULL;
    }

    /* Si l'entité est morte et une animation de mort existe */
    if (!IsEntityAlive(entity) && entity->texture_pack->animations[ANIM_DEATH]) {
        const AnimationPack *deathPack = entity->texture_pack->animations[ANIM_DEATH];
        return deathPack->specific_texture;
    }

    /* Récupération de l'animation courante.
     * S'il n'y a pas d'animation pour l'état courant, on bascule sur idle.
     */
    AnimationPack *animPack = entity->texture_pack->animations[entity->current_animation];
    if (!animPack) {
        animPack = entity->texture_pack->animations[ANIM_IDLE];
        if (!animPack) {
            fprintf(stderr, "RenderEntity : aucune animation disponible.\n");
            return NULL;
        }
    }

    int idx = entity->frame_index;
    if (idx < 0 || idx >= animPack->num_frames) {
        fprintf(stderr, "RenderEntity: frame_index (%d) hors limites. Correction à 0.\n", idx);
        idx = 0;
    }

    /* Si highlight est activé et la version highlighted existe */
    if (entity->highlight > 0 && animPack->frames_highlighted && animPack->frames_highlighted[idx]) {
        return animPack->frames_highlighted[idx];
    }

    /* Sinon, retourne la frame de base */
    if (animPack->frames && animPack->frames[idx])
        return animPack->frames[idx];

    return NULL;
}

/* ----------------------------------------------------------------------------
 * DisplayEntity
 *
 * Convertit la hitbox et dessine l'entité.
 * Si aucune texture n'est disponible, dessine un rectangle blanc.
 * Affiche également la texture de la marque (mark) si présente.
 * ----------------------------------------------------------------------------
 */
void DisplayEntity(const Entity *entity) {
    if (!entity) {
        fprintf(stderr, "DisplayEntity: entity est NULL.\n");
        return;
    }

    const SDL_Rect rect = FRectToRect(entity->hitbox);
    SDL_Texture *currentTexture = RenderEntity(entity);

    if (currentTexture)
        DrawTexture(currentTexture, rect);
    else
        DrawRectF(entity->hitbox, 255, 255, 255, 255);

    if (entity->mark)
        DrawTexture(entity->mark, rect);
}

void MoveEntity(Entity *entity, const _Bool pass) {
    // Calculer le déplacement réel par rapport au temps
    double x = entity->velocity_x * entity->speed * DELTA_TIME;
    double y = entity->velocity_y * entity->speed * DELTA_TIME;

    // Appliquer effet éventuel (ralentissement)
    if (entity->effect > 0) {
        x /= 2.0;
        y /= 2.0;
    }

    // Vérification des limites avant le déplacement
    if (pass || (entity->hitbox.x + x >= 0 && entity->hitbox.x + entity->hitbox.w + x < WIDTH)) {
        entity->hitbox.x += (float)x;
    }
    if (pass || (entity->hitbox.y + y >= 0 && entity->hitbox.y + entity->hitbox.h + y < HEIGHT)) {
        entity->hitbox.y += (float)y;
    }
}

void MoveEntityAndCursor(Entity *entity, const _Bool pass, const int cursor_x, const int cursor_y) {
    double x = entity->velocity_x * entity->speed, y = entity->velocity_y * entity->speed;
    if (pass || (entity->hitbox.x + x >= 0 && entity->hitbox.x + entity->hitbox.w + x < WIDTH)) {
        if (entity->effect > 0) x /= 2.0;
        entity->hitbox.x += (float)x;
    }
    if (pass || (entity->hitbox.y + y >= 0 && entity->hitbox.y + entity->hitbox.h + y < HEIGHT)) {
        if (entity->effect > 0) y /= 2.0;
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
    if (IsEntityAlive(entity) && entity->aiming){
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
        entity->aim_delay = 1;
    }
}

void EntityAim(Entity *entity, const float x, const float y, const float w, const float h) {
    if (entity->shoot_delay <= 0 && entity->aim_delay > 0){
        entity->aiming = 1;
        entity->target.x = x;
        entity->target.y = y;
        entity->target.w = w;
        entity->target.h = h;
        entity->aim_delay -= DELTA_TIME;
        if (entity->aim_delay <= 0)
            EntityCease(entity);
    }

}

void EntityShoot(Entity *entity, EntityList *projectiles, const Entity *ammo, const float x, const float y, const float w, const float h) {
    if (!entity->aiming && entity->shoot_delay <= 0) {
        Entity *projectile = CreateEntity();
        CloneEntity(projectile, ammo);
        EnableEntityAI(projectile, 5);
        projectile->melee_damage = entity->damage;
        projectile->hitpoints = 0.1;
        //projectile->is_projectile = 1;
        const double xa = entity->hitbox.x + entity->hitbox.w / 2.0;
        const double ya = entity->hitbox.y + entity->hitbox.h / 2.0;
        const double xb = x + w / 2.0;
        const double yb = y + h / 2.0;
        const double alpha = atan2(yb - ya, xb - xa);
        projectile->velocity_x = SetVelocityX(cos(alpha) * (1 + RandInt(-100 + entity->precision, 100 - entity->precision) / 100.0));
        projectile->velocity_y = SetVelocityY(sin(alpha) * (1 + RandInt(-100 + entity->precision, 100 - entity->precision) / 100.0));
        projectile->hitbox.x = (float)(entity->hitbox.x + entity->hitbox.w / 2.0 - projectile->hitbox.w / 2.0);
        projectile->hitbox.y = (float)(entity->hitbox.y + entity->hitbox.h / 2.0 - projectile->hitbox.h / 2.0);
        entity->shoot_delay += RandInt(entity->shoot_rate_min, entity->shoot_rate_max) / 60.;
        if (entity->effect > 0) entity->shoot_delay *= 2;
        if (entity->sounds)
            PlaySound(entity->sounds[RandInt(0, entity->num_sounds - 1)]);
        AddEntity(projectiles, projectile);
    }
}

void EntityIntercept(Entity *entity, const float x, const float y, const float w, const float h) {
    const double xa = entity->hitbox.x + entity->hitbox.w / 2.0;
    const double ya = entity->hitbox.y + entity->hitbox.h / 2.0;
    const double xb = x + w / 2.0;
    const double yb = y + h / 2.0;
    const double alpha = atan2(yb - ya, xb - xa);
    entity->velocity_x = SetVelocityX(cos(alpha) * (1 + RandInt(-100 + entity->stability, 100 - entity->stability) / 100.0));
    entity->velocity_y = SetVelocityY(sin(alpha) * (1 + RandInt(-100 + entity->stability, 100 - entity->stability) / 100.0));
}

void ProcessDroneAI(Entity *e) {
    switch (e->type)
    {
    case DRONE:
        MoveEntity(e, 1);
        EntityShoot(e, drone->projectiles, regular_bullet,
                    player->hitbox.x, player->hitbox.y,
                    player->hitbox.w, player->hitbox.h);
        break;
    case DRONE_SUICIDE_BOMBER:
        MoveEntity(e, 1);
        EntityIntercept(e, player->hitbox.x, player->hitbox.y,
                        player->hitbox.w, player->hitbox.h);
        break;
    case DRONE_NOVA:
        MoveEntity(e, 1);
        EntityShoot(e, drone->projectiles, nova_orb,
                    player->hitbox.x, player->hitbox.y,
                    player->hitbox.w, player->hitbox.h);
        break;
    case DRONE_HUNTER:
        if (AreEntitiesNear(e, player, HEIGHT - ScaleHeight(128))) {
            EntityAim(e, player->hitbox.x, player->hitbox.y,
                      player->hitbox.w, player->hitbox.h);
            EntityShoot(e, drone->projectiles, sniper_bullet,
                        player->hitbox.x, player->hitbox.y,
                        player->hitbox.w, player->hitbox.h);
        } else {
            MoveEntity(e, 1);
            EntityCease(e);
            EntityIntercept(e, player->hitbox.x, player->hitbox.y,
                            player->hitbox.w, player->hitbox.h);
        }
        break;
    default: break;
    }
}


void UpdateEntities(void) {
    const EntityNode *node = ennemies->head;
    while (node) {
        const EntityNode *next = node->next;
        Entity *e = node->entity;

        if (IsEntityAIDisabled(e)) {
            RemoveEntity(ennemies, e);
        } else if (IsEntityAlive(e)) {
            UpdateEntityState(e);
            ProcessDroneAI(e);
        } else {
            MoveEntity(e, 1);
            UpdateEntityState(e);
            e->speed *= pow(1.5, DELTA_TIME);
        }

        node = next;
    }
}

void UpdateProjectiles(EntityList *projectiles) {
    const EntityNode *node = projectiles->head;
    while (node) {
        const EntityNode *next = node->next;
        if (IsEntityAIEnabled(node->entity)) {
            MoveEntity(node->entity, 1);
            UpdateEntityState(node->entity);
        } else {
            RemoveEntity(projectiles, node->entity);
        }
        node = next;
    }
}

void HandleCollision(Entity **entity1, Entity **entity2) {

    Entity *e1 = *entity1;
    Entity *e2 = *entity2;

    if (Intersecting(e1->hitbox, e2->hitbox)) {
        //printf("Collision detected between Entity1 (HP: %.2f) and Entity2 (HP: %.2f)\n",
        //       e1->hitpoints, e2->hitpoints);

        //const double hp1_before = e1->hitpoints;
        //const double hp2_before = e2->hitpoints;

        const double damageToE1 = e2->melee_damage;
        const double damageToE2 = e1->melee_damage;

        e1->hitpoints -= damageToE1;
        e2->hitpoints -= damageToE2;

        //printf("After base damage: Entity1 HP = %.2f, Entity2 HP = %.2f\n",
        //       e1->hitpoints, e2->hitpoints);

        e1->highlight = 0.1;
        e2->highlight = 0.1;
    }

    /*
    if (entity1->hit_cooldown <= 0.0) {
        entity1->hitpoints -= entity2->melee_damage;
        entity1->hit_cooldown = 0.2;
    }
    if (entity2->hit_cooldown <= 0.0) {
        entity2->hitpoints -= entity1->melee_damage;
        entity2->hit_cooldown = 0.2;
    }
    */

    if (e1->elementary == 1) e2->effect = 1.5;
    if (e2->elementary == 1) e1->effect = 1.5;
}

void CheckCollisions(void) {
    EntityNode *node = ennemies->head;
    while (node) {
        EntityNode *next = node->next;
        if (IsEntityAlive(node->entity)) {
            HandleCollision(&node->entity, &player);
            EntityNode *proj = player->projectiles->head;
            while (proj) {
                EntityNode *next_proj = proj->next;
                HandleCollision(&node->entity, &proj->entity);
                if (IsEntityDead(proj->entity))
                    DisableEntityAI(proj->entity);
                proj = next_proj;
            }
            if (IsEntityDead(node->entity)) {
                node->entity->AI_lifetime = 5;
                node->entity->velocity_y = SetVelocityY(1);
                node->entity->speed = 5;
                GainScore(node->entity, game_difficulty);
            }
        }
        node = next;
    }
    node = drone->projectiles->head;
    while (node) {
        EntityNode *next = node->next;
        HandleCollision(&node->entity, &player);
        if (IsEntityDead(node->entity))
            DisableEntityAI(node->entity);
        node = next;
    }
}

void HandleSummonLogic(void) {
    summon_delay -= DELTA_TIME;
    if (summon_delay <= 0) {
        summon_delay += RandInt((int)(summon_delay_max / 5. * 100), (int)(summon_delay_max * 100)) / 100.;
        summon_delay_max -= DELTA_TIME;

        Entity *e = CreateEntity();
        const int category = RandInt(0, 49);

        if (easter_egg || category < 15) CloneEntity(e, drone_sb);
        else if (category < 20) CloneEntity(e, drone_nova);
        else if (category == 20) CloneEntity(e, drone_hunter);
        else CloneEntity(e, drone);

        int lifetime = category < 15 || category == 20 ? -7 : 10;
        if (category >= 15 && category < 20) lifetime = 15;

        const int prestige = RandInt(0, 10000);
        if (prestige < 100) {
            e->prestige = 1;
            AssignTexturePackToEntity(e, res.silver_drone_frames);
        } else if (prestige == 100) {
            e->prestige = 2;
            AssignTexturePackToEntity(e, res.gold_drone_frames);
        }

        const int side = RandInt(0, 1);
        const float y = (float)RandInt(0, HEIGHT - (int)drone->hitbox.h);
        const float x = side ? (float)WIDTH + drone->hitbox.w : -drone->hitbox.w;
        SummonEntity(e, lifetime, x, y);
        e->velocity_x = SetVelocityX(side ? -1 : 1);
        AddEntity(ennemies, e);
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
    return Distance(entity1->hitbox.x, entity1->hitbox.y, entity2->hitbox.x, entity2->hitbox.y) < threshold;
}

_Bool AreEntitiesFar(const Entity *entity1, const Entity *entity2, const double threshold) {
    return Distance(entity1->hitbox.x, entity1->hitbox.y, entity2->hitbox.x, entity2->hitbox.y) > threshold;
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


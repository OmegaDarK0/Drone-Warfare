#include "collision.h"
#include "entity.h"
#include "globals.h"
#include "hud.h"
#include "utils.h"

GridCell spatial_grid[GRID_HEIGHT][GRID_WIDTH];

void ClearGrid(void) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            spatial_grid[y][x].count = 0;
        }
    }
}

void InsertIntoGrid(Entity *entity) {
    const int minX = (int)entity->hitbox.x / GRID_CELL_SIZE;
    const int minY = (int)entity->hitbox.y / GRID_CELL_SIZE;
    const int maxX = (int)(entity->hitbox.x + entity->hitbox.w) / GRID_CELL_SIZE;
    const int maxY = (int)(entity->hitbox.y + entity->hitbox.h) / GRID_CELL_SIZE;

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                GridCell *cell = &spatial_grid[y][x];
                if (cell->count < MAX_ENTITIES_PER_CELL) {
                    cell->entities[cell->count++] = entity;
                }
            }
        }
    }
}

_Bool CheckCollision(const SDL_FRect *a, const SDL_FRect *b) {
    return a->x < b->x + b->w
        && a->x + a->w > b->x
        && a->y < b->y + b->h
        && a->y + a->h > b->y;
}

void CheckGridCollisions(void (*onCollision)(Entity*, Entity*)) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            const GridCell *cell = &spatial_grid[y][x];
            for (int i = 0; i < cell->count; i++) {
                Entity *e1 = cell->entities[i];
                for (int j = i + 1; j < cell->count; j++) {
                    Entity *e2 = cell->entities[j];
                    if (e1->team != e2->team) {
                        if (CheckCollision(&e1->hitbox, &e2->hitbox)) {
                            onCollision(e1, e2);
                        }
                    }
                }
            }
        }
    }
}

void ApplyCollisionEffects(Entity *target, const Entity *source) {
    double raw_damage = source->damage;
    if (source->class != PROJECTILE)
        raw_damage *= DELTA_TIME;
    target->acc_damage += raw_damage;
    const int damage = (int)round(target->acc_damage);
    target->health -= damage;
    target->acc_damage -= damage;
    if (IsEntityDead(target)) {
        if (target->class == ENEMY)
            GainScore(target);
        target->velocity_y = set_velocity_y(1);
        target->speed = 5;
    } else {
        if (source->type == PROJECTILE_NOVA_ORB)
            target->effect = 1.5;
        target->highlight = .1;
    }
}

void HandleCollision(Entity *a, Entity *b) {
    if (a->class == PROJECTILE && b->class == PROJECTILE)
        return;
    if (IsEntityAlive(a) && IsEntityAlive(b)) {
        ApplyCollisionEffects(a, b);
        ApplyCollisionEffects(b, a);
    }
}

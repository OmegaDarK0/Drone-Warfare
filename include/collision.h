#ifndef COLLISION_H
#define COLLISION_H

#include "structures.h"
#include "constants.h"

void ClearGrid(void);
void InsertIntoGrid(Entity *entity);
_Bool CheckCollision(const SDL_FRect *a, const SDL_FRect *b);
void CheckGridCollisions(void (*onCollision)(Entity*, Entity*));
void ApplyCollisionEffects(Entity *target, const Entity *source);
void HandleCollision(Entity *a, Entity *b);

#endif //COLLISION_H

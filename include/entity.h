#ifndef ENTITY_H
#define ENTITY_H

#include "structures.h"

Entity *CreateEntity(void);
void InitEntity(Entity *entity, float width, float height, int category, double HP, double damage, double m_damage, double speed, int rate_min, int rate_max, int precision, int stability);
void CloneEntity(Entity *clone, const Entity *entity);
void DestroyEntity(Entity *entity);
void UpdateEntityState(Entity *entity);
void DisplayEntity(const Entity *entity);
void MoveEntity(Entity *entity, _Bool pass);
void MoveEntityAndCursor(Entity *entity, _Bool pass, int cursor_x, int cursor_y);
void SummonEntity(Entity *entity, int lifetime, float x, float y);
void EntityShoot(Entity *entity, EntityList *projectiles, const Entity *ammo, float x, float y, float w, float h);
void EntityAim(Entity *entity, float x, float y, float w, float h);
void EntityIntercept(Entity *entity, float x, float y, float w, float h);
void HandleCollision(Entity **entity1, Entity **entity2);
void DisableEntityAI(Entity *entity);
void EnableEntityAI(Entity *entity, int lifetime);
_Bool AreEntitiesNear(const Entity *entity1, const Entity *entity2, double threshold);
_Bool AreEntitiesFar(const Entity *entity1, const Entity *entity2, double threshold);
_Bool IsEntityAIEnabled(const Entity *entity);
_Bool IsEntityAIDisabled(const Entity *entity);
_Bool IsEntityAlive(const Entity *entity);
_Bool IsEntityDead(const Entity *entity);

#endif /* ENTITY_H */

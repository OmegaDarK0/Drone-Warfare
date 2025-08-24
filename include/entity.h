#ifndef ENTITY_H
#define ENTITY_H

#include "structures.h"

Entity *CreateEntity(int class, int type, float width, float height);
void InitEntity(Entity *entity, double health, double damage, double speed, double rate_min, double rate_max, int precision, int stability);
void CloneEntity(Entity *clone, const Entity *entity);
void DestroyEntity(Entity *entity);
void UpdateEntityState(Entity *entity);
SDL_Texture* RenderEntity(const Entity *entity);
void DisplayEntity(Entity *entity);
void DisplayHealthBar(Entity *entity);
void DisplayEntityHitbox(Entity *entity);
void MoveEntity(Entity *entity, _Bool pass);
void MoveEntityAndCursor(Entity *entity, _Bool pass, int cursor_x, int cursor_y);
void SummonEntity(Entity *entity, int lifetime, float x, float y);
void DisplayEntityLaserBeam(Entity *entity);

void EntityCease(Entity *entity);
void EntityAim(Entity *entity, float x, float y, float w, float h);
void EntityShoot(Entity *entity, float x, float y, float w, float h);
void EntityIntercept(Entity *entity, float x, float y, float w, float h);
void DisableEntityAI(Entity *entity);
void EnableEntityAI(Entity *entity, int lifetime);

void ProcessDroneAI(Entity *e);
void UpdateEntities(EntityList *entities);
void SetAmmo(Entity *entity, Entity *ammo);
//void HandleCollision(Entity **entity1, Entity **entity2);
//void CheckCollisions(void);
void HandleSummonLogic(EntityList *entities);

_Bool AreEntitiesNear(const Entity *entity1, const Entity *entity2, double threshold);
_Bool AreEntitiesFar(const Entity *entity1, const Entity *entity2, double threshold);
_Bool IsEntityAIEnabled(const Entity *entity);
_Bool IsEntityAIDisabled(const Entity *entity);
_Bool IsEntityAlive(const Entity *entity);
_Bool IsEntityDead(const Entity *entity);

#endif // ENTITY_H


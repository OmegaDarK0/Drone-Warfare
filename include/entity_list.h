#ifndef ENTITY_LIST_H
#define ENTITY_LIST_H

#include "structures.h"

EntityList* CreateEntityList(void);
void AddEntity(EntityList *list, Entity *entity);
void RemoveEntity(EntityList *list, const Entity *entity);
void DestroyEntityList(EntityList *list);
void ClearEntityList(EntityList *list, int filter);
void IterateEntities(const EntityList *list, void (*func)(Entity*), int filter);

#endif // ENTITY_LIST_H


#ifndef ENTITY_LIST_H
#define ENTITY_LIST_H

#include "structures.h"

EntityList* CreateEntityList();
void AddEntity(EntityList *list, Entity *entity);
void RemoveEntity(EntityList *list, const Entity *entity);
void DestroyEntityList(EntityList *list);
void ClearEntityList(EntityList *list);
void IterateEntities(const EntityList *list, void (*func)(Entity *));

#endif // ENTITY_LIST_H

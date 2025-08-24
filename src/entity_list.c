#include "entity_list.h"
#include <stdlib.h>

#include "entity.h"

EntityList* CreateEntityList(void) {
    EntityList *list = malloc(sizeof(EntityList));
    if (list) {
        list->head = NULL;
        list->count = 0;
    }
    return list;
}

void AddEntity(EntityList *list, Entity *entity) {
    EntityNode *newNode = malloc(sizeof(EntityNode));
    if (newNode) {
        entity->team = list;
        newNode->entity = entity;
        newNode->next = list->head;
        list->head = newNode;
        list->count++;
    }
}

void RemoveEntity(EntityList *list, const Entity *entity) {
    EntityNode *current = list->head;
    EntityNode *prev = NULL;
    while (current) {
        if (current->entity == entity) {
            if (prev)
                prev->next = current->next;
            else
                list->head = current->next;
            current->entity->team = NULL;
            free(current->entity);
            free(current);
            list->count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void DestroyEntityList(EntityList *list) {
    EntityNode *current = list->head;
    while (current) {
        EntityNode *next = current->next;
        DestroyEntity(current->entity);
        free(current);
        current = next;
    }
    free(list);
}

void ClearEntityList(EntityList *list, const int filter) {
    EntityNode *current = list->head;
    while (current) {
        EntityNode *next = current->next;
        if (current->entity->class != filter)
            DestroyEntity(current->entity);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->count = 0;
}

void IterateEntities(const EntityList *list, void (*func)(Entity*), const int filter) {
    const EntityNode *current = list->head;
    while (current) {
        if (current->entity->class != filter)
            func(current->entity);
        current = current->next;
    }
}


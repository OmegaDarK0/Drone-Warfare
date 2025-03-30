#include <stdlib.h>
#include "entity_list.h"

// Initialize a new entity list
EntityList* CreateEntityList() {
    EntityList *list = malloc(sizeof(EntityList));
    if (list) {
        list->head = NULL;
        list->count = 0;
    }
    return list;
}

// Add an entity to the list
void AddEntity(EntityList *list, Entity *entity) {
    EntityNode *newNode = malloc(sizeof(EntityNode));
    if (newNode) {
        newNode->entity = entity;
        newNode->next = list->head;
        list->head = newNode;
        list->count++;
    }
}

// Remove an entity from the list
void RemoveEntity(EntityList *list, const Entity *entity) {
    EntityNode *current = list->head;
    EntityNode *previous = NULL;
    while (current) {
        if (current->entity == entity){
            if (previous) {
                previous->next = current->next;
            } else {
                list->head = current->next;
            }
            free(current->entity);
            free(current);
            list->count--;
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Free the entire entity list
void DestroyEntityList(EntityList *list) {
    EntityNode *current = list->head;
    while (current) {
        EntityNode *next = current->next;
        free(current->entity);
        free(current);
        current = next;
    }
    free(list);
}

void ClearEntityList(EntityList *list) {
    EntityNode *current = list->head;
    while (current) {
        EntityNode *next = current->next;
        free(current->entity);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->count = 0;
}

// Iterate over the entities in the list and apply a function to each
void IterateEntities(const EntityList *list, void (*func)(Entity *)) {
    const EntityNode *current = list->head;
    while (current) {
        func(current->entity);
        current = current->next;
    }
}


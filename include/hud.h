#ifndef HUD_H
#define HUD_H

#include "entity.h"

void ClearHUD(void);

// Initialise les positions et structures du HUD
void InitHUD(void);

// Affiche les informations du HUD (points de vie, score, etc.) selon l'état de l'entité
void DisplayHUD(const Entity *entity);

// Affiche un effet visuel (par exemple, lors de l'activation d'un effet)
void DisplayEffect(const Entity *entity);

// Met à jour le score affiché
void RenderScore(void);

// Augmente le score en fonction des dégâts, de la catégorie et d'un paramètre difficulté
void GainScore(const Entity *entity, int difficulty);

#endif // HUD_H


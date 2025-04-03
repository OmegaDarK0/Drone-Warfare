#ifndef HUD_H
#define HUD_H

#include "structures.h"

extern Solid *hitpoints_info;
extern Solid *score_info;

void InitHUD();
void DisplayHUD(const Entity *entity);
void DisplayEffect(const Entity *entity);
void RenderScore();
void GainScore(const Entity *entity, int difficulty);

#endif /* HUD_H */

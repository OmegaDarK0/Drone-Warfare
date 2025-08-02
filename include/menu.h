#ifndef MENU_H
#define MENU_H

#include "structures.h"

void SetLevelMenu(void);
void SetDifficultyMenu(void);
_Bool HandleLevelSelection(SDL_Point cursor_pos);
const SDL_Rect *GetHighlightRect(SDL_Point cursor_pos);
int GetDifficultyAtPosition(const SDL_Point *pos);
_Bool HandleDifficultySelection(const SDL_Point *pos);

#endif //MENU_H

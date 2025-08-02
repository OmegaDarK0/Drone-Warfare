#ifndef SCORE_H
#define SCORE_H

#include "structures.h"

void UpdatePlayer(void);
void RegisterScore(const char *player_name, int difficulty);
void ResetPlayerInfo(PlayerInfo *player_info);

#endif // SCORE_H

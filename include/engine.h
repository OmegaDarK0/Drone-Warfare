#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <freetype/freetype.h>
#include "structures.h"

// ENGINE
int QuitEngine(void);
int InitEngine(void);

// IMAGE & TEXTURE
SDL_Surface *LoadImage(const char *filename);
SDL_Surface *LoadText(FT_Face font, const char *text, SDL_Color color);
SDL_Texture *LoadTexture(const char *filename);
SDL_Texture *LoadTextureFromImage(SDL_Surface *surface);
SDL_Texture *LoadTextureFromText(FT_Face font, const char *text, SDL_Color color);
void GetTextureSize(SDL_Texture* texture, int* width, int* height);
void UnloadImage(SDL_Surface *surface);
void UnloadTexture(SDL_Texture *texture);

// SOUND & MUSIC
Mix_Chunk *LoadSound(const char *filename);
Mix_Music *LoadMusic(const char *filename);
void PlaySound(Mix_Chunk *sound);
void ChangeVolumeSound(Mix_Chunk *sound, int volume);
void IncreaseVolumeSound(Mix_Chunk *sound, int volume);
void DecreaseVolumeSound(Mix_Chunk *sound, int volume);
void PlayMusic(Mix_Music *music, int loops);
void ChangeVolumeMusic(int volume);
void IncreaseVolumeMusic(int volume);
void DecreaseVolumeMusic(int volume);
void ResetVolumeMusic();
void UnloadSound(Mix_Chunk *chunk);
void UnloadMusic(Mix_Music *music);

// DRAWING & DISPLAY
void DrawPoint(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void DrawLine(int xa, int ya, int xb, int yb, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void DrawRect(SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void DrawRectF(SDL_FRect frect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void DrawTexture(SDL_Texture *texture, SDL_Rect rect);
void DrawBackground(SDL_Texture *texture);
void DrawText(const Solid *text, _Bool centered);
void DrawButton(const Solid *button, _Bool centered, int pos_x, int pos_y, unsigned int mode);
void DisplayCursor(SDL_Texture *texture, SDL_Rect rect);

// POSITION & VELOCITY
void SetPosition(SDL_Rect *rect, int x, int y);
void SetPositionF(SDL_FRect *frect, float x, float y);
double SetVelocityX(double var);
double SetVelocityY(double var);

// INTERSECTION & COLLISION
_Bool Intersecting(SDL_FRect frect1, SDL_FRect frect2);

// COLOR & FILTER
Uint32 ColorToUint32(const SDL_Surface *surface, SDL_Color color);
void ReplaceColor(const SDL_Surface *surface, SDL_Color src_color, SDL_Color dst_color);
void HighlightFilter(const SDL_Surface* surface, int alpha);

// TEXTE & SOLID
Solid *CreateSolid(SDL_Rect rect, SDL_Color color, const char *str);
Solid *CreateSolidText(SDL_Rect rect, SDL_Color color, const char *str, FT_Face font, SDL_Color text_color);
void DestroySolid(Solid *solid);

// MISC
void RefreshScreen(void);
void ClearScreen(void);
void RegisterScore(char *player_name, int difficulty);
SDL_Rect FRectToRect(SDL_FRect frect);
int TimeToFrames(double seconds);

#endif /* ENGINE_H */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct Entity Entity;
typedef struct EntityNode EntityNode;
typedef struct EntityList EntityList;
typedef struct TexturePack TexturePack;
typedef struct SoundPack SoundPack;
typedef struct Cursor Cursor;
typedef struct Solid Solid;

struct EntityNode{
    Entity *entity;
    EntityNode *next;
};

struct EntityList{
    EntityNode *head;
    int count;
};

struct Entity{
    _Bool AI;
    int AI_lifetime;
    int ID;
    // FRONT
    double hitpoints;
    double hitpoints_max;
    double damage;
    double melee_damage;
    double speed;
    double velocity_x;
    double velocity_y;
    int shoot_rate_min;
    int shoot_rate_max;
    int shoot_delay;
    int aim_delay;
    int precision;
    int stability;
    // MIDDLE
    int elementary;
    int effect;
    int highlight;
    int category;
    int prestige;
    // BACK
    int frame_time;
    int frame_delay;
    int frame_index;
    int reverse_frame_mode;
    int num_frames;
    int num_sounds;
    SDL_FRect hitbox;
    SDL_Texture **frames;
    SDL_Texture **frames_highlighted;
    SDL_Texture *frame_dead;
    SDL_Texture *mark;
    Mix_Chunk **sounds;
    EntityList *projectiles;
};

struct TexturePack {
    SDL_Texture **frames;
    SDL_Texture **frames_highlighted;
    SDL_Texture *frame_dead;
    char *name;
    int num_frames;
    int reverse_frame_mode;
};

struct SoundPack {
    Mix_Chunk **sounds;
    char *name;
    int num_sounds;
};

struct Solid {
    SDL_Texture *texture;
    SDL_Rect rect;
    SDL_Color color;
};

#endif //STRUCTURES_H

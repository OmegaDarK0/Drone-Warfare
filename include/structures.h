#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef enum GameState GameState;
typedef enum AnimationType AnimationType;
typedef enum DroneType DroneType;
typedef struct Entity Entity;
typedef struct EntityNode EntityNode;
typedef struct EntityList EntityList;
typedef struct AnimationPack AnimationPack;
typedef struct TexturePack TexturePack;
typedef struct SoundPack SoundPack;
typedef struct ResourcePack ResourceManager;
typedef struct Cursor Cursor;
typedef struct Solid Solid;
typedef struct PlayerInfo PlayerInfo;
typedef struct EntityConfig EntityConfig;
typedef struct LevelMenuEntry LevelMenuEntry;
typedef struct DifficultyMenuEntry DifficultyMenuEntry;
typedef struct CollisionPair CollisionPair;

enum GameState{
    STATE_MENU,
    STATE_DIFFICULTY,
    STATE_GAME,
    STATE_CINEMATIC,
    STATE_GAMEOVER
};

enum AnimationType{
    ANIM_IDLE,
    //ANIM_ATTACK,
    //ANIM_MOVING,
    ANIM_DEATH,
    NUM_ANIMATION_TYPES  // Nombre total d'états
};

enum DroneType{
    DRONE,
    DRONE_SUICIDE_BOMBER,
    DRONE_NOVA,
    DRONE_HUNTER
};

struct EntityNode{
    Entity *entity;
    EntityNode *next;
};

struct EntityList{
    EntityNode *head;
    int count;
};

struct Entity{
    // CORE
    _Bool AI;
    double AI_lifetime;
    int ID;
    // FRONT
    double hitpoints;
    double hitpoints_max;
    double damage;
    double melee_damage;
    double speed;
    double velocity_x;
    double velocity_y;
    double fire_rate_min;
    double fire_rate_max;
    double fire_cooldown;
    double aim_cooldown;
    int precision;
    int stability;
    _Bool aiming;
    // MIDDLE
    /*double hit_cooldown;
    _Bool has_hit;
    _Bool is_projectile;*/
    int elementary;
    double effect;
    double highlight;
    int type;
    int prestige;
    // BACK
    double frame_time;
    double frame_cooldown;
    int frame_index;
    int current_animation;
    //int reverse_frame_mode;
    //int num_frames;
    int num_sounds;
    //int extended_frames;
    SDL_FRect hitbox;
    SDL_FRect target;
    //SDL_Texture **frames;
    //SDL_Texture **frames_highlighted;
    //SDL_Texture *frame_dead;
    TexturePack *texture_pack;
    SDL_Texture *mark;
    Mix_Chunk **sounds;
    EntityList *projectiles;
};

/* *****************************************************************************
 * AnimationPack
 *
 *  - Pour les animations classiques (idle, attack, moving), on stocke un tableau
 *    de frames et une version "highlighted" générée automatiquement (si désiré).
 *  - Pour l'animation de mort, une texture spécifique est chargée.
 **************************************************************************** */
struct AnimationPack {
    SDL_Texture **frames;              // Tableau de textures pour l'animation de base
    SDL_Texture **frames_highlighted;  // Versions "highlighted" (peut être NULL)
    int num_frames;                    // Nombre de frames dans cette animation
    int reverse_frame_mode;            // Option pour inverser l'animation (optionnel)
    SDL_Texture *specific_texture;     // Utilisé pour l'animation de mort (image unique)
};

/* *****************************************************************************
 * TexturePack
 *
 *  - Regroupe l'ensemble des animations d'une entité.
 **************************************************************************** */
struct TexturePack {
    AnimationPack *animations[NUM_ANIMATION_TYPES];  // Une animation par type
};

struct SoundPack {
    Mix_Chunk **sounds;
    char *name;
    int num_sounds;
};

struct ResourcePack{
    SDL_Texture *explosive_mark;
    SDL_Texture *nova_mark;
    SDL_Texture *hunter_mark;
    SDL_Texture *entity_debug_hitbox;

    SDL_Texture *level1_background;
    SDL_Texture *level2_background;
    SDL_Texture *level3_background;
    SDL_Texture *level4_background;
    SDL_Texture *level0_background;

    Mix_Music *level0_music;
    Mix_Music *level1_music;
    Mix_Music *level2_music;
    Mix_Music *level3_music;
    Mix_Music *level4_music;

    TexturePack *drone_frames;
    TexturePack *silver_drone_frames;
    TexturePack *gold_drone_frames;
    TexturePack *player_drone_frames;
    TexturePack *regular_bullet_frames;
    TexturePack *sniper_bullet_frames;
    TexturePack *plasma_orb_frames;
    TexturePack *raw_plasma_orb_frames;
    TexturePack *nova_orb_frames;

    SoundPack *magnum_pistol_sounds;
    SoundPack *plasma_pistol_sounds;
    SoundPack *grenade_launcher_sounds;
    SoundPack *sniper_rifle_sounds;
};

struct Solid {
    SDL_Texture *texture;
    SDL_Rect rect;
    SDL_Color color;
};

#define MAX_PLAYER_NAME_LENGTH 8

struct PlayerInfo{
    char name[MAX_PLAYER_NAME_LENGTH + 1];
    int length;
    Solid *title;
};

struct EntityConfig{
    double HP;
    double damage;
    double m_damage;
    double speed;
    double rate_min;
    double rate_max;
    int precision;
    int stability;
};

struct LevelMenuEntry{
    SDL_Rect area;
    Mix_Music *music;
    SDL_Texture *background;
    int level_id;  // Pour logique éventuelle
};

struct DifficultyMenuEntry{
    SDL_Rect area;
    int difficulty;
    EntityConfig drone_config[4]; // DRONE, DRONE_SB, DRONE_NOVA, DRONE_HUNTER
    int summon_delay;
};

struct CollisionPair{
    Entity *a;
    Entity *b;
};

#endif // STRUCTURES_H


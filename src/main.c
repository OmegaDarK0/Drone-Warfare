#ifdef _WIN32
#include <windows.h>
// Pour les applications Windows, on redirige l'entrée vers la fonction main.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    extern int main(int argc, char *argv[]);
    return main(__argc, __argv);
}
#endif

#define DEFINE_CONSTANTS
#define DEFINE_GLOBALS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "engine.h"          // Initialisation SDL, gestion du rendu, etc.
#include "graphics.h"
#include "globals.h"         // Variables globales (window, renderer, etc.)
#include "constants.h"       // Constantes (dimensions, couleurs, etc.)
#include "utils.h"           // Helpers de scaling, couleurs, etc.
#include "score.h"
#include "entity.h"          // Création et gestion des entités
#include "entity_list.h"     // Gestion de liste d'entités
#include "hud.h"             // Affichage du HUD
#include "glyph.h"           // Gestion des glyphes pour le texte
#include "resource_pack.h"   // Gestion des texture/sound packs
#include "ui.h"              // Gestion de l'interface (boutons, texte, curseur)
#include "images.h"          // Chargement d'images & textures
#include "audio.h"           // Chargement et lecture audio

void LoadAllResources(ResourceManager *res) {
    res->sb_mark             = LoadTexture("sb.png");
    res->nova_mark           = LoadTexture("nova.png");
    res->hunter_mark         = LoadTexture("hunter.png");
    res->debug_hitbox        = LoadTexture("debug.png");

    res->level1              = LoadTexture("level1.png");
    res->level2              = LoadTexture("level2.png");
    res->level3              = LoadTexture("level3.png");
    res->level4              = LoadTexture("level4.png");
    res->level0              = LoadTexture(".Naval_ensign_of_the_Empire_of_Japan.png");

    res->level1_music        = LoadMusic("05 Flollo.mp3");
    res->level2_music        = LoadMusic("17 Best Guess At Best.mp3");
    res->level3_music        = LoadMusic("10 Six-Armed Robbing Suit.mp3");
    res->level4_music        = LoadMusic("11 Action Figure Hands.mp3");
    res->level0_music        = LoadMusic(".Battotai - Imperial Japanese Army March.mp3");

    res->black_drone_frames  = CreateTexturePack("drone", RED, 1, 1);
    res->silver_drone_frames = CreateTexturePack("silver_drone", RED, 1, 1);
    res->gold_drone_frames   = CreateTexturePack("gold_drone", RED, 1, 1);
    res->player_drone_frames = CreateTexturePack("silver_drone", BLUE, 1, 1);
    res->regular_bullet_frames = CreateTexturePack("regular_bullet", VOID, 0, 0);
    res->sniper_bullet_frames  = CreateTexturePack("sniper_bullet", VOID, 0, 0);
    res->plasma_frames       = CreateTexturePack("plasma_orb", VOID, 0, 0);
    res->nova_frames         = CreateTexturePack("nova", VOID, 0, 0);

    res->magnum_pistol_sounds = CreateSoundPack("magnum", 4);
    res->plasma_pistol_sounds = CreateSoundPack("plas_pistol_fire_reach", 3);
    res->grenade_launcher_sounds = CreateSoundPack("grenade_launcher_fire", 10);
    res->sniper_rifle_sounds = CreateSoundPack("sniper_distant", 4);
}

void FreeAllResources(const ResourceManager *res) {
    UnloadTexture(res->sb_mark);
    UnloadTexture(res->nova_mark);
    UnloadTexture(res->hunter_mark);
    UnloadTexture(res->debug_hitbox);

    UnloadTexture(res->level1);
    UnloadTexture(res->level2);
    UnloadTexture(res->level3);
    UnloadTexture(res->level4);
    UnloadTexture(res->level0);

    UnloadMusic(res->level0_music);
    UnloadMusic(res->level1_music);
    UnloadMusic(res->level2_music);
    UnloadMusic(res->level3_music);
    UnloadMusic(res->level4_music);

    DestroyTexturePack(res->black_drone_frames);
    DestroyTexturePack(res->silver_drone_frames);
    DestroyTexturePack(res->gold_drone_frames);
    DestroyTexturePack(res->player_drone_frames);
    DestroyTexturePack(res->regular_bullet_frames);
    DestroyTexturePack(res->sniper_bullet_frames);
    DestroyTexturePack(res->plasma_frames);
    DestroyTexturePack(res->nova_frames);

    DestroySoundPack(res->magnum_pistol_sounds);
    DestroySoundPack(res->plasma_pistol_sounds);
    DestroySoundPack(res->grenade_launcher_sounds);
    DestroySoundPack(res->sniper_rifle_sounds);
}

// --------------------------------------------------------
// 2. Fonction utilitaire pour le curseur
// --------------------------------------------------------
void UpdateCursorPosition(Solid *cursor) {
    SDL_GetMouseState(&cursor->rect.x, &cursor->rect.y);
}

// --------------------------------------------------------
// 3. Machine à états : définition et fonctions associées
// --------------------------------------------------------

GameState game_state = STATE_MENU;
SDL_bool running = SDL_TRUE;
_Bool restart = 0;
int difficulty = 0;
int score = 0;
SDL_Texture *background = NULL;
Mix_Music *current_music = NULL;

// (Autres variables globales utilisées par le code)
Entity *player, *drone, *drone_sb, *drone_nova, *drone_hunter;
Entity *regular_bullet, *sniper_bullet, *plasma, *nova;
EntityList *ennemies;
Solid *cursor, *button1, *button2, *button3, *button4, *button5;
Solid *restart_button, *game_over_title, *pause_title, *player_name_title, *score_title;
int font_small, font_medium, font_large, font_xxl, font_jumbo;
SDL_Rect box1, box2, box3, box4;

// Prototypes des fonctions d'initialisation, mise à jour, rendu et condition de transition pour chaque état
void InitMenu(void);
void UpdateMenu(void);
void RenderMenu(void);
int MenuShouldTransition(void);

void UpdateDifficulty(void);
void RenderDifficulty(void);
int DifficultyShouldTransition(void);

void InitGame(void);
void UpdateGame(void);
void RenderGame(void);
int GameShouldTransition(void);

void InitCinematic(void);
void UpdateCinematic(void);
void RenderCinematic(void);
int CinematicShouldTransition(void);

void InitGameOver(void);
void UpdateGameOver(void);
void RenderGameOver(void);
int GameOverShouldTransition(void);

// --------------------------------------------------------
// Implantations des fonctions d'état
// --------------------------------------------------------

void InitMenu(void) {
    easter_egg = 0;
    restart = 0;
    difficulty = 0;
    background = NULL;
    score = 0;
    entity_count = 0;
    summon_delay = TimeToFrames(3);
    key_pause = SDL_FALSE;
    key_up = SDL_FALSE;
    key_down = SDL_FALSE;
    key_left = SDL_FALSE;
    key_right = SDL_FALSE;
    key_mouse = SDL_FALSE;
    ClearEntityList(player->projectiles);
    ClearEntityList(ennemies);
    player_name[0] = '\0';
    player_name_title->texture = LoadTextureFromText("ENTER YOUR NAME", font_large, WHITE);
    RenderScore();
    ResetVolumeMusic();
}

void UpdateMenu(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = SDL_FALSE;
            return;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = SDL_FALSE;
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (cursor->rect.x == 0 && cursor->rect.y == 0) {
                current_music = res.level0_music;
                background = res.level0;
                easter_egg = 1;
            }
            else if (HEIGHT / 5 <= cursor->rect.y && cursor->rect.y <= 7 * HEIGHT / 15) {
                if (WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 2 * WIDTH / 5) {
                    current_music = res.level1_music;
                    background = res.level1;
                }
                else if (3 * WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 4 * WIDTH / 5) {
                    current_music = res.level2_music;
                    background = res.level2;
                }
            }
            else if (3 * HEIGHT / 5 <= cursor->rect.y && cursor->rect.y <= 13 * HEIGHT / 15) {
                if (WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 2 * WIDTH / 5) {
                    current_music = res.level3_music;
                    background = res.level3;
                }
                else if (3 * WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 4 * WIDTH / 5) {
                    current_music = res.level4_music;
                    background = res.level4;
                }
            }
        }
    }
}

void RenderMenu(void) {
    ClearScreen();
    SDL_Rect highlight = {0, 0, 0, 0};
    if (HEIGHT / 5 <= cursor->rect.y && cursor->rect.y <= 7 * HEIGHT / 15) {
        if (WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 2 * WIDTH / 5) {
            highlight.x = WIDTH / 5 - 3;
            highlight.y = HEIGHT / 5 - 3;
            highlight.w = WIDTH / 5 + 6;
            highlight.h = HEIGHT / 5 + 6;
        }
        else if (3 * WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 4 * WIDTH / 5) {
            highlight.x = 3 * WIDTH / 5 - 3;
            highlight.y = HEIGHT / 5 - 3;
            highlight.w = WIDTH / 5 + 6;
            highlight.h = HEIGHT / 5 + 6;
        }
    }
    else if (3 * HEIGHT / 5 <= cursor->rect.y && cursor->rect.y <= 13 * HEIGHT / 15) {
        if (WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 2 * WIDTH / 5) {
            highlight.x = WIDTH / 5 - 3;
            highlight.y = 3 * HEIGHT / 5 - 3;
            highlight.w = WIDTH / 5 + 6;
            highlight.h = HEIGHT / 5 + 6;
        }
        else if (3 * WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 4 * WIDTH / 5) {
            highlight.x = 3 * WIDTH / 5 - 3;
            highlight.y = 3 * HEIGHT / 5 - 3;
            highlight.w = WIDTH / 5 + 6;
            highlight.h = HEIGHT / 5 + 6;
        }
    }
    DrawRect(highlight, 255, 255, 255, 255);
    DrawTexture(res.level1, box1);
    DrawTexture(res.level2, box2);
    DrawTexture(res.level3, box3);
    DrawTexture(res.level4, box4);
    RefreshScreen();
}

int MenuShouldTransition(void) {
    return background != NULL;
}

void UpdateDifficulty(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            running = SDL_FALSE;
            break;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = SDL_FALSE;
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            if (2 * WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 3 * WIDTH / 5){
                if (HEIGHT / 6 <= cursor->rect.y && cursor->rect.y <= HEIGHT / 6 + HEIGHT / 9) {
                    InitEntity(drone, 128, 128, DRONE, 5, 2.5, 0.75, 4, 200, 1000, 60, 100);
                    InitEntity(drone_sb, 128, 128, DRONE_SUICIDE_BOMBER, 15, 0, 1, 6, 0, 0, 0, 100);
                    InitEntity(drone_nova, 141, 141, DRONE_NOVA, 25, 5, 0.75, 2, 100, 1000, 50, 100);
                    InitEntity(drone_hunter, 128, 128, DRONE_HUNTER, 50, 5, 1, 5, 125, 1250, 100, 100);
                    summon_delay_max = TimeToFrames(6);
                    difficulty = 1;
                } else if (2 * HEIGHT / 6 <= cursor->rect.y && cursor->rect.y <= 2 * HEIGHT / 6 + HEIGHT / 9) {
                    InitEntity(drone, 128, 128, DRONE, 10, 5, 1, 5, 160, 800, 80, 100);
                    InitEntity(drone_sb, 128, 128, DRONE_SUICIDE_BOMBER, 20, 0, 1.5, 7, 0, 0, 0, 95);
                    InitEntity(drone_nova, 141, 141, DRONE_NOVA, 30, 10, 1, 2.5, 80, 800, 75, 100);
                    InitEntity(drone_hunter, 128, 128, DRONE_HUNTER, 55, 10, 1.25, 6.25, 100, 1000, 100, 100);
                    summon_delay_max = TimeToFrames(5);
                    difficulty = 2;
                } else if (3 * HEIGHT / 6 <= cursor->rect.y && cursor->rect.y <= 3 * HEIGHT / 6 + HEIGHT / 9) {
                    InitEntity(drone, 128, 128, DRONE, 15, 10, 1.25, 6, 120, 600, 90, 100);
                    InitEntity(drone_sb, 128, 128, DRONE_SUICIDE_BOMBER, 25, 0, 2, 9, 0, 0, 0, 90);
                    InitEntity(drone_nova, 141, 141, DRONE_NOVA, 35, 15, 1.25, 3, 60, 600, 80, 100);
                    InitEntity(drone_hunter, 128, 128, DRONE_HUNTER, 65, 20, 1.5, 7.5, 75, 750, 100, 100);
                    summon_delay_max = TimeToFrames(4);
                    difficulty = 3;
                } else if (4 * HEIGHT / 6 <= cursor->rect.y && cursor->rect.y <= 4 * HEIGHT / 6 + HEIGHT / 9) {
                    InitEntity(drone, 128, 128, DRONE, 20, 15, 1.5, 7, 80, 400, 95, 100);
                    InitEntity(drone_sb, 128, 128, DRONE_SUICIDE_BOMBER, 30, 0, 2.5, 11, 0, 0, 0, 75);
                    InitEntity(drone_nova, 141, 141, DRONE_NOVA, 40, 20, 1.5, 3.5, 40, 400, 85, 100);
                    InitEntity(drone_hunter, 128, 128, DRONE_HUNTER, 80, 30, 1.75, 8.75, 50, 500, 100, 100);
                    summon_delay_max = TimeToFrames(3);
                    difficulty = 4;
                } else if (5 * HEIGHT / 6 <= cursor->rect.y && cursor->rect.y <= 5 * HEIGHT / 6 + HEIGHT / 9) {
                    InitEntity(drone, 128, 128, DRONE, 25, 20, 1.75, 8, 40, 200, 100, 100);
                    InitEntity(drone_sb, 128, 128, DRONE_SUICIDE_BOMBER, 40, 0, 3, 14, 0, 0, 0, 50);
                    InitEntity(drone_nova, 141, 141, DRONE_NOVA, 50, 25, 1.75, 4, 20, 200, 90, 100);
                    InitEntity(drone_hunter, 128, 128, DRONE_HUNTER, 100, 40, 2, 10, 25, 250, 100, 100);
                    summon_delay_max = TimeToFrames(2);
                    difficulty = 5;
                }
            }
        }
    }
}

void RenderDifficulty(void) {
    ClearScreen();
    DrawBackground(background);
    DrawButton(button1, 1, cursor->rect.x, cursor->rect.y, 6);
    DrawButton(button2, 1, cursor->rect.x, cursor->rect.y, 5);
    DrawButton(button3, 1, cursor->rect.x, cursor->rect.y, 1);
    DrawButton(button4, 1, cursor->rect.x, cursor->rect.y, 3);
    DrawButton(button5, 1, cursor->rect.x, cursor->rect.y, 2);
    RefreshScreen();
}

int DifficultyShouldTransition(void) {
    return difficulty != 0;
}

// --------------------------------------------------------
// Etat GAME
// --------------------------------------------------------
void InitGame(void) {
    drone_hunter->aim_delay = TimeToFrames(1);
    SummonEntity(player, 0, (float)WIDTH / 2, (float)HEIGHT / 2);
    player->speed = 8;
    PlayMusic(current_music, -1);
    SDL_ShowCursor(SDL_DISABLE);
}

void UpdateGame(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { running = SDL_FALSE; return; }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = SDL_FALSE;
            else if (event.key.keysym.scancode == SDL_SCANCODE_TAB)
                debug = !debug;
            else if (event.key.keysym.scancode == SDL_SCANCODE_PAUSE) {
                if (!key_pause) {
                    DrawRect(SCREEN, 0, 0, 0, 128);
                    DrawText(pause_title, 1);
                    RefreshScreen();
                }
                key_pause = !key_pause;
                SDL_ShowCursor(key_pause);
            }
            else {
                switch (event.key.keysym.sym) {
                    case SDLK_z: case SDLK_UP: key_up = SDL_TRUE; break;
                    case SDLK_s: case SDLK_DOWN: key_down = SDL_TRUE; break;
                    case SDLK_q: case SDLK_LEFT: key_left = SDL_TRUE; break;
                    case SDLK_d: case SDLK_RIGHT: key_right = SDL_TRUE; break;
                    default: break;
                }
            }
        }
        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_z: case SDLK_UP: key_up = SDL_FALSE; break;
                case SDLK_s: case SDLK_DOWN: key_down = SDL_FALSE; break;
                case SDLK_q: case SDLK_LEFT: key_left = SDL_FALSE; break;
                case SDLK_d: case SDLK_RIGHT: key_right = SDL_FALSE; break;
                default: break;
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            key_mouse = SDL_TRUE;
        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
            key_mouse = SDL_FALSE;
    }

    if (!key_pause) {
        if (key_left == key_right)
            player->velocity_x = 0;
        else if (key_left)
            player->velocity_x = SetVelocityX(-1);
        else if (key_right)
            player->velocity_x = SetVelocityX(1);

        if (key_up == key_down)
            player->velocity_y = 0;
        else if (key_up)
            player->velocity_y = SetVelocityY(-1);
        else if (key_down)
            player->velocity_y = SetVelocityY(1);

        if (key_mouse)
            EntityShoot(player, player->projectiles, plasma, (float)cursor->rect.x, (float)cursor->rect.y, 0, 0);

        // Génération des ennemis
        summon_delay--;
        if (!summon_delay) {
            summon_delay = randint(summon_delay_max / 5, summon_delay_max);
            summon_delay_max--;
            Entity *summoned = CreateEntity();
            const int rand_category = randint(0, 49);
            int lifetime;
            if (easter_egg || rand_category < 15) {
                CloneEntity(summoned, drone_sb);
                lifetime = -1;
            } else if (rand_category < 20) {
                CloneEntity(summoned, drone_nova);
                lifetime = TimeToFrames(15);
            } else if (rand_category == 20) {
                CloneEntity(summoned, drone_hunter);
                lifetime = -1;
            } else {
                CloneEntity(summoned, drone);
                lifetime = TimeToFrames(10);
            }
            const int rand_prestige = randint(0, 10000);
            if (rand_prestige < 100) {
                summoned->prestige = 1;
                AssignTexturePackToEntity(summoned, res.silver_drone_frames);
            } else if (rand_prestige == 100) {
                summoned->prestige = 2;
                AssignTexturePackToEntity(summoned, res.gold_drone_frames);
            }
            const int rand_side = randint(0, 1);
            if (!rand_side) {
                SummonEntity(summoned, lifetime, -drone->hitbox.w, (float)randint(0, HEIGHT - (int)drone->hitbox.h));
                summoned->velocity_x = SetVelocityX(1);
            } else {
                SummonEntity(summoned, lifetime, (float)WIDTH + drone->hitbox.w, (float)randint(0, HEIGHT - (int)drone->hitbox.h));
                summoned->velocity_x = SetVelocityX(-1);
            }
            AddEntity(ennemies, summoned);
        }

        EntityNode *node = ennemies->head;
        while (node) {
            EntityNode *next_entity = node->next;
            if (IsEntityAIEnabled(node->entity)) {
                if (IsEntityAlive(node->entity)) {
                    UpdateEntityState(node->entity);
                    switch (node->entity->type) {
                        case DRONE:
                            EntityShoot(node->entity, drone->projectiles, regular_bullet,
                                player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                            MoveEntity(node->entity, 1);
                            break;
                        case DRONE_SUICIDE_BOMBER:
                            EntityIntercept(node->entity, player->hitbox.x, player->hitbox.y,
                                player->hitbox.w, player->hitbox.h);
                            MoveEntity(node->entity, 1);
                            break;
                        case DRONE_NOVA:
                            EntityShoot(node->entity, drone->projectiles, nova,
                                player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                            MoveEntity(node->entity, 1);
                            break;
                        case DRONE_HUNTER:
                            if (AreEntitiesNear(node->entity, player, HEIGHT - scale_height(128))) {
                                EntityAim(node->entity, player->hitbox.x, player->hitbox.y,
                                    player->hitbox.w, player->hitbox.h);
                                EntityShoot(node->entity, drone->projectiles, sniper_bullet,
                                    player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                            } else {
                                EntityCease(node->entity);
                                EntityIntercept(node->entity, player->hitbox.x, player->hitbox.y,
                                    player->hitbox.w, player->hitbox.h);
                                MoveEntity(node->entity, 1);
                            }
                            break;
                        default:
                            MoveEntity(node->entity, 1);
                    }
                } else {
                    MoveEntity(node->entity, 1);
                    UpdateEntityState(node->entity);
                    node->entity->speed *= 1.005;
                }
            } else {
                RemoveEntity(ennemies, node->entity);
            }
            node = next_entity;
        }

        node = drone->projectiles->head;
        while (node) {
            EntityNode *next_projectile = node->next;
            if (IsEntityAIEnabled(node->entity)) {
                MoveEntity(node->entity, 1);
                UpdateEntityState(node->entity);
            } else {
                RemoveEntity(drone->projectiles, node->entity);
            }
            node = next_projectile;
        }

        MoveEntity(player, 0);
        UpdateEntityState(player);

        node = player->projectiles->head;
        while (node) {
            EntityNode *next_projectile = node->next;
            if (IsEntityAIEnabled(node->entity)) {
                MoveEntity(node->entity, 1);
                UpdateEntityState(node->entity);
            } else {
                RemoveEntity(player->projectiles, node->entity);
            }
            node = next_projectile;
        }

        node = ennemies->head;
        while (node) {
            EntityNode *next_entity = node->next;
            if (IsEntityAlive(node->entity)) {
                HandleCollision(&node->entity, &player);
                EntityNode *proj = player->projectiles->head;
                while (proj) {
                    EntityNode *next_projectile = proj->next;
                    HandleCollision(&node->entity, &proj->entity);
                    if (IsEntityDead(proj->entity))
                        DisableEntityAI(proj->entity);
                    proj = next_projectile;
                }
                if (IsEntityDead(node->entity)) {
                    node->entity->AI_lifetime = TimeToFrames(5);
                    node->entity->velocity_y = SetVelocityY(1);
                    node->entity->speed = 5;
                    GainScore(node->entity, difficulty);
                }
            }
            node = next_entity;
        }

        node = drone->projectiles->head;
        while (node) {
            EntityNode *next_projectile = node->next;
            HandleCollision(&node->entity, &player);
            if (IsEntityDead(node->entity))
                DisableEntityAI(node->entity);
            node = next_projectile;
        }
    }
}

void RenderGame(void) {
    if (!key_pause) {
        ClearScreen();
        DrawBackground(background);
        IterateEntities(drone->projectiles, DisplayEntity);
        IterateEntities(player->projectiles, DisplayEntity);
        IterateEntities(ennemies, DisplayEntityLaserBeam);
        IterateEntities(ennemies, DisplayEntity);
        DisplayEntity(player);
        DisplayHUD(player);
        DisplayEffect(player);
        DisplayCursor(cursor->texture, cursor->rect);
        RefreshScreen();
    }
}

int GameShouldTransition(void) {
    return player->hitpoints <= 0;
}

// --------------------------------------------------------
// Etat CINEMATIC
// --------------------------------------------------------
void InitCinematic(void) {
    EnableEntityAI(player, TimeToFrames(4));
    player->velocity_y = SetVelocityY(1);
    player->speed = 5;
    player->effect = 0;
}

void UpdateCinematic(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            running = SDL_FALSE;
            break;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = SDL_FALSE;
    }
    const EntityNode *node = ennemies->head;
    while (node) {
        const EntityNode *next_entity = node->next;
        UpdateEntityState(node->entity);
        if (IsEntityDead(node->entity))
            MoveEntity(node->entity, 1);
        node = next_entity;
    }
    DecreaseVolumeMusic(1);
    MoveEntity(player, 1);
    UpdateEntityState(player);
}

void RenderCinematic(void) {
    ClearScreen();
    DrawBackground(background);
    IterateEntities(ennemies, DisplayEntity);
    DrawRect(SCREEN, 0, 0, 0, 255 - player->AI_lifetime);
    DisplayEntity(player);
    RefreshScreen();
}

int CinematicShouldTransition(void) {
    return !IsEntityAIEnabled(player);
}

// --------------------------------------------------------
// Etat GAME OVER
// --------------------------------------------------------
void InitGameOver(void) {
    SDL_ShowCursor(SDL_ENABLE);
    Mix_HaltMusic();
    SDL_StartTextInput();
    text_input = SDL_TRUE;
    char score_str[16];
    sprintf(score_str, "SCORE: %d", score);
    score_title = CreateSolidText(rect(0, 4 * HEIGHT / 10, 1, 1, 0), VOID, score_str, font_large, WHITE);
    centered_abs(&score_title->rect);
}

void UpdateGameOver(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { running = SDL_FALSE; break; }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = SDL_FALSE;
            if (text_input) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    text_input = SDL_FALSE;
                    SDL_StopTextInput();
                    if (strlen(player_name)) {
                        player_name_title->texture = LoadTextureFromText(player_name, font_large, GREEN);
                        RegisterScore(player_name, difficulty);
                    } else {
                        player_name_title->texture = LoadTextureFromText("GUEST", font_large, GREEN);
                        RegisterScore("guest", difficulty);
                    }
                }
                if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(player_name) > 0) {
                    player_name[strlen(player_name) - 1] = '\0';
                    if (strlen(player_name) == 0)
                        player_name_title->texture = LoadTextureFromText("ENTER YOUR NAME", font_large, WHITE);
                    else
                        player_name_title->texture = LoadTextureFromText(player_name, font_large, WHITE);
                }
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (2 * WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 3 * WIDTH / 5 &&
                2 * HEIGHT / 3 <= cursor->rect.y && cursor->rect.y <= 2 * HEIGHT / 3 + HEIGHT / 9)
                restart = 1;
        }
        if (text_input && event.type == SDL_TEXTINPUT && strlen(player_name) < 8) {
            strcat(player_name, event.text.text);
            player_name_title->texture = LoadTextureFromText(player_name, font_large, WHITE);
        }
    }
}

void RenderGameOver(void) {
    ClearScreen();
    DrawText(game_over_title, 1);
    DrawText(score_title, 1);
    DrawText(player_name_title, 1);
    DrawButton(restart_button, 1, cursor->rect.x, cursor->rect.y, 7);
    RefreshScreen();
}

int GameOverShouldTransition(void) {
    return restart;
}

// --------------------------------------------------------
// Structure pour la table d'états
// --------------------------------------------------------
typedef struct {
    void (*init)(void);
    void (*update)(void);
    void (*render)(void);
    int (*shouldTransition)(void); // Retourne 1 si on doit passer à l'état suivant
} GameStateFunctions;

GameStateFunctions state_funcs[] = {
    [STATE_MENU]       = { InitMenu,      UpdateMenu,      RenderMenu,      MenuShouldTransition },
    [STATE_DIFFICULTY] = { NULL,          UpdateDifficulty, RenderDifficulty, DifficultyShouldTransition },
    [STATE_GAME]       = { InitGame,      UpdateGame,      RenderGame,      GameShouldTransition },
    [STATE_CINEMATIC]  = { InitCinematic, UpdateCinematic, RenderCinematic, CinematicShouldTransition },
    [STATE_GAMEOVER]   = { InitGameOver,  UpdateGameOver,  RenderGameOver,  GameOverShouldTransition }
};

// --------------------------------------------------------
// Fonction main : boucle principale et nettoyage final
// --------------------------------------------------------
int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (InitEngine() != 0)
        return -1;

    // Initialisation des polices
    font_small  = 15;
    font_medium = 25;
    font_large  = 50;
    font_xxl    = 100;
    font_jumbo  = 200;

    // Définition des zones pour les boutons
    box1 = rect(WIDTH / 5, HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);
    box2 = rect(3 * WIDTH / 5, HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);
    box3 = rect(WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);
    box4 = rect(3 * WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);

    LoadAllResources(&res);

    // Chargement des glyphes et initialisation du HUD
    CreateGlyphs();
    InitHUD();

    // Création des éléments de l'interface utilisateur
    cursor         = CreateSolid(rect(0, 0, 32, 32, 1), VOID, "crosshair.png");
    button1        = CreateSolidText(rect(0, HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      color(0,0,200,255), "EASY", font_large, WHITE);
    button2        = CreateSolidText(rect(0, 2 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      color(0,200,0,255), "MEDIUM", font_large, WHITE);
    button3        = CreateSolidText(rect(0, 3 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      color(200,200,0,255), "HARD", font_large, WHITE);
    button4        = CreateSolidText(rect(0, 4 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      color(200,0,0,255), "HARDER", font_large, WHITE);
    button5        = CreateSolidText(rect(0, 5 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      color(200,0,200,255), "INSANE", font_large, WHITE);
    restart_button = CreateSolidText(rect(0, 2 * HEIGHT / 3, WIDTH / 5, HEIGHT / 9, 0),
                                      color(100,100,100,255), "RESTART", 45, WHITE);
    game_over_title= CreateSolidText(rect(0, HEIGHT / 4, 1, 1, 0), VOID, "GAME OVER", font_xxl, WHITE);
    pause_title    = CreateSolidText(rect(0, 0, 1, 1, 0), VOID, "PAUSE", font_xxl, WHITE);
    player_name_title = CreateSolidText(rect(0, 11 * HEIGHT / 20, 1, 1, 0),
                                      VOID, "ENTER YOUR NAME", font_large, WHITE);
    score_title    = NULL; // Créé lors de l'initialisation de l'état Game Over

    centered_abs(&button1->rect);
    centered_abs(&button2->rect);
    centered_abs(&button3->rect);
    centered_abs(&button4->rect);
    centered_abs(&button5->rect);
    centered_abs(&restart_button->rect);
    centered_abs(&game_over_title->rect);
    centered_coord(&pause_title->rect);
    centered_abs(&player_name_title->rect);

    ennemies     = CreateEntityList();
    player       = CreateEntity();
    drone        = CreateEntity();
    drone_sb     = CreateEntity();
    drone_nova   = CreateEntity();
    drone_hunter = CreateEntity();
    regular_bullet = CreateEntity();
    sniper_bullet  = CreateEntity();
    plasma         = CreateEntity();
    nova           = CreateEntity();

    // Initialisation des entités
    InitEntity(player, 128, 128, 0, 100, 5, 1, 8, 10, 10, 90, 100);
    InitEntity(regular_bullet, 16, 16, 0, 1, 0, 0, 20, 0, 0, 0, 0);
    InitEntity(sniper_bullet, 16, 16, 0, 1, 0, 0, 30, 0, 0, 0, 0);
    InitEntity(plasma, 16, 16, 0, 1, 0, 0, 20, 0, 0, 0, 0);
    InitEntity(nova, 24, 24, 0, 1, 0, 0, 15, 0, 0, 0, 0);
    nova->elementary = 1;

    // Affecter les packs de textures et de sons aux entités en utilisant les ressources chargées
    AssignTexturePackToEntity(player, res.player_drone_frames);
    AssignTexturePackToEntity(drone, res.black_drone_frames);
    AssignTexturePackToEntity(drone_sb, res.black_drone_frames);
    AssignTexturePackToEntity(drone_nova, res.black_drone_frames);
    AssignTexturePackToEntity(drone_hunter, res.black_drone_frames);
    AssignTexturePackToEntity(regular_bullet, res.regular_bullet_frames);
    AssignTexturePackToEntity(plasma, res.plasma_frames);
    AssignTexturePackToEntity(nova, res.nova_frames);
    AssignTexturePackToEntity(sniper_bullet, res.sniper_bullet_frames);

    // Affecter des marques et des sons spécifiques
    drone_sb->mark    = res.sb_mark;
    drone_nova->mark  = res.nova_mark;
    drone_hunter->mark = res.hunter_mark;
    AssignSoundPackToEntity(drone, res.magnum_pistol_sounds);
    AssignSoundPackToEntity(player, res.plasma_pistol_sounds);
    AssignSoundPackToEntity(drone_nova, res.grenade_launcher_sounds);
    AssignSoundPackToEntity(drone_hunter, res.sniper_rifle_sounds);

    game_state = STATE_MENU;
    running = SDL_TRUE;

    // Boucle principale de la machine à états
    while (running) {
        const GameStateFunctions *currentState = &state_funcs[game_state];
        if (currentState->init)
            currentState->init();
        while (running && !currentState->shouldTransition()) {
            UpdateCursorPosition(cursor);
            currentState->update();
            currentState->render();
            // Vous pouvez ajouter ici un contrôle de framerate (exemple : SDL_Delay)
        }
        // Transition entre les états
        switch(game_state) {
            case STATE_MENU:
                game_state = STATE_DIFFICULTY;
                break;
            case STATE_DIFFICULTY:
                game_state = STATE_GAME;
                break;
            case STATE_GAME:
                game_state = STATE_CINEMATIC;
                break;
            case STATE_CINEMATIC:
                game_state = STATE_GAMEOVER;
                break;
            case STATE_GAMEOVER:
                game_state = restart ? STATE_MENU : STATE_GAMEOVER;
                break;
        }
    }

    // Nettoyage final
    FreeAllResources(&res);
    ClearHUD();
    ClearGlyphs();

    DestroySolid(cursor);
    DestroySolid(button1);
    DestroySolid(button2);
    DestroySolid(button3);
    DestroySolid(button4);
    DestroySolid(button5);
    DestroySolid(restart_button);
    DestroySolid(game_over_title);
    DestroySolid(pause_title);
    DestroySolid(score_title);
    DestroySolid(player_name_title);

    DestroyEntityList(ennemies);
    DestroyEntity(player);
    DestroyEntity(drone);
    DestroyEntity(drone_sb);
    DestroyEntity(drone_nova);
    DestroyEntity(drone_hunter);
    DestroyEntity(regular_bullet);
    DestroyEntity(sniper_bullet);
    DestroyEntity(plasma);
    DestroyEntity(nova);
    QuitEngine();
    return 0;
}

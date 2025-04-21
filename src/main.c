#define DEFINE_GLOBALS
#define DEFINE_CONSTANTS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "engine.h"      // Initialisation SDL, gestion du rendu, etc.
#include "graphics.h"
#include "globals.h"     // Variables globales (window, renderer, etc.)
#include "constants.h"   // Constantes (dimensions, couleurs, etc.)
#include "utils.h"       // Helpers de scaling, couleurs, etc.
#include "score.h"
#include "entity.h"      // Création et gestion des entités
#include "entity_list.h" // Gestion de liste d'entités
#include "hud.h"         // Affichage du HUD
#include "glyph.h"       // Gestion des glyphes pour le texte
#include "resource_pack.h"    // Gestion des texture/sound packs
#include "ui.h"          // Gestion de l'interface (boutons, texte, curseur)
#include "images.h"      // Chargement d'images & textures
#include "audio.h"       // Chargement et lecture audio

// Définition d'une machine à états pour le jeu
typedef enum {
    STATE_MENU,
    STATE_DIFFICULTY,
    STATE_GAME,
    STATE_CINEMATIC,
    STATE_GAMEOVER
} GameState;

GameState gameState = STATE_MENU;

_Bool easter_egg = 0;
_Bool restart = 0;

SDL_Rect box1, box2, box3, box4;

SDL_bool key_up = SDL_FALSE, key_down = SDL_FALSE, key_left = SDL_FALSE, key_right = SDL_FALSE, key_mouse = SDL_FALSE,
         key_pause = SDL_FALSE;

EntityNode *next_entity = NULL;
EntityNode *next_projectile = NULL;

Mix_Music *current_music = NULL;
Mix_Music *level1_music = NULL, *level2_music = NULL, *level3_music = NULL, *level4_music = NULL, *level0_music = NULL;

// Globalités (pour cet exemple, certaines variables sont globales, mais elles pourraient être encapsulées)
SDL_Texture *background = NULL;
SDL_Texture *level1 = NULL, *level2 = NULL, *level3 = NULL, *level4 = NULL, *level0 = NULL;
SDL_Texture *sb_mark = NULL, *nova_mark = NULL, *hunter_mark = NULL;
TexturePack *black_drone_frames = NULL, *silver_drone_frames = NULL, *gold_drone_frames = NULL, *player_drone_frames = NULL, *regular_bullet_frames = NULL,
            *sniper_bullet_frames = NULL, *plasma_frames = NULL, *nova_frames = NULL;
SoundPack *magnum_pistol_sounds = NULL, *plasma_pistol_sounds = NULL,
          *grenade_launcher_sounds = NULL, *sniper_rifle_sounds = NULL;

// Entités et listes
Entity *player = NULL, *drone = NULL, *drone_sb = NULL, *drone_nova = NULL, 
       *drone_hunter = NULL, *regular_bullet = NULL, *sniper_bullet = NULL, *plasma = NULL, *nova = NULL;
EntityList *ennemies = NULL;

// UI Elements
Solid *cursor = NULL;
Solid *button1 = NULL, *button2 = NULL, *button3 = NULL, *button4 = NULL, *button5 = NULL;
Solid *restart_button = NULL, *game_over_title = NULL, *pause_title = NULL, *player_name_title = NULL, *score_title = NULL;

// Variables de jeu
char player_name[9] = {0};
int difficulty = 0;
int summon_delay = 0, summon_delay_max = 0;
int score = 0;

// Pour la saisie de texte durant le Game Over
SDL_bool text_input = SDL_FALSE;

// ======================= Gestion des ressources =======================
void loadResources(void) {

    sb_mark = LoadTexture("sb.png");
    nova_mark = LoadTexture("nova.png");
    hunter_mark = LoadTexture("hunter.png");
    debug_hitbox = LoadTexture("debug.png");  // Optionnel pour debug

    level1 = LoadTexture("level1.png");
    level2 = LoadTexture("level2.png");
    level3 = LoadTexture("level3.png");
    level4 = LoadTexture("level4.png");
    level0 = LoadTexture(".Naval_ensign_of_the_Empire_of_Japan.png");

    level1_music = LoadMusic("05 Flollo.mp3");
    level2_music = LoadMusic("17 Best Guess At Best.mp3");
    level3_music = LoadMusic("10 Six-Armed Robbing Suit.mp3");
    level4_music = LoadMusic("11 Action Figure Hands.mp3");
    level0_music = LoadMusic(".Battotai - Imperial Japanese Army March.mp3");

    // Création de packs de textures
    black_drone_frames   = CreateTexturePack("drone", 4, 0, 1, RED);
    silver_drone_frames = CreateTexturePack("silver_drone", 4, 0, 1, RED);
    gold_drone_frames = CreateTexturePack("gold_drone", 4, 0, 1, RED);
    player_drone_frames  = CreateTexturePack("silver_drone", 4, 0, 1, BLUE);
    regular_bullet_frames= CreateTexturePack("regular_bullet", 9, 1, 0, VOID);
    sniper_bullet_frames = CreateTexturePack("sniper_bullet", 9, 1, 0, VOID);
    plasma_frames        = CreateTexturePack("plasma_orb", 9, 1, 0, VOID);
    nova_frames          = CreateTexturePack("nova", 4, 0, 0, VOID);

    // Création de packs de sons
    magnum_pistol_sounds  = CreateSoundPack("magnum", 4);
    plasma_pistol_sounds  = CreateSoundPack("plas_pistol_fire_reach", 3);
    grenade_launcher_sounds = CreateSoundPack("grenade_launcher_fire", 10);
    sniper_rifle_sounds   = CreateSoundPack("sniper_distant", 4);

}

void freeResources(void) {
    UnloadTexture(sb_mark);
    UnloadTexture(nova_mark);
    UnloadTexture(hunter_mark);
    UnloadTexture(debug_hitbox);

    UnloadTexture(level1);
    UnloadTexture(level2);
    UnloadTexture(level3);
    UnloadTexture(level4);
    UnloadTexture(level0);

    UnloadMusic(level0_music);
    UnloadMusic(level1_music);
    UnloadMusic(level2_music);
    UnloadMusic(level3_music);
    UnloadMusic(level4_music);

    DestroyTexturePack(black_drone_frames);
    DestroyTexturePack(silver_drone_frames);
    DestroyTexturePack(gold_drone_frames);
    DestroyTexturePack(player_drone_frames);
    DestroyTexturePack(regular_bullet_frames);
    DestroyTexturePack(sniper_bullet_frames);
    DestroyTexturePack(plasma_frames);
    DestroyTexturePack(nova_frames);

    DestroySoundPack(magnum_pistol_sounds);
    DestroySoundPack(plasma_pistol_sounds);
    DestroySoundPack(grenade_launcher_sounds);
    DestroySoundPack(sniper_rifle_sounds);
}

// ======================= Gestion des états =======================
void initMenu(void) {
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
    // Réinitialisation des textes de l'écran du menu
    player_name[0] = '\0';
    player_name_title->texture = LoadTextureFromText("ENTER YOUR NAME", font_large, WHITE);
    RenderScore();
    ResetVolumeMusic();
}
void updateMenu(void) {
    SDL_Event event;
    // Gestion des événements pour la sélection du niveau
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = SDL_FALSE;
            return;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = SDL_FALSE;
        // On traite un clic pour la sélection du background/niveau
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            // Exemple simplifié :
            if (cursor->rect.x == 0 && cursor->rect.y == 0){
                current_music = level0_music;
                //current_music_intro = level0_music;
                background = level0;
                easter_egg = 1;
            }
            else if (HEIGHT / 5. <= cursor->rect.y && cursor->rect.y <= 7 * HEIGHT / 15.){
                if (WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 2 * WIDTH / 5.) {
                    current_music = level1_music;
                    //current_music_intro = level1_music_intro;
                    background = level1;
                }
                else if (3 * WIDTH / 5. <= cursor->rect.x && cursor->rect.x <= 4 * WIDTH / 5.) {
                    current_music = level2_music;
                    //current_music_intro = level2_music_intro;
                    background = level2;
                }
            }
            else if (3 * HEIGHT / 5. <= cursor->rect.y && cursor->rect.y <= 13 * HEIGHT / 15.){
                if (WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 2 * WIDTH / 5.) {
                    current_music = level3_music;
                    //current_music_intro = level3_music_intro;
                    background = level3;
                }
                else if (3 * WIDTH / 5. <= cursor->rect.x && cursor->rect.x <= 4 * WIDTH / 5.) {
                    current_music = level4_music;
                    //current_music_intro = level4_music_intro;
                    background = level4;
                }
            }
        }
    }
}
void renderMenu(void) {
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
        else if (3 * WIDTH / 5. <= cursor->rect.x && cursor->rect.x <= 4 * WIDTH / 5) {
            highlight.x = 3 * WIDTH / 5 - 3;
            highlight.y = 3 * HEIGHT / 5 - 3;
            highlight.w = WIDTH / 5 + 6;
            highlight.h = HEIGHT / 5 + 6;
        }
    }
    DrawRect(highlight, 255, 255, 255, 255);
    DrawTexture(level1, box1);
    DrawTexture(level2, box2);
    DrawTexture(level3, box3);
    DrawTexture(level4, box4);
    RefreshScreen();
}

void updateDifficulty(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            running = SDL_FALSE;
            break;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = SDL_FALSE;
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
            if (2 * WIDTH / 5. <= cursor->rect.x && cursor->rect.x <= 3 * WIDTH / 5.){
                if (HEIGHT / 6. <= cursor->rect.y && cursor->rect.y <= HEIGHT / 6. + HEIGHT / 9.) {
                    InitEntity(drone, 128, 128, 0, 5, 2.5, 0.75, 4, 200, 1000, 60, 100);
                    InitEntity(drone_sb, 128, 128, 1, 15, 0, 1, 6, 0, 0, 0, 100);
                    InitEntity(drone_nova, 141, 141, 2, 25, 5, 0.75, 2, 100, 1000, 50, 100);
                    InitEntity(drone_hunter, 128, 128, 3, 50, 5, 1, 5, 125, 1250, 100, 100);
                    summon_delay_max = TimeToFrames(6);
                    difficulty = 1;
                } else if (2 * HEIGHT / 6. <= cursor->rect.y && cursor->rect.y <= 2 * HEIGHT / 6. + HEIGHT / 9.) {
                    InitEntity(drone, 128, 128, 0, 10, 5, 1, 5, 160, 800, 80, 100);
                    InitEntity(drone_sb, 128, 128, 1, 20, 0, 1.5, 7, 0, 0, 0, 95);
                    InitEntity(drone_nova, 141, 141, 2, 30, 10, 1, 2.5, 80, 800, 75, 100);
                    InitEntity(drone_hunter, 128, 128, 3, 55, 10, 1.25, 6.25, 100, 1000, 100, 100);
                    summon_delay_max = TimeToFrames(5);
                    difficulty = 2;
                } else if (3 * HEIGHT / 6. <= cursor->rect.y && cursor->rect.y <= 3 * HEIGHT / 6. + HEIGHT / 9.) {
                    InitEntity(drone, 128, 128, 0, 15, 10, 1.25, 6, 120, 600, 90, 100);
                    InitEntity(drone_sb, 128, 128, 1, 25, 0, 2, 9, 0, 0, 0, 90);
                    InitEntity(drone_nova, 141, 141, 2, 35, 15, 1.25, 3, 60, 600, 80, 100);
                    InitEntity(drone_hunter, 128, 128, 3, 65, 20, 1.5, 7.5, 75, 750, 100, 100);
                    summon_delay_max = TimeToFrames(4);
                    difficulty = 3;
                } else if (4 * HEIGHT / 6. <= cursor->rect.y && cursor->rect.y <= 4 * HEIGHT / 6. + HEIGHT / 9.) {
                    InitEntity(drone, 128, 128, 0, 20, 15, 1.5, 7, 80, 400, 95, 100);
                    InitEntity(drone_sb, 128, 128, 1, 30, 0, 2.5, 11, 0, 0, 0, 75);
                    InitEntity(drone_nova, 141, 141, 2, 40, 20, 1.5, 3.5, 40, 400, 85, 100);
                    InitEntity(drone_hunter, 128, 128, 3, 80, 30, 1.75, 8.75, 50, 500, 100, 100);
                    summon_delay_max = TimeToFrames(3);
                    difficulty = 4;
                } else if (5 * HEIGHT / 6. <= cursor->rect.y && cursor->rect.y <= 5 * HEIGHT / 6. + HEIGHT / 9.) {
                    InitEntity(drone, 128, 128, 0, 25, 20, 1.75, 8, 40, 200, 100, 100);
                    InitEntity(drone_sb, 128, 128, 1, 40, 0, 3, 14, 0, 0, 0, 50);
                    InitEntity(drone_nova, 141, 141, 2, 50, 25, 1.75, 4, 20, 200, 90, 100);
                    InitEntity(drone_hunter, 128, 128, 3, 100, 40, 2, 10, 25, 250, 100, 100);
                    summon_delay_max = TimeToFrames(2);
                    difficulty = 5;
                }
            }
        }
    }
}

void renderDifficulty(void){
    ClearScreen();
    DrawBackground(background);
    DrawButton(button1, 1, cursor->rect.x, cursor->rect.y, 6);
    DrawButton(button2, 1, cursor->rect.x, cursor->rect.y, 5);
    DrawButton(button3, 1, cursor->rect.x, cursor->rect.y, 1);
    DrawButton(button4, 1, cursor->rect.x, cursor->rect.y, 3);
    DrawButton(button5, 1, cursor->rect.x, cursor->rect.y, 2);
    RefreshScreen();
}

void initGame(void) {

    drone_hunter->aim_delay = 60;
    
    SummonEntity(player, 0, (float)WIDTH/2, (float)HEIGHT/2);
    player->speed = 8;

    PlayMusic(current_music, -1);
    SDL_ShowCursor(SDL_DISABLE);
}
void updateGame(void) {
    SDL_Event event;
    // Gestion des entrées (déplacements, tirs)
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { running = SDL_FALSE; return; }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = SDL_FALSE;
            else if (event.key.keysym.scancode == SDL_SCANCODE_TAB)
                debug = !debug;
            else if (event.key.keysym.scancode == SDL_SCANCODE_PAUSE){
                if (!key_pause){
                    DrawRect(SCREEN, 0, 0, 0, 128);
                    DrawText(pause_title, 1);
                    RefreshScreen();
                }
                key_pause = !key_pause;
                SDL_ShowCursor(key_pause);
            }
            // Gestion des touches pour le déplacement
            else switch (event.key.keysym.sym) {
                case SDLK_z: case SDLK_UP: key_up = SDL_TRUE; break;
                case SDLK_s: case SDLK_DOWN: key_down = SDL_TRUE; break;
                case SDLK_q: case SDLK_LEFT: key_left = SDL_TRUE; break;
                case SDLK_d: case SDLK_RIGHT: key_right = SDL_TRUE; break;
                default: break;
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
    if (!key_pause){
        // Mise à jour des mouvements du joueur
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

        // Génération des ennemis (simplifiée)
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
                AssignTexturePackToEntity(summoned, silver_drone_frames);
            } else if (rand_prestige == 100) {
                summoned->prestige = 2;
                AssignTexturePackToEntity(summoned, gold_drone_frames);
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

        // Mise à jour des ennemis
        EntityNode *node = ennemies->head;
        while (node) {
            next_entity = node->next;
            if (IsEntityAIEnabled(node->entity)) {
                if (IsEntityAlive(node->entity)) {
                    //DisplayEntity(node->entity);
                    UpdateEntityState(node->entity);
                    switch (node->entity->category) {
                    case 0:
                        EntityShoot(node->entity, drone->projectiles, regular_bullet,
                                    player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                        MoveEntity(node->entity, 1);
                        break;
                    case 1:
                        EntityIntercept(node->entity, player->hitbox.x, player->hitbox.y,
                                        player->hitbox.w, player->hitbox.h);
                        MoveEntity(node->entity, 1);
                        break;
                    case 2:
                        EntityShoot(node->entity, drone->projectiles, nova,
                                    player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                        MoveEntity(node->entity, 1);
                        break;
                    case 3:
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
                    //DisplayEntity(node->entity);
                    UpdateEntityState(node->entity);
                    node->entity->speed *= 1.005;
                }
            } else {
                RemoveEntity(ennemies, node->entity);
            }
            node = next_entity;
        }

        // Mise à jour des projectiles ennemis
        node = drone->projectiles->head;
        while (node) {
            next_projectile = node->next;
            if (IsEntityAIEnabled(node->entity)) {
                MoveEntity(node->entity, 1);
                //DisplayEntity(node->entity);
                UpdateEntityState(node->entity);
            } else {
                RemoveEntity(drone->projectiles, node->entity);
            }
            node = next_projectile;
        }

        // Mise à jour du joueur
        MoveEntity(player, 0);
        //DisplayEntity(player);
        UpdateEntityState(player);

        // Mise à jour des projectiles du joueur
        node = player->projectiles->head;
        while (node) {
            next_projectile = node->next;
            if (IsEntityAIEnabled(node->entity)) {
                MoveEntity(node->entity, 1);
                //DisplayEntity(node->entity);
                UpdateEntityState(node->entity);
            } else {
                RemoveEntity(player->projectiles, node->entity);
            }
            node = next_projectile;
        }

        // Collision entre ennemis et joueur + collision entre ennemis et projectiles du joueur
        node = ennemies->head;
        while (node) {
            next_entity = node->next;
            if (IsEntityAlive(node->entity)) {
                HandleCollision(&node->entity, &player);
                EntityNode *proj = player->projectiles->head;
                while (proj) {
                    next_projectile = proj->next;
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

        // Collision entre projectiles ennemis et joueur
        node = drone->projectiles->head;
        while (node) {
            next_projectile = node->next;
            HandleCollision(&node->entity, &player);
            if (IsEntityDead(node->entity))
                DisableEntityAI(node->entity);
            node = next_projectile;
        }
    }
}

void renderGame(void) {
    if (!key_pause){
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

void initCinematic(void){
    EnableEntityAI(player, TimeToFrames(4));
    player->velocity_y = SetVelocityY(1);
    player->speed = 5;
    player->effect = 0;
}

void updateCinematic(void){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            running = SDL_FALSE;
            break;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = SDL_FALSE;
    }
    const EntityNode *node = ennemies->head;
    while (node){
        next_entity = node->next;
        UpdateEntityState(node->entity);
        if (IsEntityDead(node->entity))
            MoveEntity(node->entity, 1);
        node = next_entity;
    }
    DecreaseVolumeMusic(1);
    MoveEntity(player, 1);
    UpdateEntityState(player);
}

void renderCinematic(void){
    ClearScreen();
    DrawBackground(background);
    IterateEntities(ennemies, DisplayEntity);
    DrawRect(SCREEN, 0, 0, 0, 255 - player->AI_lifetime);
    DisplayEntity(player);
    RefreshScreen();
}

void initGameOver(void) {
    SDL_ShowCursor(SDL_ENABLE);
    Mix_HaltMusic();
    SDL_StartTextInput();
    text_input = SDL_TRUE;
    char score_str[16];
    sprintf(score_str, "SCORE: %d", score);
    score_title = CreateSolidText(rect(0, 4 * HEIGHT / 10, 1, 1, 0), VOID, score_str, font_large, WHITE);
    centered_abs(&score_title->rect);
}

void updateGameOver(void) {
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
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            if (2 * WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 3 * WIDTH / 5 &&
                2 * HEIGHT / 3 <= cursor->rect.y && cursor->rect.y <= 2 * HEIGHT / 3 + HEIGHT / 9)
                restart = 1;
        if (text_input && event.type == SDL_TEXTINPUT && strlen(player_name) < 8) {
            strcat(player_name, event.text.text);
            player_name_title->texture = LoadTextureFromText(player_name, font_large, WHITE);
        }
    }
}

void renderGameOver(void) {
    ClearScreen();
    DrawText(game_over_title, 1);
    DrawText(score_title, 1);
    DrawText(player_name_title, 1);
    DrawButton(restart_button, 1, cursor->rect.x, cursor->rect.y, 7);
    RefreshScreen();
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (InitEngine() != 0)
        return -1;

    font_small = 15;
    font_medium = 25;
    font_large = 50;
    font_xxl = 100;
    font_jumbo = 200;

    box1 = rect(WIDTH / 5, HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);
    box2 = rect(3 * WIDTH / 5, HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);
    box3 = rect(WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);
    box4 = rect(3 * WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);

    loadResources();
    // Chargement des glyphes pour le texte
    CreateGlyphs();
    InitHUD();


    // Création des UI
    cursor = CreateSolid(rect(0, 0, 32, 32, 1), VOID, "crosshair.png");
    button1 = CreateSolidText(rect(0, HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(0,0,200,255), "EASY", font_large, WHITE);
    button2 = CreateSolidText(rect(0, 2 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(0,200,0,255), "MEDIUM", font_large, WHITE);
    button3 = CreateSolidText(rect(0, 3 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(200,200,0,255), "HARD", font_large, WHITE);
    button4 = CreateSolidText(rect(0, 4 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(200,0,0,255), "HARDER", font_large, WHITE);
    button5 = CreateSolidText(rect(0, 5 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(200,0,200,255), "INSANE", font_large, WHITE);
    restart_button = CreateSolidText(rect(0, 2 * HEIGHT / 3, WIDTH / 5, HEIGHT / 9, 0), color(100,100,100,255), "RESTART", 45, WHITE);
    game_over_title = CreateSolidText(rect(0, HEIGHT / 4, 1, 1, 0), VOID, "GAME OVER", font_xxl, WHITE);
    pause_title = CreateSolidText(rect(0, 0, 1, 1, 0), VOID, "PAUSE", font_xxl, WHITE);
    player_name_title = CreateSolidText(rect(0, 11 * HEIGHT / 20, 1, 1, 0), VOID, "ENTER YOUR NAME", font_large, WHITE);
    score_title = NULL;//CreateSolidText(rect(0, 4 * HEIGHT / 10, 1, 1, 0), VOID, "SCORE: 0", font_large, WHITE);
    centered_abs(&button1->rect);
    centered_abs(&button2->rect);
    centered_abs(&button3->rect);
    centered_abs(&button4->rect);
    centered_abs(&button5->rect);
    centered_abs(&restart_button->rect);
    centered_abs(&game_over_title->rect);
    centered_coord(&pause_title->rect);
    centered_abs(&player_name_title->rect);
    //centered_abs(&score_title->rect);
    
    ennemies = CreateEntityList();
    player = CreateEntity();
    drone = CreateEntity();
    drone_sb = CreateEntity();
    drone_nova = CreateEntity();
    drone_hunter = CreateEntity();
    regular_bullet = CreateEntity();
    sniper_bullet = CreateEntity();
    plasma = CreateEntity();
    nova = CreateEntity();

    InitEntity(player, 128, 128, 0, 100, 5, 1, 8, 10, 10, 90, 100);
    InitEntity(regular_bullet, 16, 16, 0, 1, 0, 0, 20, 0, 0, 0, 0);
    InitEntity(sniper_bullet, 16, 16, 0, 1, 0, 0, 30, 0, 0, 0, 0);
    InitEntity(plasma, 16, 16, 0, 1, 0, 0, 20, 0, 0, 0, 0);
    InitEntity(nova, 24, 24, 0, 1, 0, 0, 15, 0, 0, 0, 0);
    nova->elementary = 1;

    // Affecter des texture packs aux entités
    AssignTexturePackToEntity(player, player_drone_frames);
    AssignTexturePackToEntity(drone, black_drone_frames);
    AssignTexturePackToEntity(drone_sb, black_drone_frames);
    AssignTexturePackToEntity(drone_nova, black_drone_frames);
    AssignTexturePackToEntity(drone_hunter, black_drone_frames);
    AssignTexturePackToEntity(regular_bullet, regular_bullet_frames);
    AssignTexturePackToEntity(plasma, plasma_frames);
    AssignTexturePackToEntity(nova, nova_frames);
    AssignTexturePackToEntity(sniper_bullet, sniper_bullet_frames);

    // Affectation des marques
    drone_sb->mark = sb_mark;
    drone_nova->mark = nova_mark;
    drone_hunter->mark = hunter_mark;

    // Affectation des sons
    AssignSoundPackToEntity(drone, magnum_pistol_sounds);
    AssignSoundPackToEntity(player, plasma_pistol_sounds);
    AssignSoundPackToEntity(drone_nova, grenade_launcher_sounds);
    AssignSoundPackToEntity(drone_hunter, sniper_rifle_sounds);
    
    gameState = STATE_MENU;
    running = SDL_TRUE;

    // Boucle principale de la machine à états
    while (running) {
        switch(gameState) {
            case STATE_MENU:
                initMenu();
                while (running && gameState == STATE_MENU) {
                    SDL_GetMouseState(&cursor->rect.x, &cursor->rect.y);
                    updateMenu();
                    renderMenu();
                    // Par exemple, si un background est choisi et une difficulté définie, passer en jeu
                    if (background)
                        gameState = STATE_DIFFICULTY;
                    //SDL_Delay(16);
                }
                break;
            case STATE_DIFFICULTY:
                while (running && gameState == STATE_DIFFICULTY){
                    SDL_GetMouseState(&cursor->rect.x, &cursor->rect.y);
                    updateDifficulty();
                    renderDifficulty();
                    if (difficulty)
                        gameState = STATE_GAME;
                    //SDL_Delay(16);
                }
            case STATE_GAME:
                initGame();
                while (running && gameState == STATE_GAME && player->hitpoints > 0) {
                    SDL_GetMouseState(&cursor->rect.x, &cursor->rect.y);
                    updateGame();
                    renderGame();
                    //SDL_Delay(16);
                }
                if (running)
                    gameState = STATE_CINEMATIC;
                break;
            case STATE_CINEMATIC:
                initCinematic();
                while (running && gameState == STATE_CINEMATIC && IsEntityAIEnabled(player)) {
                    updateCinematic();
                    renderCinematic();
                    //SDL_Delay(16);
                }
                if (running)
                    gameState = STATE_GAMEOVER;
                break;
            case STATE_GAMEOVER:
                initGameOver();
                while (running && gameState == STATE_GAMEOVER) {
                    SDL_GetMouseState(&cursor->rect.x, &cursor->rect.y);
                    updateGameOver();
                    renderGameOver();
                    // Si restart est déclenché, on revient au menu
                    if (restart)
                        gameState = STATE_MENU;
                    //SDL_Delay(16);
                }
                break;
        }
    }
    // Nettoyage final
    freeResources();
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


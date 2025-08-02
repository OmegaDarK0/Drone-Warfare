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
#include "player.h"
#include "entity.h"          // Création et gestion des entités
#include "entity_list.h"     // Gestion de liste d'entités
#include "hud.h"             // Affichage du HUD
#include "glyph.h"           // Gestion des glyphes pour le texte
#include "resource_pack.h"   // Gestion des texture/sound packs
#include "ui.h"              // Gestion de l'interface (boutons, texte, curseur)
#include "images.h"          // Chargement d'images & textures
#include "audio.h"           // Chargement et lecture audio
#include "menu.h"
#include "input.h"

void LoadResourcePack(ResourceManager *res) {
    res->explosive_mark             = LoadTexture("explosive_mark.png");
    res->nova_mark           = LoadTexture("nova_mark.png");
    res->hunter_mark         = LoadTexture("hunter_mark.png");
    res->entity_debug_hitbox        = LoadTexture("debug.png");

    res->level1_background              = LoadTexture("level1.png");
    res->level2_background              = LoadTexture("level2.png");
    res->level3_background              = LoadTexture("level3.png");
    res->level4_background              = LoadTexture("level4.png");
    res->level0_background              = LoadTexture(".Naval_ensign_of_the_Empire_of_Japan.png");

    res->level1_music        = LoadMusic("05 Flollo.mp3");
    res->level2_music        = LoadMusic("17 Best Guess At Best.mp3");
    res->level3_music        = LoadMusic("10 Six-Armed Robbing Suit.mp3");
    res->level4_music        = LoadMusic("11 Action Figure Hands.mp3");
    res->level0_music        = LoadMusic(".Battotai - Imperial Japanese Army March.mp3");

    res->drone_frames  = CreateTexturePack("drone", RED, 1, 1);
    res->silver_drone_frames = CreateTexturePack("silver_drone", RED, 1, 1);
    res->gold_drone_frames   = CreateTexturePack("gold_drone", RED, 1, 1);
    res->player_drone_frames = CreateTexturePack("silver_drone", BLUE, 1, 1);
    res->regular_bullet_frames = CreateTexturePack("regular_bullet", VOID, 0, 0);
    res->sniper_bullet_frames  = CreateTexturePack("sniper_bullet", VOID, 0, 0);
    res->plasma_orb_frames       = CreateTexturePack("plasma_orb", VOID, 0, 0);
    res->nova_orb_frames         = CreateTexturePack("nova_orb", VOID, 0, 0);

    res->magnum_pistol_sounds = CreateSoundPack("magnum", 4);
    res->plasma_pistol_sounds = CreateSoundPack("plas_pistol_fire_reach", 3);
    res->grenade_launcher_sounds = CreateSoundPack("grenade_launcher_fire", 10);
    res->sniper_rifle_sounds = CreateSoundPack("sniper_distant", 4);
}

void FreeResourcePack(const ResourceManager *res) {
    UnloadTexture(res->explosive_mark);
    UnloadTexture(res->nova_mark);
    UnloadTexture(res->hunter_mark);
    UnloadTexture(res->entity_debug_hitbox);

    UnloadTexture(res->level1_background);
    UnloadTexture(res->level2_background);
    UnloadTexture(res->level3_background);
    UnloadTexture(res->level4_background);
    UnloadTexture(res->level0_background);

    UnloadMusic(res->level0_music);
    UnloadMusic(res->level1_music);
    UnloadMusic(res->level2_music);
    UnloadMusic(res->level3_music);
    UnloadMusic(res->level4_music);

    DestroyTexturePack(res->drone_frames);
    DestroyTexturePack(res->silver_drone_frames);
    DestroyTexturePack(res->gold_drone_frames);
    DestroyTexturePack(res->player_drone_frames);
    DestroyTexturePack(res->regular_bullet_frames);
    DestroyTexturePack(res->sniper_bullet_frames);
    DestroyTexturePack(res->plasma_orb_frames);
    DestroyTexturePack(res->nova_orb_frames);

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
SDL_bool game_running = SDL_TRUE;
SDL_bool game_restart = SDL_FALSE;
SDL_bool game_debug = SDL_FALSE;
int game_difficulty = 0;
int player_score = 0;

SDL_Texture *current_background = NULL;
Mix_Music *current_music = NULL;

// Prototypes des fonctions d'initialisation, mise à jour, rendu et condition de transition pour chaque état
void InitLevelMenu(void);
void UpdateLevelMenu(void);
void RenderLevelMenu(void);
int LevelMenuShouldTransition(void);

void UpdateDifficultyMenu(void);
void RenderDifficultyMenu(void);
int DifficultyMenuShouldTransition(void);

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

void InitLevelMenu(void) {
    easter_egg = 0;
    game_restart = SDL_FALSE;
    game_difficulty = SDL_FALSE;
    current_background = NULL;
    player_score = 0;
    entity_count = 0;
    summon_delay = TimeToFrames(3);
    key_pause = SDL_FALSE;
    key_up = SDL_FALSE;
    key_down = SDL_FALSE;
    key_left = SDL_FALSE;
    key_right = SDL_FALSE;
    key_mouse = SDL_FALSE;
    ClearEntityList(ennemies);
    ResetPlayerInfo(&player_info);
    DestroySolid(score_title);
    RenderScore();
    ResetVolumeMusic();
}

void UpdateLevelMenu(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
           (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
            game_running = SDL_FALSE;
            return;
           }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            const SDL_Point cursor_pos = { cursor->rect.x, cursor->rect.y };
            HandleLevelSelection(cursor_pos);
        }
    }
}

void RenderLevelMenu(void) {
    ClearScreen();
    const SDL_Point cursor_pos = { cursor->rect.x, cursor->rect.y };
    const SDL_Rect *highlight = GetHighlightRect(cursor_pos);
    if (highlight) {
        SDL_Rect hl = *highlight;
        hl.x -= 3;
        hl.y -= 3;
        hl.w += 6;
        hl.h += 6;
        DrawRect(hl, 255, 255, 255, 255);
    }
    DrawTexture(res.level1_background, level1_container);
    DrawTexture(res.level2_background, level2_container);
    DrawTexture(res.level3_background, level3_container);
    DrawTexture(res.level4_background, level4_container);
    RefreshScreen();
}

int LevelMenuShouldTransition(void) {
    return current_background != NULL;
}

void UpdateDifficultyMenu(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
           (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
            game_running = SDL_FALSE;
            return;
           }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            SDL_Point cursor_pos = { cursor->rect.x, cursor->rect.y };
            HandleDifficultySelection(&cursor_pos);
        }
    }
}

void RenderDifficultyMenu(void) {
    ClearScreen();
    DrawBackground(current_background);
    DrawButton(easy_button, 1, cursor->rect.x, cursor->rect.y, 6);
    DrawButton(medium_button, 1, cursor->rect.x, cursor->rect.y, 5);
    DrawButton(hard_button, 1, cursor->rect.x, cursor->rect.y, 1);
    DrawButton(harder_button, 1, cursor->rect.x, cursor->rect.y, 3);
    DrawButton(insane_button, 1, cursor->rect.x, cursor->rect.y, 2);
    RefreshScreen();
}

int DifficultyMenuShouldTransition(void) {
    return game_difficulty != 0;
}

// --------------------------------------------------------
// Etat GAME
// --------------------------------------------------------
void InitGame(void) {
    drone_hunter->aim_delay = TimeToFrames(1);
    SummonEntity(player, 0, (float)(WIDTH / 2. - ScaleWidth(64)), (float)(HEIGHT / 2. - ScaleHeight(64)));
    player->speed = 8;
    PlayMusic(current_music, -1);
    SDL_ShowCursor(SDL_DISABLE);
}

void UpdateGame(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        HandleKeyboardEvent(&event);
    }

    if (!key_pause) {
        UpdatePlayer();
        HandleSummonLogic();
        UpdateEntities();
        UpdateProjectiles(drone->projectiles);
        MoveEntity(player, 0);
        UpdateEntityState(player);
        UpdateProjectiles(player->projectiles);
        CheckCollisions();
    }
}

void RenderGame(void) {
    if (!key_pause) {
        ClearScreen();
        DrawBackground(current_background);
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
    EnableEntityAI(player, TimeToFrames(3));
    player->velocity_y = SetVelocityY(1);
    player->speed = 5;
    player->effect = 0;
}

void UpdateCinematic(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            game_running = SDL_FALSE;
            break;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            game_running = SDL_FALSE;
    }
    const EntityNode *node = ennemies->head;
    while (node) {
        const EntityNode *next_entity = node->next;
        UpdateEntityState(node->entity);
        if (IsEntityDead(node->entity))
            MoveEntity(node->entity, 1);
        node = next_entity;
    }
    player->speed *= 1.005;
    MoveEntity(player, 1);
    DecreaseVolumeMusic(1);
    UpdateEntityState(player);
}

void RenderCinematic(void) {
    ClearScreen();
    DrawBackground(current_background);
    IterateEntities(ennemies, DisplayEntity);
    DrawRect(SCREEN, 0, 0, 0, 255 - (int)((double) player->AI_lifetime / TimeToFrames(3) * 255));
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
    ClearEntityList(player->projectiles);
    ClearEntityList(drone->projectiles);
    SDL_ShowCursor(SDL_ENABLE);
    Mix_HaltMusic();
    SDL_StartTextInput();
    text_input = SDL_TRUE;
    char score_str[16];
    sprintf(score_str, "SCORE: %d", player_score);
    score_title = CreateSolidText(Rect(0,  3 * HEIGHT / 10, 1, 1, 0), VOID, score_str, font_large, WHITE);
    CenteredAbs(&score_title->rect);
}

void UpdateGameOver(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { game_running = SDL_FALSE; break; }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                game_running = SDL_FALSE;
            if (text_input) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    text_input = SDL_FALSE;
                    SDL_StopTextInput();
                    if (player_info.length) {
                        player_info.title->texture = LoadTextureFromText(player_info.name, font_large, GREEN);
                        RegisterScore(player_info.name, game_difficulty);
                    } else {
                        player_info.title->texture = LoadTextureFromText("GUEST", font_large, GREEN);
                        RegisterScore("guest", game_difficulty);
                    }
                }
                if (event.key.keysym.sym == SDLK_BACKSPACE && player_info.length) {
                    player_info.name[--player_info.length] = '\0';
                    if (player_info.length == 0)
                        player_info.title->texture = LoadTextureFromText("ENTER YOUR NAME", font_large, WHITE);
                    else
                        player_info.title->texture = LoadTextureFromText(player_info.name, font_large, WHITE);
                }
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (2 * WIDTH / 5 <= cursor->rect.x && cursor->rect.x <= 3 * WIDTH / 5 &&
                2 * HEIGHT / 3 <= cursor->rect.y && cursor->rect.y <= 2 * HEIGHT / 3 + HEIGHT / 9)
                game_restart = 1;
        }
        if (text_input && event.type == SDL_TEXTINPUT && player_info.length < 8) {
            strcat(player_info.name, event.text.text);
            player_info.length++;
            player_info.title->texture = LoadTextureFromText(player_info.name, font_large, WHITE);
        }
    }
}

void RenderGameOver(void) {
    ClearScreen();
    DrawText(game_over_title, 1);
    DrawText(score_title, 1);
    DrawText(player_info.title, 1);
    DrawButton(restart_button, 1, cursor->rect.x, cursor->rect.y, 7);
    RefreshScreen();
}

int GameOverShouldTransition(void) {
    return game_restart;
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
    [STATE_MENU]       = { InitLevelMenu,      UpdateLevelMenu,      RenderLevelMenu,      LevelMenuShouldTransition },
    [STATE_DIFFICULTY] = { NULL,          UpdateDifficultyMenu, RenderDifficultyMenu, DifficultyMenuShouldTransition },
    [STATE_GAME]       = { InitGame,      UpdateGame,      RenderGame,      GameShouldTransition },
    [STATE_CINEMATIC]  = { InitCinematic, UpdateCinematic, RenderCinematic, CinematicShouldTransition },
    [STATE_GAMEOVER]   = { InitGameOver,  UpdateGameOver,  RenderGameOver,  GameOverShouldTransition }
};

// --------------------------------------------------------
// Fonction main : boucle principale et nettoyage final
// --------------------------------------------------------
#ifdef _WIN32
int WinMain(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
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
    level1_container = Rect(WIDTH / 5, HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);
    level2_container = Rect(3 * WIDTH / 5, HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);
    level3_container = Rect(WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);
    level4_container = Rect(3 * WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, HEIGHT / 5, 0);

    LoadResourcePack(&res);

    SetLevelMenu();
    SetDifficultyMenu();

    // Chargement des glyphes et initialisation du HUD
    CreateGlyphs();
    InitHUD();

    // Création des éléments de l'interface utilisateur
    cursor         = CreateSolid(Rect(0, 0, 32, 32, 1), VOID, "crosshair.png");
    easy_button        = CreateSolidText(Rect(0, HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      Color(0,0,200,255), "EASY", font_large, WHITE);
    medium_button        = CreateSolidText(Rect(0, 2 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      Color(0,200,0,255), "MEDIUM", font_large, WHITE);
    hard_button        = CreateSolidText(Rect(0, 3 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      Color(200,200,0,255), "HARD", font_large, WHITE);
    harder_button        = CreateSolidText(Rect(0, 4 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      Color(200,0,0,255), "HARDER", font_large, WHITE);
    insane_button        = CreateSolidText(Rect(0, 5 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0),
                                      Color(200,0,200,255), "INSANE", font_large, WHITE);
    restart_button = CreateSolidText(Rect(0, 3 * HEIGHT / 4, WIDTH / 5, HEIGHT / 9, 0),
                                      Color(100,100,100,255), "RESTART", 45, WHITE);
    game_over_title= CreateSolidText(Rect(0, 3 * HEIGHT / 20, 1, 1, 0), VOID, "GAME OVER", font_xxl, WHITE);
    pause_title    = CreateSolidText(Rect(0, 0, 1, 1, 0), VOID, "PAUSE", font_xxl, WHITE);
    player_info.title = CreateSolidText(Rect(0, 11 * HEIGHT / 20, 1, 1, 0),
                                      VOID, "ENTER YOUR NAME", font_large, WHITE);
    score_title    = NULL; // Créé lors de l'initialisation de l'état Game Over

    CenteredAbs(&easy_button->rect);
    CenteredAbs(&medium_button->rect);
    CenteredAbs(&hard_button->rect);
    CenteredAbs(&harder_button->rect);
    CenteredAbs(&insane_button->rect);
    CenteredAbs(&restart_button->rect);
    CenteredAbs(&game_over_title->rect);
    CenteredCoord(&pause_title->rect);
    CenteredAbs(&player_info.title->rect);

    ennemies     = CreateEntityList();
    player       = CreateEntity();
    drone        = CreateEntity();
    drone_sb     = CreateEntity();
    drone_nova   = CreateEntity();
    drone_hunter = CreateEntity();
    regular_bullet = CreateEntity();
    sniper_bullet  = CreateEntity();
    plasma_orb         = CreateEntity();
    nova_orb           = CreateEntity();

    // Initialisation des entités
    InitEntity(player, 128, 128, 0, 100, 5, 1, 8, 10, 10, 90, 100);
    InitEntity(regular_bullet, 16, 16, 0, 1, 0, 0, 20, 0, 0, 0, 0);
    InitEntity(sniper_bullet, 16, 16, 0, 1, 0, 0, 30, 0, 0, 0, 0);
    InitEntity(plasma_orb, 16, 16, 0, 1, 0, 0, 20, 0, 0, 0, 0);
    InitEntity(nova_orb, 24, 24, 0, 1, 0, 0, 15, 0, 0, 0, 0);
    nova_orb->elementary = 1;

    // Affecter les packs de textures et de sons aux entités en utilisant les ressources chargées
    AssignTexturePackToEntity(player, res.player_drone_frames);
    AssignTexturePackToEntity(drone, res.drone_frames);
    AssignTexturePackToEntity(drone_sb, res.drone_frames);
    AssignTexturePackToEntity(drone_nova, res.drone_frames);
    AssignTexturePackToEntity(drone_hunter, res.drone_frames);
    AssignTexturePackToEntity(regular_bullet, res.regular_bullet_frames);
    AssignTexturePackToEntity(plasma_orb, res.plasma_orb_frames);
    AssignTexturePackToEntity(nova_orb, res.nova_orb_frames);
    AssignTexturePackToEntity(sniper_bullet, res.sniper_bullet_frames);

    // Affecter des marques et des sons spécifiques
    drone_sb->mark    = res.explosive_mark;
    drone_nova->mark  = res.nova_mark;
    drone_hunter->mark = res.hunter_mark;
    AssignSoundPackToEntity(drone, res.magnum_pistol_sounds);
    AssignSoundPackToEntity(player, res.plasma_pistol_sounds);
    AssignSoundPackToEntity(drone_nova, res.grenade_launcher_sounds);
    AssignSoundPackToEntity(drone_hunter, res.sniper_rifle_sounds);

    game_state = STATE_MENU;
    game_running = SDL_TRUE;

    // Boucle principale de la machine à états
    while (game_running) {
        const GameStateFunctions *currentState = &state_funcs[game_state];
        if (currentState->init)
            currentState->init();
        while (game_running && !currentState->shouldTransition()) {
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
                game_state = game_restart ? STATE_MENU : STATE_GAMEOVER;
                break;
        }
    }

    // Nettoyage final
    FreeResourcePack(&res);
    ClearHUD();
    ClearGlyphs();

    DestroySolid(cursor);
    DestroySolid(easy_button);
    DestroySolid(medium_button);
    DestroySolid(hard_button);
    DestroySolid(harder_button);
    DestroySolid(insane_button);
    DestroySolid(restart_button);
    DestroySolid(game_over_title);
    DestroySolid(pause_title);
    DestroySolid(player_info.title);

    DestroyEntityList(ennemies);
    DestroyEntity(player);
    DestroyEntity(drone);
    DestroyEntity(drone_sb);
    DestroyEntity(drone_nova);
    DestroyEntity(drone_hunter);
    DestroyEntity(regular_bullet);
    DestroyEntity(sniper_bullet);
    DestroyEntity(plasma_orb);
    DestroyEntity(nova_orb);
    QuitEngine();
    return 0;
}

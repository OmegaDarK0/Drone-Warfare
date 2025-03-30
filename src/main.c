#include <stdio.h>
#include <time.h>
#include "constants.h"
#include "engine.h"
#include "entity.h"
#include "entity_list.h"
#include "globals.h"
#include "hud.h"
#include "resource.h"
#include "utils.h"
//#include "debug.h"

int main(int argc, char *argv[]){
    // TIME
    /*const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    Uint32 start_time = SDL_GetTicks();
    int frame_count = 0;
    double fps = 0.0;*/
    // STATEMENT
    _Bool restart;
    _Bool easter_egg = 0;
    char player_name[9];
    int difficulty = 0;
    SDL_Event event;
    SDL_bool key_pause = SDL_FALSE;
    SDL_bool key_up = SDL_FALSE;
    SDL_bool key_down = SDL_FALSE;
    SDL_bool key_left = SDL_FALSE;
    SDL_bool key_right = SDL_FALSE;
    SDL_bool key_mouse = SDL_FALSE;
    SDL_bool text_input;
    // STATEMENT TEXTURES
    SDL_Texture *background;
    SDL_Texture *level1;
    SDL_Texture *level2;
    SDL_Texture *level3;
    SDL_Texture *level4;
    SDL_Texture *level0;
    SDL_Texture *sb_mark;
    SDL_Texture *nova_mark;
    SDL_Texture *hunter_mark;
    TexturePack *black_drone_frames;
    TexturePack *silver_drone_frames;
    TexturePack *gold_drone_frames;
    TexturePack *player_drone_frames;
    TexturePack *regular_bullet_frames;
    TexturePack *sniper_bullet_frames;
    TexturePack *plasma_frames;
    TexturePack *nova_frames;
    // STATEMENT SOUNDS
    //SoundPack *assault_rifle_sounds;
    SoundPack *magnum_pistol_sounds;
    //SoundPack *marksman_rifle_sounds;
    SoundPack *plasma_pistol_sounds;
    //SoundPack *plasma_rifle_sounds;
    //SoundPack *plasma_repeater_sounds;
    SoundPack *grenade_launcher_sounds;
    SoundPack *sniper_rifle_sounds;
    // STATEMENT ENTITY
    Entity *player;
    Entity *drone;
    Entity *drone_sb;
    Entity *drone_nova;
    Entity *drone_hunter;
    Entity *regular_bullet;
    Entity *sniper_bullet;
    Entity *plasma;
    Entity *nova;
    EntityList *ennemies;
    EntityNode *current_entity;
    EntityNode *next_entity;
    EntityNode *current_projectile;
    EntityNode *next_projectile;
    // INIT
    srand(time(NULL));
    InitEngine();
    //InitDebug();
    InitHUD();
    double treshold = HEIGHT - scale_height(128);
    // CREATE AND LOAD TEXTURES
    sb_mark = LoadTexture("sb.png");
    nova_mark = LoadTexture("nova.png");
    hunter_mark = LoadTexture("hunter.png");
    debug_hitbox = LoadTexture("debug.png");
    level1 = LoadTexture("level1.png");
    level2 = LoadTexture("level2.png");
    level3 = LoadTexture("level3.png");
    level4 = LoadTexture("level4.png");
    level0 = LoadTexture(".Naval_ensign_of_the_Empire_of_Japan.png");
    background = NULL;
    hitpoints_info = CreateSolidText(rect(0, 5, 1, 1, 0), WHITE, "HP", font_large, WHITE);
    score_info = CreateSolidText(rect(0, HEIGHT - HEIGHT * 45 / 1080, 1, 1, 0), WHITE, "SCORE: 0", font_large, WHITE);
    black_drone_frames = CreateTexturePackEXT("drone", 4, 0, RED);
    silver_drone_frames = CreateTexturePackEXT("silver_drone", 4, 0, RED);
    gold_drone_frames = CreateTexturePackEXT("gold_drone", 4, 0, RED);
    player_drone_frames = CreateTexturePackEXT("silver_drone", 4, 0, BLUE);
    regular_bullet_frames = CreateTexturePack("regular_bullet", 9, 1, VOID);
    sniper_bullet_frames = CreateTexturePack("sniper_bullet", 9, 1, VOID);
    plasma_frames = CreateTexturePack("plasma_orb", 9, 1, VOID);
    nova_frames = CreateTexturePack("nova", 4, 0, VOID);
    // CREATE AND LOAD SOUNDS
    //assault_rifle_sounds = CreateSoundPack("ar_reach", 3);
    magnum_pistol_sounds = CreateSoundPack("magnum", 4);
    //marksman_rifle_sounds = CreateSoundPack("marksman_rifle_", 3);
    plasma_pistol_sounds = CreateSoundPack("plas_pistol_fire_reach", 3);
    //plasma_rifle_sounds = CreateSoundPack("plas_rif_fire_reach", 3);
    //plasma_repeater_sounds = CreateSoundPack("plas_repeater_fire", 3);
    grenade_launcher_sounds = CreateSoundPack("grenade_launcher_fire", 10);
    sniper_rifle_sounds = CreateSoundPack("sniper_distant", 4);
    // MUSIC
    Mix_Music *current_music = NULL;
    //Mix_Music *current_music_intro = NULL;
    /*Mix_Music *level1_music_intro = LoadMusic("1-04. Money or Meteors (Arcadia Intro Version).mp3");
    Mix_Music *level2_music_intro = LoadMusic("1-09. Bad Here Day (Shield Interior Intro Version).mp3");
    Mix_Music *level3_music_intro = LoadMusic("1-05. Best Guess at Best (Shield World Intro Version).mp3");
    Mix_Music *level4_music_intro = LoadMusic("1-07. Fingerprints are Broken (Harvest Intro Version).mp3");*/
    Mix_Music *level1_music = LoadMusic("05 Flollo.mp3");
    Mix_Music *level2_music = LoadMusic("17 Best Guess At Best.mp3");
    Mix_Music *level3_music = LoadMusic("10 Six-Armed Robbing Suit.mp3");
    Mix_Music *level4_music = LoadMusic("11 Action Figure Hands.mp3");
    Mix_Music *level0_music = LoadMusic(".Battotai - Imperial Japanese Army March.mp3");
    // SOLIDS
    Solid *score_title;
    Solid *cursor = CreateSolid(rect(0, 0, 32, 32, 1), VOID, "crosshair.png");
    Solid *button1 = CreateSolidText(rect(0, HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(0, 0, 200, 255), "EASY", font_large, WHITE);
    Solid *button2 = CreateSolidText(rect(0, 2 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(0, 200, 0, 255), "MEDIUM", font_large, WHITE);
    Solid *button3 = CreateSolidText(rect(0, 3 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(200, 200, 0, 255), "HARD", font_large, WHITE);
    Solid *button4 = CreateSolidText(rect(0, 4 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(200, 0, 0, 255), "HARDER", font_large, WHITE);
    Solid *button5 = CreateSolidText(rect(0, 5 * HEIGHT / 6, WIDTH / 5, HEIGHT / 9, 0), color(200, 0, 200, 255), "INSANE", font_large, WHITE);
    Solid *restart_button = CreateSolidText(rect(0, 2 * HEIGHT / 3, WIDTH / 5, HEIGHT / 9, 0), color(100, 100, 100, 255), "RESTART", font_large, WHITE);
    Solid *game_over_title = CreateSolidText(rect(0, HEIGHT / 4, 1, 1, 0), VOID, "GAME OVER", font_xxl, WHITE);
    Solid *pause_title = CreateSolidText(rect(0, 0, 1, 1, 0), VOID, "PAUSE", font_xxl, WHITE);
    Solid *player_name_title = CreateSolidText(rect(0, 11 * HEIGHT / 20, 1, 1, 0), VOID, "ENTER YOUR NAME", font_large, WHITE);
    centered_abs(&button1->rect);
    centered_abs(&button2->rect);
    centered_abs(&button3->rect);
    centered_abs(&button4->rect);
    centered_abs(&button5->rect);
    centered_abs(&restart_button->rect);
    centered_abs(&game_over_title->rect);
    centered_coord(&pause_title->rect);
    centered_abs(&player_name_title->rect);
    // CREATE ENTITIES
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
    // INIT ENTITIES
    //InitEntity(drone, 128, 128, 0, 15, 5, 1, 5, 80, 800, 80, 100);
    //InitEntity(drone_sb, 128, 128, 1, 25, 0, 1.5, 7.5, 0, 0, 0, 80);
    //InitEntity(drone_nova, 128, 128, 2, 35, 10, 1, 2.5, 80, 800, 75, 100);
    InitEntity(player, 128, 128, 0, 100, 5, 1, 8, 10, 10, 90, 100);
    InitEntity(regular_bullet, 16, 16, 0, 1, 0, 0, 20, 0, 0, 0, 0);
    InitEntity(sniper_bullet, 16, 16, 0, 1, 0, 0, 30, 0, 0, 0, 0);
    InitEntity(plasma, 16, 16, 0, 1, 0, 0, 20, 0, 0, 0, 0);
    InitEntity(nova, 24, 24, 0, 1, 0, 0, 15, 0, 0, 0, 0);
    nova->elementary = 1;
    // ASSIGN TEXTURES TO ENTITY (TMP)
    AssignTexturePackToEntity(player, player_drone_frames);
    AssignTexturePackToEntity(drone, black_drone_frames);
    AssignTexturePackToEntity(drone_sb, black_drone_frames);
    AssignTexturePackToEntity(drone_nova, black_drone_frames);
    AssignTexturePackToEntity(drone_hunter, black_drone_frames);
    AssignTexturePackToEntity(regular_bullet, regular_bullet_frames);
    AssignTexturePackToEntity(sniper_bullet, sniper_bullet_frames);
    AssignTexturePackToEntity(plasma, plasma_frames);
    AssignTexturePackToEntity(nova, nova_frames);
    drone_sb->mark = sb_mark;
    drone_nova->mark = nova_mark;
    drone_hunter->mark = hunter_mark;
    // ASSSIGN SOUND TO ENTITY (TMP)
    //AssignSoundPackToEntity(player, assault_rifle_sounds);
    AssignSoundPackToEntity(drone, magnum_pistol_sounds);
    AssignSoundPackToEntity(player, plasma_pistol_sounds);
    AssignSoundPackToEntity(drone_nova, grenade_launcher_sounds);
    AssignSoundPackToEntity(drone_hunter, sniper_rifle_sounds);
    // MENU
    const SDL_Rect box1 = {WIDTH / 5, HEIGHT / 5, WIDTH / 5, HEIGHT / 5},
    box2 = {3 * WIDTH / 5, HEIGHT / 5, WIDTH / 5, HEIGHT / 5},
    box3 = {WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, HEIGHT / 5},
    box4 = {3 * WIDTH / 5, 3 * HEIGHT / 5, WIDTH / 5, HEIGHT / 5};
    while (running) {
        player_name[0] = '\0';
        summon_delay = TimeToFrames(3);
        background = NULL;
        difficulty = 0;
        restart = 0;
        score = 0;
        entity_count = 0;
        easter_egg = 0;
        key_pause = SDL_FALSE;
        key_up = SDL_FALSE;
        key_down = SDL_FALSE;
        key_left = SDL_FALSE;
        key_right = SDL_FALSE;
        key_mouse = SDL_FALSE;
        player_name_title->texture = LoadTextureFromText(font_large, "ENTER YOUR NAME", WHITE);
        ClearEntityList(ennemies);
        //ClearEntityList(drone->projectiles);
        //ClearEntityList(player->projectiles);
        RenderScore();
        ResetVolumeMusic();
        while (running && !background){
            SDL_GetMouseState(&cursor->rect.x, &cursor->rect.y);
            while (SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT){
                    running = SDL_FALSE;
                    break;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = SDL_FALSE;
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
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
        // DIFFICULTY
        while (running && !difficulty){
            SDL_GetMouseState(&cursor->rect.x, &cursor->rect.y);
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
                        }
                        else if (2 * HEIGHT / 6. <= cursor->rect.y && cursor->rect.y <= 2 * HEIGHT / 6. + HEIGHT / 9.) {
                            InitEntity(drone, 128, 128, 0, 10, 5, 1, 5, 160, 800, 80, 100);
                            InitEntity(drone_sb, 128, 128, 1, 20, 0, 1.5, 7, 0, 0, 0, 95);
                            InitEntity(drone_nova, 141, 141, 2, 30, 10, 1, 2.5, 80, 800, 75, 100);
                            InitEntity(drone_hunter, 128, 128, 3, 55, 10, 1.25, 6.25, 100, 1000, 100, 100);
                            summon_delay_max = TimeToFrames(5);
                            difficulty = 2;
                        }
                        else if (3 * HEIGHT / 6. <= cursor->rect.y && cursor->rect.y <= 3 * HEIGHT / 6. + HEIGHT / 9.) {
                            InitEntity(drone, 128, 128, 0, 15, 10, 1.25, 6, 120, 600, 90, 100);
                            InitEntity(drone_sb, 128, 128, 1, 25, 0, 2, 9, 0, 0, 0, 90);
                            InitEntity(drone_nova, 141, 141, 2, 35, 15, 1.25, 3, 60, 600, 80, 100);
                            InitEntity(drone_hunter, 128, 128, 3, 65, 20, 1.5, 7.5, 75, 750, 100, 100);
                            summon_delay_max = TimeToFrames(4);
                            difficulty = 3;
                        }
                        else if (4 * HEIGHT / 6. <= cursor->rect.y && cursor->rect.y <= 4 * HEIGHT / 6. + HEIGHT / 9.) {
                            InitEntity(drone, 128, 128, 0, 20, 15, 1.5, 7, 80, 400, 95, 100);
                            InitEntity(drone_sb, 128, 128, 1, 30, 0, 2.5, 11, 0, 0, 0, 75);
                            InitEntity(drone_nova, 141, 141, 2, 40, 20, 1.5, 3.5, 40, 400, 85, 100);
                            InitEntity(drone_hunter, 128, 128, 3, 80, 30, 1.75, 8.75, 50, 500, 100, 100);
                            summon_delay_max = TimeToFrames(3);
                            difficulty = 4;
                        }
                        else if (5 * HEIGHT / 6. <= cursor->rect.y && cursor->rect.y <= 5 * HEIGHT / 6. + HEIGHT / 9.) {
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
            ClearScreen();
            DrawBackground(background);
            DrawButton(button1, 1, cursor->rect.x, cursor->rect.y, 6);
            DrawButton(button2, 1, cursor->rect.x, cursor->rect.y, 5);
            DrawButton(button3, 1, cursor->rect.x, cursor->rect.y, 1);
            DrawButton(button4, 1, cursor->rect.x, cursor->rect.y, 3);
            DrawButton(button5, 1, cursor->rect.x, cursor->rect.y, 2);
            RefreshScreen();
        }
        // GAME STARTED
        PlayMusic(current_music, -1);
        SDL_ShowCursor(SDL_DISABLE);
        SummonEntity(player, 0, (float)(WIDTH / 2.), (float)(HEIGHT / 2.));
        player->speed = 8;
        drone_hunter->aim_delay = 60;
        while (running && player->hitpoints > 0){
            //Uint32 frame_start_time = SDL_GetTicks();
            SDL_GetMouseState(&cursor->rect.x, &cursor->rect.y);
            /*if (!Mix_PlayingMusic())
                PlayMusic(current_music, 0);*/
            while (SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT){
                    running = SDL_FALSE;
                    break;
                }
                if (event.type == SDL_KEYDOWN){
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = SDL_FALSE;
                    else if (event.key.keysym.scancode == SDL_SCANCODE_TAB) debug = !debug;
                    else if (event.key.keysym.scancode == SDL_SCANCODE_PAUSE){
                        if (!key_pause){
                            DrawRect(SCREEN, 0, 0, 0, 128);
                            DrawText(pause_title, 1);
                            RefreshScreen();
                        }
                        key_pause = !key_pause;
                        SDL_ShowCursor(key_pause);
                    }
                    else switch (event.key.keysym.sym){
                        case SDLK_z:
                        case SDLK_UP:
                            key_up = SDL_TRUE;
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            key_down = SDL_TRUE;
                            break;
                        case SDLK_q:
                        case SDLK_LEFT:
                            key_left = SDL_TRUE;
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:
                            key_right = SDL_TRUE;
                            break;
                        default:
                            break;
                    }
                }
                if (event.type == SDL_KEYUP){
                    switch (event.key.keysym.sym){
                        case SDLK_z:
                        case SDLK_UP:
                            key_up = SDL_FALSE;
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            key_down = SDL_FALSE;
                            break;
                        case SDLK_q:
                        case SDLK_LEFT:
                            key_left = SDL_FALSE;
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:
                            key_right = SDL_FALSE;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) key_mouse = SDL_TRUE;
                if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) key_mouse = SDL_FALSE;
            }
            //Uint32 current_time = SDL_GetTicks();
            if (!key_pause) {
                // BEGIN UI
                ClearScreen();
                DrawBackground(background);
                SetPosition(&cursor->rect, cursor->rect.x, cursor->rect.y);
                // COMMANDS
                if (key_left == key_right) player->velocity_x = 0;
                else if (key_left) player->velocity_x = SetVelocityX(-1);
                else if (key_right) player->velocity_x = SetVelocityX(1);
                if (key_up == key_down) player->velocity_y = 0;
                else if (key_up) player->velocity_y = SetVelocityY(-1);
                else if (key_down) player->velocity_y = SetVelocityY(1);
                if (key_mouse) EntityShoot(player, player->projectiles, plasma, (float)cursor->rect.x, (float)cursor->rect.y, 0, 0);
                // SUMMON ENTITIES
                summon_delay--;
                if (!summon_delay){
                    summon_delay = randint(summon_delay_max / 10, summon_delay_max);
                    summon_delay_max--;
                    Entity *summoned = CreateEntity(); // COULD BE A MEMORY LEAK
                    const int rand_category = randint(0, 49);
                    int lifetime;
                    if (easter_egg || rand_category < 15){
                        CloneEntity(summoned, drone_sb);
                        lifetime = -1;
                    }
                    else if (rand_category < 20) {
                        CloneEntity(summoned, drone_nova);
                        lifetime = TimeToFrames(15);
                    }
                    else if (rand_category == 20) {
                        CloneEntity(summoned, drone_hunter);
                        lifetime = -1;
                    }
                    else{
                        CloneEntity(summoned, drone);
                        lifetime = TimeToFrames(10);
                    }
                    // SILVER AND GOLD DRONES
                    const int rand_prestige = randint(0, 10000);
                    if (rand_prestige < 100){
                        summoned->prestige = 1;
                        AssignTexturePackToEntity(summoned, silver_drone_frames);
                    }
                    else if (rand_prestige == 100) {
                        summoned->prestige = 2;
                        AssignTexturePackToEntity(summoned, gold_drone_frames);
                    }
                    const int rand_side = randint(0, 1);
                    if (!rand_side){
                        SummonEntity(summoned, lifetime, -drone->hitbox.w, (float)randint(0, HEIGHT - (int)drone->hitbox.h));
                        summoned->velocity_x = SetVelocityX(1);
                    }
                    else{
                        SummonEntity(summoned, lifetime, (float)WIDTH + drone->hitbox.w, (float)randint(0, HEIGHT - (int)drone->hitbox.h));
                        summoned->velocity_x = SetVelocityX(-1);
                    }
                    AddEntity(ennemies, summoned);
                }
                // ENTITIES
                current_entity = ennemies->head;
                while (current_entity){
                    next_entity = current_entity->next;
                    if (IsEntityAIEnabled(current_entity->entity)){
                        if (IsEntityAlive(current_entity->entity)){
                            //MoveEntity(current_entity->entity, 1);
                            DisplayEntity(current_entity->entity);
                            UpdateEntityState(current_entity->entity);
                            switch (current_entity->entity->category) {
                                case 0:
                                    EntityShoot(current_entity->entity, drone->projectiles, regular_bullet, player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                                    MoveEntity(current_entity->entity, 1);
                                    break;
                                case 1:
                                    EntityIntercept(current_entity->entity,player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                                    MoveEntity(current_entity->entity, 1);
                                    break;
                                case 2:
                                    EntityShoot(current_entity->entity, drone->projectiles, nova, player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                                    MoveEntity(current_entity->entity, 1);
                                    break;
                                case 3:
                                    if (AreEntitiesNear(current_entity->entity, player, treshold)) {
                                        EntityAim(current_entity->entity, player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                                        EntityShoot(current_entity->entity, drone->projectiles, sniper_bullet, player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                                    }
                                    else {
                                        EntityIntercept(current_entity->entity,player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                                        MoveEntity(current_entity->entity, 1);
                                    }
                                    break;
                                default:
                                    MoveEntity(current_entity->entity, 1);
                            }
                            /*
                            if (current_entity->entity->category == 0) EntityShoot(current_entity->entity, drone->projectiles, bullet, player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                            else if (current_entity->entity->category == 1) EntityIntercept(current_entity->entity,player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                            else if (current_entity->entity->category == 2) EntityShoot(current_entity->entity, drone->projectiles, nova, player->hitbox.x, player->hitbox.y, player->hitbox.w, player->hitbox.h);
                            */
                        }
                        else{
                            MoveEntity(current_entity->entity, 1);
                            DisplayEntity(current_entity->entity);
                            UpdateEntityState(current_entity->entity);
                            current_entity->entity->speed *= 1.005;
                        }
                    }
                    else RemoveEntity(ennemies, current_entity->entity);
                    current_entity = next_entity;
                }
                // ENTITIES PROJECTILES
                current_projectile = drone->projectiles->head;
                while (current_projectile){
                    next_projectile = current_projectile->next;
                    if (IsEntityAIEnabled(current_projectile->entity)) {
                        MoveEntity(current_projectile->entity, 1);
                        DisplayEntity(current_projectile->entity);
                        UpdateEntityState(current_projectile->entity);
                    }
                    else RemoveEntity(drone->projectiles, current_projectile->entity);
                    current_projectile = next_projectile;
                }
                /*current_projectile = drone_nova->projectiles->head;
                while (current_projectile){
                    next_projectile = current_projectile->next;
                    if (IsEntityAIEnabled(current_projectile->entity)) {
                        MoveEntity(current_projectile->entity, 1);
                        RenderEntity(current_projectile->entity);
                        UpdateEntityState(current_projectile->entity);
                    }
                    else RemoveEntity(drone->projectiles, current_projectile->entity);
                    current_projectile = next_projectile;
                }*/
                // PLAYER
                //MoveEntityAndCursor(player, 0, cursor->rect.x, cursor->rect.y);
                MoveEntity(player, 0);
                DisplayEntity(player);
                UpdateEntityState(player);
                // PLAYER PROJECTILES
                current_projectile = player->projectiles->head;
                while (current_projectile){
                    next_projectile = current_projectile->next;
                    if (IsEntityAIEnabled(current_projectile->entity)) {
                        MoveEntity(current_projectile->entity, 1);
                        DisplayEntity(current_projectile->entity);
                        UpdateEntityState(current_projectile->entity);
                    }
                    else RemoveEntity(player->projectiles, current_projectile->entity);
                    current_projectile = next_projectile;
                }
                // COLLISION BETWEEN ENNEMY AND PLAYER
                current_entity = ennemies->head;
                while (current_entity){
                    next_entity = current_entity->next;
                    if (IsEntityAlive(current_entity->entity)){
                        HandleCollision(&current_entity->entity, &player);
                        // COLLISION BETWEEN ENNEMY AND PLAYER PROJECTILE
                        current_projectile = player->projectiles->head;
                        while (current_projectile){
                            next_projectile = current_projectile->next;
                            HandleCollision(&current_entity->entity, &current_projectile->entity);
                            if (IsEntityDead(current_projectile->entity))
                                DisableEntityAI(current_projectile->entity);
                            current_projectile = next_projectile;
                        }
                        // END COLLISION
                        if (IsEntityDead(current_entity->entity)) {
                            //AssignTexturePackToEntity(current_entity->entity, dead_black_drone_frames);
                            current_entity->entity->AI_lifetime = TimeToFrames(5);
                            current_entity->entity->velocity_y = SetVelocityY(1);
                            current_entity->entity->speed = 5;
                            GainScore(current_entity->entity, difficulty);
                        }
                    }
                    current_entity = next_entity;
                }
                // END COLLISION
                // COLLISION BETWEEN PLAYER AND ENNEMY PROJECTILE
                current_projectile = drone->projectiles->head;
                while (current_projectile){
                    next_projectile = current_projectile->next;
                    HandleCollision(&current_projectile->entity, &player);
                    if (IsEntityDead(current_projectile->entity))
                        DisableEntityAI(current_projectile->entity);
                    current_projectile = next_projectile;
                }
                /*current_projectile = drone_nova->projectiles->head;
                while (current_projectile){
                    next_projectile = current_projectile->next;
                    HandleCollision(&current_projectile->entity, &player);
                    if (IsEntityDead(current_projectile->entity))
                        DisableEntityAI(current_projectile->entity);
                    current_projectile = next_projectile;
                }*/
                // END COLLISION
                DisplayHUD(player);
                DisplayEffect(player);
                DisplayCursor(cursor->texture, cursor->rect);
                RefreshScreen();
                //RenderDebug();
                // END UI
            }
            /*frame_count++;
            if (current_time - start_time >= 1000) { // Update FPS every second
                fps = frame_count / ((current_time - start_time) / 1000.0);
                printf("FPS: %.2f\n", fps);
                frame_count = 0;
                start_time = current_time;
            }
            Uint32 frame_end_time = SDL_GetTicks();
            int frame_time = frame_end_time - frame_start_time;
            if (frame_time < FRAME_DELAY) {
                SDL_Delay(FRAME_DELAY - frame_time); // Attendre pour limiter le FPS
            }*/
        }
        // GAME OVER
        //AssignTexturePackToEntity(player, dead_silver_drone_frames);
       if (running) {
           EnableEntityAI(player, TimeToFrames(4));
           player->velocity_y = SetVelocityY(1);
           player->speed = 5;
           player->effect = 0;
       }
        while (running && IsEntityAIEnabled(player)) {
            while (SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT){
                    running = SDL_FALSE;
                    break;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = SDL_FALSE;
            }
            ClearScreen();
            DrawBackground(background);
            current_entity = ennemies->head;
            while (current_entity){
                next_entity = current_entity->next;
                DisplayEntity(current_entity->entity);
                UpdateEntityState(current_entity->entity);
                if (IsEntityDead(current_entity->entity)) MoveEntity(current_entity->entity, 1);
                current_entity = next_entity;
            }
            DecreaseVolumeMusic(1);
            DrawRect(SCREEN, 0, 0, 0, 255 - player->AI_lifetime);
            //player->speed *= 1.001;
            MoveEntity(player, 1);
            DisplayEntity(player);
            UpdateEntityState(player);
            RefreshScreen();
        }
        char score_str[16];
        sprintf(score_str, "SCORE: %d", score);
        score_title = CreateSolidText(rect(0, 4 * HEIGHT / 10, 1, 1, 0), VOID, score_str, font_large, WHITE);
        centered_abs(&score_title->rect);
        SDL_ShowCursor(SDL_ENABLE);
        Mix_HaltMusic();
        SDL_StartTextInput();
        text_input = SDL_TRUE;
        // END
        while (running && !restart) {
            SDL_GetMouseState(&cursor->rect.x, &cursor->rect.y);
            while (SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT){
                    running = SDL_FALSE;
                    break;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = SDL_FALSE;
                    if (text_input) {
                        if (event.key.keysym.sym == SDLK_RETURN) {
                            text_input = SDL_FALSE;
                            SDL_StopTextInput();
                            if (strlen(player_name)) {
                                player_name_title->texture = LoadTextureFromText(font_large, player_name, GREEN);
                                RegisterScore(player_name, difficulty);
                            }
                            else {
                                player_name_title->texture = LoadTextureFromText(font_large, "GUEST", GREEN);
                                RegisterScore("guest", difficulty);
                            }
                        }
                        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(player_name) > 0) {
                            player_name[strlen(player_name) - 1] = '\0';
                            if (strlen(player_name) == 0) player_name_title->texture = LoadTextureFromText(font_large, "ENTER YOUR NAME", WHITE);
                            else player_name_title->texture = LoadTextureFromText(font_large, player_name, WHITE);
                        }
                    }
                }
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
                    if (2 * WIDTH / 5. <= cursor->rect.x && cursor->rect.x <= 3 * WIDTH / 5. &&
                        2 * HEIGHT / 3. <= cursor->rect.y && cursor->rect.y <= 2 * HEIGHT / 3. + HEIGHT / 9.)
                        restart = 1;
                if (text_input && event.type == SDL_TEXTINPUT && strlen(player_name) < 8) {
                    strcat(player_name, event.text.text);
                    player_name_title->texture = LoadTextureFromText(font_large, player_name, WHITE);
                }
            }
            ClearScreen();
            DrawText(game_over_title, 1);
            DrawText(score_title, 1);
            DrawText(player_name_title, 1);
            DrawButton(restart_button, 1, cursor->rect.x, cursor->rect.y, 7);
            RefreshScreen();
        }
        SDL_StopTextInput();
    }
    // UNLOAD MUSIC
    UnloadMusic(level0_music);
    UnloadMusic(level1_music);
    UnloadMusic(level2_music);
    UnloadMusic(level3_music);
    UnloadMusic(level4_music);
    /*UnloadMusic(level1_music_intro);
    UnloadMusic(level2_music_intro);
    UnloadMusic(level3_music_intro);
    UnloadMusic(level4_music_intro);*/
    // UNLOAD TEXTURES
    UnloadTexture(sb_mark);
    UnloadTexture(nova_mark);
    UnloadTexture(hunter_mark);
    UnloadTexture(debug_hitbox);
    UnloadTexture(level0);
    UnloadTexture(level1);
    UnloadTexture(level2);
    UnloadTexture(level3);
    UnloadTexture(level4);
    DestroySolid(hitpoints_info);
    DestroySolid(score_info);
    DestroySolid(player_name_title);
    // DESTROY TEXTURES PACK
    DestroyTexturePack(black_drone_frames);
    DestroyTexturePack(silver_drone_frames);
    DestroyTexturePack(gold_drone_frames);
    DestroyTexturePack(player_drone_frames);
    DestroyTexturePack(regular_bullet_frames);
    DestroyTexturePack(sniper_bullet_frames);
    DestroyTexturePack(plasma_frames);
    DestroyTexturePack(nova_frames);
    // DESTROY SOUNDS PACK
    //DestroySoundPack(assault_rifle_sounds);
    DestroySoundPack(magnum_pistol_sounds);
    //DestroySoundPack(marksman_rifle_sounds);
    DestroySoundPack(plasma_pistol_sounds);
    //DestroySoundPack(plasma_rifle_sounds);
    //DestroySoundPack(plasma_repeater_sounds);
    DestroySoundPack(grenade_launcher_sounds);
    DestroySoundPack(sniper_rifle_sounds);
    // DESTROY ENTITIES
    DestroyEntity(regular_bullet);
    DestroyEntity(sniper_bullet);
    DestroyEntity(plasma);
    DestroyEntity(nova);
    DestroyEntity(drone);
    DestroyEntity(drone_sb);
    DestroyEntity(drone_nova);
    DestroyEntity(drone_hunter);
    DestroyEntity(player);
    DestroyEntityList(ennemies);
    // DESTROY SOLIDS
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
    // END
    //QuitDebug();
    QuitEngine();
}

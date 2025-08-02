#include "input.h"
#include "constants.h"
#include "globals.h"
#include "graphics.h"
#include "ui.h"

void TogglePause(void) {
    if (!key_pause) {
        DrawRect(SCREEN, 0, 0, 0, 128);
        DrawText(pause_title, 1);
        RefreshScreen();
    }
    key_pause = !key_pause;
    SDL_ShowCursor(key_pause);
}

void HandleKeyboardEvent(SDL_Event *event) {
    SDL_Scancode sc = event->key.keysym.scancode;
    SDL_Keycode key = event->key.keysym.sym;

    if (event->type == SDL_QUIT) {
        game_running = SDL_FALSE;
        return;
    }

    if (event->type == SDL_KEYDOWN) {
        switch (sc) {
        case SDL_SCANCODE_ESCAPE: game_running = SDL_FALSE; break;
        case SDL_SCANCODE_TAB: game_debug = !game_debug; break;
        case SDL_SCANCODE_PAUSE: TogglePause(); break;
        default:
            switch (key) {
        case SDLK_z: case SDLK_UP: key_up = SDL_TRUE; break;
        case SDLK_s: case SDLK_DOWN: key_down = SDL_TRUE; break;
        case SDLK_q: case SDLK_LEFT: key_left = SDL_TRUE; break;
        case SDLK_d: case SDLK_RIGHT: key_right = SDL_TRUE; break;
            }
        }
    }

    if (event->type == SDL_KEYUP) {
        switch (key) {
        case SDLK_z: case SDLK_UP: key_up = SDL_FALSE; break;
        case SDLK_s: case SDLK_DOWN: key_down = SDL_FALSE; break;
        case SDLK_q: case SDLK_LEFT: key_left = SDL_FALSE; break;
        case SDLK_d: case SDLK_RIGHT: key_right = SDL_FALSE; break;
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
        key_mouse = SDL_TRUE;
    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
        key_mouse = SDL_FALSE;
}

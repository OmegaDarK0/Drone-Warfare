#include "ui.h"
#include "images.h"
#include "graphics.h"
#include <stdlib.h>

Solid *CreateSolid(const SDL_Rect rect, const SDL_Color color, const char *str) {
    Solid *solid = malloc(sizeof(Solid));
    solid->texture = LoadTexture(str);
    solid->rect = rect;
    solid->color = color;
    return solid;
}

Solid *CreateSolidText(const SDL_Rect rect, const SDL_Color color, const char *str, const int size, const SDL_Color text_color) {
    Solid *solid = malloc(sizeof(Solid));
    solid->texture = LoadTextureFromText(str, size, text_color);
    solid->rect = rect;
    solid->color = color;
    return solid;
}

void DestroySolid(Solid *solid) {
    if (solid) {
        if (solid->texture)
            UnloadTexture(solid->texture);
        free(solid);
    }
}

void DrawText(const Solid *text, const _Bool centered) {
    int w, h;
    int x = text->rect.x;
    int y = text->rect.y;
    GetTextureSize(text->texture, &w, &h);
    if (centered) {
        x += (text->rect.w - w) / 2;
        y += (text->rect.h - h) / 2;
    }
    DrawTexture(text->texture, (SDL_Rect){x, y, w, h});
}

void DrawButton(const Solid *button, const _Bool centered, const int pos_x, const int pos_y, const unsigned int mode) {
    int w, h;
    int x = button->rect.x;
    int y = button->rect.y;
    int r = button->color.r;
    int g = button->color.g;
    int b = button->color.b;
    const int a = button->color.a;
    if (pos_x >= button->rect.x && pos_x <= button->rect.x + button->rect.w &&
        pos_y >= button->rect.y && pos_y <= button->rect.y + button->rect.h) {
        if (mode >= 4 && mode <= 7)
            r += 100;
        if (mode == 2 || mode == 3 || mode == 6 || mode == 7)
            g += 100;
        if (mode % 2)
            b += 100;
    }
    GetTextureSize(button->texture, &w, &h);
    DrawRect(button->rect, r, g, b, a);
    if (centered) {
        x += (button->rect.w - w) / 2;
        y += (button->rect.h - h) / 2;
    }
    DrawTexture(button->texture, (SDL_Rect){x, y, w, h});
}

void DisplayCursor(SDL_Texture *texture, const SDL_Rect rect) {
    SDL_FRect frect = {(float)rect.x, (float)rect.y, (float)rect.w, (float)rect.h};
    frect.x -= frect.w / 2.0f;
    frect.y -= frect.h / 2.0f;
    DrawTexture(texture, FRectToRect(frect));
}


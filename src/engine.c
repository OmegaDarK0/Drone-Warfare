#include "engine.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include "constants.h"
#include "globals.h"
#include "utils.h"

int QuitEngine(){
    running = SDL_FALSE;
    if (font_jumbo) FT_Done_Face(font_jumbo);
    if (font_xxl) FT_Done_Face(font_xxl);
    if (font_large) FT_Done_Face(font_large);
    if (font_medium) FT_Done_Face(font_medium);
    if (font_small) FT_Done_Face(font_small);
    if (ft_library) FT_Done_FreeType(ft_library);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    SDL_QuitSubSystem(SDL_INIT_TIMER);
    SDL_Quit();
    return -1;
}

int InitEngine() {
    SDL_DisplayMode dm;
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0){
        SDL_Log("SDL_Init: %s\n", SDL_GetError());
        return QuitEngine();
    }
    /*if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") < 0) {
        SDL_Log("SDL_SetHint: %s\n", SDL_GetError());
        return QuitEngine();
    }*/
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        SDL_Log("IMG_Init: %s\n", IMG_GetError());
        return QuitEngine();
    }
    if ((Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC) & (MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC)) != (MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC)) {
        SDL_Log("Mix_Init: %s\n", Mix_GetError());
        return QuitEngine();
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Mix_OpenAudio: %s\n", Mix_GetError());
        return QuitEngine();
    }
    if (Mix_AllocateChannels(32) < 0) {
        SDL_Log("Mix_AllocateChannels: %s\n", Mix_GetError());
        return QuitEngine();
    }
    if (SDL_GetCurrentDisplayMode(0, &dm) < 0) {
        SDL_Log("SDL_GetCurrentDisplayMode: %s\n", SDL_GetError());
        return QuitEngine();
    }
    WIDTH = dm.w;
    HEIGHT = dm.h;
    REFRESH_RATE = dm.refresh_rate;
    SCREEN = rect(0, 0, WIDTH, HEIGHT, 0);
    SDL_Log("WIDTH: %d, HEIGHT: %d, REFRESH RATE: %d", WIDTH, HEIGHT, REFRESH_RATE);
    window = SDL_CreateWindow("Drone Warfare", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN);
    if (!window){
        SDL_Log("SDL_CreateWindow: %s\n", SDL_GetError());
        return QuitEngine();
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer){
        SDL_Log("SDL_CreateRenderer: %s\n", SDL_GetError());
        return QuitEngine();
    }
    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0) {
        SDL_Log("SDL_SetRenderDrawBlendMode: %s\n", SDL_GetError());
        return QuitEngine();
    }

    FT_Error error = FT_Init_FreeType(&ft_library);
    if (error) {
        fprintf(stderr, "FT_Init_FreeType: %d\n", error);
        return QuitEngine();
    }
    error = FT_New_Face(ft_library, "assets/fonts/slkscr.ttf", 0, &font_small);
    if (error) {
        fprintf(stderr, "FT_New_Face: %d\n", error);
        return QuitEngine();
    }
    error = FT_New_Face(ft_library, "assets/fonts/slkscr.ttf", 0, &font_medium);
    if (error) {
        fprintf(stderr, "FT_New_Face: %d\n", error);
        return QuitEngine();
    }
    error = FT_New_Face(ft_library, "assets/fonts/slkscr.ttf", 0, &font_large);
    if (error) {
        fprintf(stderr, "FT_New_Face: %d\n", error);
        return QuitEngine();
    }
    error = FT_New_Face(ft_library, "assets/fonts/slkscr.ttf", 0, &font_xxl);
    if (error) {
        fprintf(stderr, "FT_New_Face: %d\n", error);
        return QuitEngine();
    }
    error = FT_New_Face(ft_library, "assets/fonts/slkscr.ttf", 0, &font_jumbo);
    if (error) {
        fprintf(stderr, "FT_New_Face: %d\n", error);
        return QuitEngine();
    }
    error = FT_Set_Pixel_Sizes(font_small, 0, scale_height(16));
    if (error) {
        fprintf(stderr, "FT_Set_Pixel_Sizes: %d\n", error);
        return QuitEngine();
    }
    error = FT_Set_Pixel_Sizes(font_medium, 0, scale_height(32));
    if (error) {
        fprintf(stderr, "FT_Set_Pixel_Sizes: %d\n", error);
        return QuitEngine();
    }
    error = FT_Set_Pixel_Sizes(font_large, 0, scale_height(64));
    if (error) {
        fprintf(stderr, "FT_Set_Pixel_Sizes: %d\n", error);
        return QuitEngine();
    }
    error = FT_Set_Pixel_Sizes(font_xxl, 0, scale_height(128));
    if (error) {
        fprintf(stderr, "FT_Set_Pixel_Sizes: %d\n", error);
        return QuitEngine();
    }
    error = FT_Set_Pixel_Sizes(font_jumbo, 0, scale_height(256));
    if (error) {
        fprintf(stderr, "FT_Set_Pixel_Sizes: %d\n", error);
        return QuitEngine();
    }
    return 0;
}

SDL_Rect FRectToRect(const SDL_FRect frect){
    const SDL_Rect rect = {(int)roundf(frect.x), (int)roundf(frect.y), (int)frect.w, (int)frect.h};
    return rect;
}

void DrawPoint(const int x, const int y, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) {
        SDL_Log("SDL_SetRenderDrawColor: %s\n", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderDrawPoint(renderer, x, y) < 0) {
        SDL_Log("SDL_RenderDrawPoint: %s\n", SDL_GetError());
        QuitEngine();
    }
}

void DrawLine(const int xa, const int ya, const int xb, const int yb, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0) {
        SDL_Log("SDL_SetRenderDrawColor: %s\n", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderDrawLine(renderer, xa, ya, xb, yb) < 0) {
        SDL_Log("SDL_RenderDrawLine: %s\n", SDL_GetError());
        QuitEngine();
    }
}

void DrawRect(const SDL_Rect rect, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a){
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0){
        SDL_Log("SDL_SetRenderDrawColor: %s\n", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderFillRect(renderer, &rect) < 0) {
        SDL_Log("SDL_RenderFillRect: %s\n", SDL_GetError());
        QuitEngine();
    }
}

void DrawRectF(const SDL_FRect frect, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a){
    if (SDL_SetRenderDrawColor(renderer, r, g, b, a) < 0){
        SDL_Log("SDL_SetRenderDrawColor: %s\n", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderFillRectF(renderer, &frect) < 0) {
        SDL_Log("SDL_RenderFillRectF: %s\n", SDL_GetError());
        QuitEngine();
    }
}

Uint32 ColorToUint32(const SDL_Surface *surface, const SDL_Color color){
    const Uint32 color32 = SDL_MapRGB(surface->format, color.r, color.g, color.b);
    if (color32 == 0){
        SDL_Log("SDL_MapRGB: %s\n", SDL_GetError());
        QuitEngine();
    }
    return color32;
}

void ReplaceColor(const SDL_Surface *surface, const SDL_Color src_color, const SDL_Color dst_color){
    const Uint32 src = ColorToUint32(surface, src_color), dst = ColorToUint32(surface, dst_color);
    Uint32 *pixels = surface->pixels;
    const int pixel_count = surface->pitch / 4 * surface->h;
    for (int i = 0; i < pixel_count; i++)
        if (pixels[i] == src)
            pixels[i] = dst;
}

void GetTextureSize(SDL_Texture* texture, int* width, int* height) {
    if (SDL_QueryTexture(texture, NULL, NULL, width, height) < 0) {
        SDL_Log("SDL_QueryTexture: %s\n", SDL_GetError());
        QuitEngine();
    }
}

void SetPosition(SDL_Rect *rect, const int x, const int y){
    rect->x = x;
    rect->y = y;
}

void SetPositionF(SDL_FRect *frect, const float x, const float y){
    frect->x = x;
    frect->y = y;
}

_Bool Intersecting(const SDL_FRect frect1, const SDL_FRect frect2){
    const SDL_Rect rect1 = FRectToRect(frect1), rect2 = FRectToRect(frect2);
    return SDL_HasIntersection(&rect1, &rect2);
}

void RefreshScreen(){
    SDL_RenderPresent(renderer);
}

void ClearScreen(){
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) < 0){
        SDL_Log("SDL_SetRenderDrawColor: %s\n", SDL_GetError());
        QuitEngine();
    }
    if (SDL_RenderClear(renderer) < 0){
        SDL_Log("SDL_RenderClear: %s\n", SDL_GetError());
        QuitEngine();
    }
}

SDL_Surface *LoadImage(const char *filename){
    char path[256]; //= malloc((int)strlen(filename) + 15);
    sprintf(path, "assets/images/%s", filename);
    SDL_Surface *surface = IMG_Load(path);
    if (!surface){
        SDL_Log("IMG_Load: %s\n", IMG_GetError());
        QuitEngine();
        return NULL;
    }
    return surface;
}

/*SDL_Surface *LoadText(FT_Face face, const char *text, const SDL_Color color){
    int width = 0, height = 0;

    // Calculer les dimensions de la surface à créer
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
            fprintf(stderr, "FT_Load_Char: Could not load character '%c'\n", *p);
            return NULL;
        }
        width += (int)face->glyph->bitmap.width;
        if (face->glyph->bitmap.rows > height) {
            height = (int)face->glyph->bitmap.rows;
        }
    }

    // Créer une surface SDL avec la taille calculée
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) {
        SDL_Log("SDL_CreateRGBSurfaceWithFormat: %s\n", SDL_GetError());
        return NULL;
    }

    // Remplir la surface avec des glyphs rendus
    int x = 0;
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
            SDL_Log("FT_Load_Char: Could not load character '%c'\n", *p);
            SDL_FreeSurface(surface);
            return NULL;
        }

        // Récupérer le glyphe rendu
        FT_Bitmap* bitmap = &face->glyph->bitmap;

        // Copier le glyphe dans la surface SDL
        for (int i = 0; i < bitmap->rows; i++) {
            for (int j = 0; j < bitmap->width; j++) {
                const int dst_index = (i * surface->pitch / 4) + (x + j);
                ((Uint32*)surface->pixels)[dst_index] = SDL_MapRGBA(surface->format, color.r, color.g, color.b, bitmap->buffer[i * bitmap->width + j]);
            }
        }
        x += (int)bitmap->width;
    }
    return surface;
}*/

SDL_Surface *LoadText(FT_Face font, const char *text, const SDL_Color color) {
    if (!font || !text) {
        fprintf(stderr, "Font or text is NULL\n");
        return NULL;
    }

    int width = 0, height = 0;
    int max_height = 0;

    // Calculer les dimensions de la surface à créer
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(font, *p, FT_LOAD_RENDER)) {
            fprintf(stderr, "FT_Load_Char: Could not load character '%c'\n", *p);
            return NULL;
        }
        width += (int)font->glyph->advance.x >> 6; // Utiliser l'avance en x pour la largeur de chaque caractère
        if (font->glyph->bitmap.rows > max_height) {
            max_height = (int)font->glyph->bitmap.rows;
        }
    }

    height = max_height;

    // Créer une surface SDL avec la taille calculée
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) {
        SDL_Log("SDL_CreateRGBSurfaceWithFormat: %s\n", SDL_GetError());
        return NULL;
    }

    // Remplir la surface avec des glyphs rendus
    int x = 0;
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(font, *p, FT_LOAD_RENDER)) {
            SDL_Log("FT_Load_Char: Could not load character '%c'\n", *p);
            SDL_FreeSurface(surface);
            return NULL;
        }

        // Récupérer le glyphe rendu
        FT_Bitmap* bitmap = &font->glyph->bitmap;
        FT_Int x_offset = font->glyph->bitmap_left;
        FT_Int y_offset = height - font->glyph->bitmap_top;

        // Copier le glyphe dans la surface SDL
        for (int i = 0; i < bitmap->rows; i++) {
            for (int j = 0; j < bitmap->width; j++) {
                const int dst_index = ((i + y_offset) * surface->pitch / 4) + (x + j + x_offset);
                ((Uint32*)surface->pixels)[dst_index] = SDL_MapRGBA(surface->format, color.r, color.g, color.b, bitmap->buffer[i * bitmap->pitch + j]);
            }
        }

        x += (int)font->glyph->advance.x >> 6;
    }

    return surface;
}

SDL_Texture *LoadTexture(const char *filename){
    char path[256]; //= malloc((int)strlen(filename) + 15);
    sprintf(path, "assets/images/%s", filename);
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture){
        SDL_Log("IMG_LoadTexture: %s\n", IMG_GetError());
        QuitEngine();
        return NULL;
    }
    return texture;
}

SDL_Texture *LoadTextureFromImage(SDL_Surface *surface){
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture){
        SDL_Log("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        QuitEngine();
        return NULL;
    }
    return texture;
}

SDL_Texture *LoadTextureFromText(FT_Face font, const char *text, const SDL_Color color){
    SDL_Surface *surface = LoadText(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    UnloadImage(surface);
    if (!texture){
        SDL_Log("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        QuitEngine();
        return NULL;
    }
    return texture;
}

Mix_Chunk *LoadSound(const char *filename) {
    char path[256]; //malloc((int)strlen(filename) + 15);
    sprintf(path, "assets/sounds/%s", filename);
    Mix_Chunk *chunk = Mix_LoadWAV(path);
    if (!chunk) {
        SDL_Log("Mix_LoadWAV: %s\n", Mix_GetError());
        QuitEngine();
    }
    return chunk;
}

Mix_Music *LoadMusic(const char *filename) {
    char path[256]; //= malloc((int)strlen(filename) + 15);
    sprintf(path, "assets/musics/%s", filename);
    Mix_Music *music = Mix_LoadMUS(path);
    if (!music) {
        SDL_Log("Mix_LoadMUS: %s\n", Mix_GetError());
        QuitEngine();
    }
    return music;
}

void PlaySound(Mix_Chunk *sound) {
    if (sound){
        if (Mix_PlayChannel(-1, sound, 0) < 0) {
            SDL_Log("Mix_PlayChannel: %s\n", Mix_GetError());
            QuitEngine();
        }
    }
}

void ChangeVolumeSound(Mix_Chunk *sound, const int volume){
    if (Mix_VolumeChunk(sound, volume) < 0){
        SDL_Log("Mix_VolumeChunk: %s\n", Mix_GetError());
        QuitEngine();
    }
}

void IncreaseVolumeSound(Mix_Chunk *sound, const int volume) {
    ChangeVolumeSound(sound, Mix_VolumeChunk(sound, -1) + volume);
}

void DecreaseVolumeSound(Mix_Chunk *sound, const int volume) {
    ChangeVolumeSound(sound, Mix_VolumeChunk(sound, -1) - volume);
}

void PlayMusic(Mix_Music *music, const int loops) {
    if (music){
        if (Mix_PlayMusic(music, loops) < 0) {
            SDL_Log("Mix_PlayMusic: %s\n", Mix_GetError());
            QuitEngine();
        }
    }
}

void ChangeVolumeMusic(const int volume) {
    if (Mix_VolumeMusic(volume) < 0){
        SDL_Log("Mix_VolumeMusic: %s\n", Mix_GetError());
        QuitEngine();
    }
}

void IncreaseVolumeMusic(const int volume) {
    ChangeVolumeMusic(Mix_VolumeMusic(-1) + volume);
}

void DecreaseVolumeMusic(const int volume) {
    ChangeVolumeMusic(Mix_VolumeMusic(-1) - volume);
}

void ResetVolumeMusic() {
    ChangeVolumeMusic(128);
}

void UnloadImage(SDL_Surface *surface){
    if (surface) SDL_FreeSurface(surface);
}

void UnloadTexture(SDL_Texture *texture){
    if (texture) SDL_DestroyTexture(texture);
}

void UnloadSound(Mix_Chunk *chunk){
    if (chunk) Mix_FreeChunk(chunk);
}

void UnloadMusic(Mix_Music *music) {
    if (music) Mix_FreeMusic(music);
}

void DrawTexture(SDL_Texture *texture, const SDL_Rect rect){
    if (texture){
        if (SDL_RenderCopy(renderer, texture, NULL, &rect) < 0){
            SDL_Log("SDL_RenderCopy: %s\n", SDL_GetError());
            QuitEngine();
        }
    }
    else DrawRect(rect, 255, 255, 255, 255);
}

void DrawBackground(SDL_Texture *texture){
    if (texture){
        if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0){
            SDL_Log("SDL_RenderCopy: %s\n", SDL_GetError());
            QuitEngine();
        }
    }
}

SDL_Surface* CreateSurfaceFromTexture(SDL_Renderer* renderer, SDL_Texture* texture) {
    // Obtenir les dimensions de la texture
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    // Créer une surface avec le même format que la texture
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface) {
        fprintf(stderr, "Failed to create surface: %s\n", SDL_GetError());
        return NULL;
    }

    // Copier les pixels de la texture dans la surface
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    SDL_SetRenderTarget(renderer, NULL);

    return surface;
}

void HighlightFilter(const SDL_Surface* surface, const int alpha){
    Uint32 *pixels = surface->pixels;
    const int pixel_count = surface->pitch / 4 * surface->h;
    for (int i = 0; i < pixel_count; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], surface->format, &r, &g, &b, &a);
        //r = color.r;
        //g = color.g;
        //b = color.b;
        if (a) a = alpha;
        pixels[i] = SDL_MapRGBA(surface->format, r, g, b, a);
    }
}

Solid *CreateSolid(const SDL_Rect rect, const SDL_Color color, const char *str) {
    Solid *solid = malloc(sizeof(Solid));
    solid->texture = LoadTexture(str);
    solid->rect = rect;
    solid->color = color;
    return solid;
}

Solid *CreateSolidText(const SDL_Rect rect, const SDL_Color color, const char *str, FT_Face font, const SDL_Color text_color) {
    Solid *solid = malloc(sizeof(Solid));
    solid->texture = LoadTextureFromText(font, str, text_color);
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

void DrawText(const Solid *text, const _Bool centered){
    int w, h;
    int x = text->rect.x;
    int y = text->rect.y;
    GetTextureSize(text->texture, &w, &h);
    if (centered) x += (text->rect.w - w) / 2;
    if (centered) y += (text->rect.h - h) / 2;
    DrawTexture(text->texture, rect(x, y, w, h, 0));
}

void DrawButton(const Solid *button, const _Bool centered, const int pos_x, const int pos_y, unsigned int mode) {
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
    if (centered) x += (button->rect.w - w) / 2;
    if (centered) y += (button->rect.h - h) / 2;
    DrawTexture(button->texture, rect(x, y, w, h, 0));
}

void DisplayCursor(SDL_Texture *texture, const SDL_Rect rect){
    SDL_FRect frect = {(float)rect.x, (float)rect.y, (float)rect.w, (float)rect.h};
    frect.x -= (float)(frect.w / 2.);
    frect.y -= (float)(frect.h / 2.);
    DrawTexture(texture, FRectToRect(frect));
}

double SetVelocityX(const double var) {
    return var / 1920 * WIDTH / REFRESH_RATE * 60;
}

double SetVelocityY(const double var) {
    return var / 1080 * HEIGHT / REFRESH_RATE * 60;
}

int TimeToFrames(const double seconds) {
    return (int)((double) seconds * REFRESH_RATE);
}

void RegisterScore(char *player_name, const int difficulty) {
    FILE *F = fopen("score.txt", "a");
    if (F == NULL) {
        fprintf(stderr, "Could not open \"score.txt\" for writing\n");
        return;
    }
    fprintf(F, "%s : %d | ", player_name, score);
    if (difficulty == 1)
        fprintf(F, "EASY");
    else if (difficulty == 2)
        fprintf(F, "MEDIUM");
    else if (difficulty == 3)
        fprintf(F, "HARD");
    else if (difficulty == 4)
        fprintf(F, "HARDER");
    else if (difficulty == 5)
        fprintf(F, "INSANE");
    fprintf(F, "\n");
    fclose(F);
}

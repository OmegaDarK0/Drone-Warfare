#include "engine.h"
#include "glyph.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include "constants.h"
#include "globals.h"
#include "utils.h"

int QuitEngine(){
    running = SDL_FALSE;
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

/*
SDL_Surface *GetAsciiChar(const SDL_Surface *asciiImage, const char c) {
    const int charWidth = 8;  // Largeur d'un caractère
    const int charHeight = 8; // Hauteur d'un caractère
    const char *asciiLayout =
        " !\"#$%&'()*+,-./[\\]^_`\n"
        "0123456789:;<=>?@{|}~\n"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
        "abcdefghijklmnopqrstuvwxyz";

    // Trouver l'index du caractère dans le layout
    char *charPos = strchr(asciiLayout, c);
    if (!charPos) {
        SDL_Log("Caractère '%c' introuvable dans ascii.png", c);
        return NULL;
    }

    int charIndex = (int)(charPos - asciiLayout); // Index du caractère dans le tableau
    int cols = asciiImage->w / charWidth; // Nombre de colonnes dans l'image

    // Calculer la position du caractère dans la grille
    int row = charIndex / cols;
    int col = charIndex % cols;

    // Créer une surface vide pour contenir le caractère extrait
    SDL_Surface *charSurface = SDL_CreateRGBSurfaceWithFormat(0, charWidth, charHeight, 32, SDL_PIXELFORMAT_RGBA32);
    if (!charSurface) {
        SDL_Log("SDL_CreateRGBSurface: %s\n", SDL_GetError());
        return NULL;
    }

    Uint32 *src_pixels = (Uint32 *)asciiImage->pixels;    // Pixels source (image ASCII)
    Uint32 *dst_pixels = (Uint32 *)charSurface->pixels;   // Pixels destination (caractère)
    const int src_pitch = asciiImage->pitch / 4;          // Largeur mémoire réelle (source)
    const int dst_pitch = charSurface->pitch / 4;         // Largeur mémoire réelle (destination)

    // Copier bit à bit les pixels du caractère
    for (int y = 0; y < charHeight; y++) {
        for (int x = 0; x < charWidth; x++) {
            // Calcul des coordonnées source
            int srcX = col * charWidth + x; // Colonne dans la grille
            int srcY = row * charHeight + y; // Ligne dans la grille

            // Copier le pixel source vers la destination
            Uint32 pixel = src_pixels[srcY * src_pitch + srcX];
            dst_pixels[y * dst_pitch + x] = pixel;
        }
    }

    return charSurface;
}
*/

void CopyImage(const SDL_Surface *src, const SDL_Surface *dst, const int x_offset, const int y_offset) {
    const Uint32 *src_pixels = (Uint32 *)src->pixels;
    Uint32 *dst_pixels = (Uint32 *)dst->pixels;
    const int src_pitch = src->pitch / 4;
    const int dst_pitch = dst->pitch / 4;
    for (int y = 0; y < dst->h; y++) {
        for (int x = 0; x < dst->w; x++) {
            const int src_x = x + x_offset;
            const int src_y = y + y_offset;
            dst_pixels[y * dst_pitch + x] = src_pixels[src_y * src_pitch + src_x];
        }
    }
}

void ResizeImage(const SDL_Surface *src, const SDL_Surface *dst, const int width, const int height, const int offset) {
    // Extraction des informations source et destination
    const Uint32 *src_pixels = (Uint32 *)src->pixels;
    Uint32 *dst_pixels = (Uint32 *)dst->pixels;
    const int src_pitch = src->pitch / 4; // Largeur mémoire réelle (source)
    const int dst_pitch = dst->pitch / 4; // Largeur mémoire réelle (destination)
    const int sw = src->w;             // Largeur de l'image source
    const int sh = src->h;             // Hauteur de l'image source
    // Redimensionnement bit à bit
    for (int dy = 0; dy < height; dy++) { // Ligne destination
        for (int dx = 0; dx < width; dx++) { // Colonne destination
            // Calcul des coordonnées dans l'image source (proportions)
            const int sx = dx * sw / width;  // Coordonnée source x
            const int sy = dy * sh / height; // Coordonnée source y
            const Uint32 pixel = src_pixels[sy * src_pitch + sx]; // Récupération du pixel source
            dst_pixels[dy * dst_pitch + dx + offset] = pixel; // Placement du pixel dans la surface destination
        }
    }
}

SDL_Surface *CreateImage(const int width, const int height){
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface){
        SDL_Log("SDL_CreateRGBSurface: %s\n", SDL_GetError());
        QuitEngine();
        return NULL;
    }
    return surface;
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

SDL_Surface *LoadText(const char *text, const int size, const SDL_Color color) {
    const int scaled_size = scale_width(size);
    const int n = (int)strlen(text);
    const int char_width = scaled_size; // Largeur du caractère (dépend du paramètre size)
    const int char_height = scaled_size; // Hauteur du caractère (dépend du paramètre size)
    const int width = char_width * n + scaled_size * n / 8; // Largeur totale de la surface
    const int height = char_height;     // Hauteur totale de la surface
    SDL_Surface *dst = CreateImage(width, height);
    for (int i = 0; i < n; i++){
        const SDL_Surface *src = GetGlyph(text[i]);
        ResizeImage(src, dst, char_width, char_height, char_width * i + scaled_size * i / 8);
        ReplaceColor(dst, WHITE, color);
        //SDL_FreeSurface(src);
    }
    return dst;
}

SDL_Texture *LoadTextureFromText(const char *text, const int size, const SDL_Color color){
    SDL_Surface *surface = LoadText(text, size, color);
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

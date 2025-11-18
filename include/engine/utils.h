#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

typedef enum {
    SUCCESS = 0,
    WARNING = 1,
    ERROR = -1
}Status;

typedef enum {
    BLACK = 0x000000FF,
    WHITE = 0xFFFFFFFF,
    RED = 0xFF0000FF,
    GREEN = 0x00FF00FF,
    BLUE = 0x0000FFFF,
    YELLOW = 0xFFFF00FF,
    CYAN = 0x00FFFFFF,
    MAGENTA = 0xFF00FFFF
}Color;

inline Uint32 RGB(const Uint8 r, const Uint8 g, const Uint8 b) {
    return r << 24 | g << 16 | b << 8 | 0xFF;
}

inline Uint32 RGBA(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
    return r << 24 | g << 16 | b << 8 | a;
}

#endif //ENGINE_UTILS_H
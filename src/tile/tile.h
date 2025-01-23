#ifndef TILES_H_128937891273
#define TILES_H_128937891273
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

typedef enum tile_t : uint8_t {
        WALL              = 0,
        LEFT              = 1 << 0,
        RIGHT             = 1 << 1,
        UP                = 1 << 2,
        DOWN              = 1 << 3,
        EMPTY             = LEFT | UP | RIGHT | DOWN,
        LEFTUP            = LEFT | UP,
        LEFTRIGHT         = LEFT | RIGHT,
        LEFTDOWN          = LEFT | DOWN,
        RIGHTUP           = RIGHT | UP,
        RIGHTDOWN         = RIGHT | DOWN,
        DOWNUP            = DOWN | UP,
        WALLLEFT          = EMPTY - LEFT,
        WALLRIGHT         = EMPTY - RIGHT,
        WALLUP            = EMPTY - UP,
        WALLDOWN          = EMPTY - DOWN
} ETile;

extern const char WALLIO;
extern const char EMPTYIO;
ETile inverse_tile(ETile t);
void tiles_to_wall_string(const ETile * tiles, size_t length, char**str_p);
void tiles_to_path_string(const ETile * tiles, size_t length, wchar_t**wstr_p);

#endif // TILES_H_128937891273

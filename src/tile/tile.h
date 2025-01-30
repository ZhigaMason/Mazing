#ifndef TILES_H_128937891273
#define TILES_H_128937891273
#include <stdlib.h>
#include <wchar.h>

typedef enum tile_t {
        TILE_WALL              = 0,
        TILE_LEFT              = 1 << 0,
        TILE_RIGHT             = 1 << 1,
        TILE_UP                = 1 << 2,
        TILE_DOWN              = 1 << 3,
        TILE_EMPTY             = TILE_LEFT  | TILE_UP | TILE_RIGHT | TILE_DOWN,
        TILE_HOR               = TILE_LEFT  | TILE_RIGHT,
        TILE_LEFT_UP           = TILE_LEFT  | TILE_UP,
        TILE_LEFT_DOWN         = TILE_LEFT  | TILE_DOWN,
        TILE_RIGHT_UP          = TILE_RIGHT | TILE_UP,
        TILE_RIGHT_DOWN        = TILE_RIGHT | TILE_DOWN,
        TILE_VER               = TILE_DOWN  | TILE_UP,
        TILE_WALLLEFT          = TILE_EMPTY - TILE_LEFT,
        TILE_WALLRIGHT         = TILE_EMPTY - TILE_RIGHT,
        TILE_WALLUP            = TILE_EMPTY - TILE_UP,
        TILE_WALLDOWN          = TILE_EMPTY - TILE_DOWN
} ETile;

ETile inverse_tile(ETile t);
void tiles_to_wall_string(const ETile * tiles, size_t length, char**str_p, char WALL, char EMPTY);
void tiles_to_path_string(const ETile * tiles, size_t length, wchar_t**wstr_p);

#endif // TILES_H_128937891273

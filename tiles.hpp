#ifndef TILES_HPP_128937891273
#define TILES_HPP_128937891273
#include <cstdint>
#include <string>

enum class TILE : uint16_t {
        EMPTY             = 0b1111,
        WALL              = 0b0000,
        LEFT              = 0b0001,
        RIGHT             = 0b0010,
        UP                = 0b0100,
        DOWN              = 0b1000,
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
};

std::string line_to_string(const TILE * const tiles, size_t length);

#endif // TILES_HPP_128937891273

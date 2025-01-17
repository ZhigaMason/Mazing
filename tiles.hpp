#ifndef TILES_HPP_128937891273
#define TILES_HPP_128937891273
#include <cstdint>
#include <string>

enum class TILE : uint8_t {
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

struct tileio {
        const static char WALL;
        const static char EMPTY;
        static std::string line_to_string(const TILE * const tiles, size_t length);
private:
        tileio();
        static std::array<char, 3> _push_correct_char_l1(TILE tile);
        static std::array<char, 3> _push_correct_char_l2(TILE tile);
        static std::array<char, 3> _push_correct_char_l3(TILE tile);
};

TILE operator|(TILE l, TILE r);
TILE & operator|=(TILE & l, TILE r);
TILE operator&(TILE l, TILE r);
TILE & operator&=(TILE & l, TILE r);
bool is_present(TILE t);
bool is_absent(TILE t);
TILE inverse_tile(TILE t);



#endif // TILES_HPP_128937891273

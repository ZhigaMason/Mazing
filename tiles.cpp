#include "tiles.hpp"
#include <array>
#include <vector>

#define _tile_u8 static_cast<uint8_t>

std::array<char, 3> _push_correct_char_l1(TILE tile) {
        return {
                '#',
                (_tile_u8(tile) & _tile_u8(TILE::UP)) ? ' ' : '#',
                '#'
        };
}

std::array<char, 3> _push_correct_char_l2(TILE tile) {
        return {
                (_tile_u8(tile) & _tile_u8(TILE::LEFT)) ? ' ' : '#',
                tile != TILE::WALL ? ' ' : '#',
                (_tile_u8(tile) & _tile_u8(TILE::RIGHT)) ? ' ' : '#',

        };
}

std::array<char, 3> _push_correct_char_l3(TILE tile) {
        return {
                '#',
                (_tile_u8(tile) & _tile_u8(TILE::DOWN)) ? ' ' : '#',
                '#'
        };
}

std::string line_to_string(const TILE * const tiles, size_t length) {
        std::vector<char> res;
        res.reserve(3 * (length + 1) + 1);

        const TILE * const end = tiles + length;
        for(const TILE * it = tiles; it != end; ++it) {
                std::array<char, 3> tmp = _push_correct_char_l1(*it);
                res.push_back(tmp[0]);
                res.push_back(tmp[1]);
                res.push_back(tmp[2]);
        }
        res.push_back('\n');

        for(const TILE * it = tiles; it != end; ++it) {
                std::array<char, 3> tmp = _push_correct_char_l2(*it);
                res.push_back(tmp[0]);
                res.push_back(tmp[1]);
                res.push_back(tmp[2]);
        }
        res.push_back('\n');


        for(const TILE * it = tiles; it != end; ++it) {
                std::array<char, 3> tmp = _push_correct_char_l3(*it);
                res.push_back(tmp[0]);
                res.push_back(tmp[1]);
                res.push_back(tmp[2]);
        }
        res.push_back('\n');
        res.push_back('\0');

        return std::string(res.begin(), res.end());
}

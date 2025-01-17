#ifndef TILES_CPP_19231293789127389127
#define TILES_CPP_19231293789127389127
#include "tile.hpp"
#include <array>
#include <vector>

TILE operator|(TILE l, TILE r) {
  return TILE(static_cast<uint8_t>(l) | static_cast<uint8_t>(r));
}
TILE &operator|=(TILE &l, TILE r) { return l = (l | r); }

TILE operator&(TILE l, TILE r) {
  return TILE(static_cast<uint8_t>(l) & static_cast<uint8_t>(r));
}

TILE &operator&=(TILE &l, TILE r) { return l = (l & r); }

bool is_present(TILE t) { return static_cast<uint8_t>(t); }

bool is_absent(TILE t) { return !static_cast<uint8_t>(t); }

TILE inverse_tile(TILE t) {
        switch(t) {
                case TILE::LEFT:
                        return TILE::RIGHT;
                case TILE::RIGHT:
                        return TILE::LEFT;
                case TILE::UP:
                        return TILE::DOWN;
                case TILE::DOWN:
                        return TILE::UP;
                default:
                        return t;
        };
}

tileio::tileio() {}

const char tileio::WALL  = 'X';
const char tileio::EMPTY = ' ';

std::array<char, 3> tileio::_push_correct_char_l1(TILE tile) {
  return {tileio::WALL, is_present(tile & TILE::UP) ? tileio::EMPTY : tileio::WALL, tileio::WALL};
}

std::array<char, 3> tileio::_push_correct_char_l2(TILE tile) {
  return {
      is_present(tile & TILE::LEFT) ? tileio::EMPTY : tileio::WALL,
      tile != TILE::WALL ? tileio::EMPTY : tileio::WALL,
      is_present(tile & TILE::RIGHT) ? tileio::EMPTY : tileio::WALL,

  };
}

std::array<char, 3> tileio::_push_correct_char_l3(TILE tile) {
  return {tileio::WALL, is_present(tile & TILE::DOWN) ? tileio::EMPTY : tileio::WALL, tileio::WALL};
}

std::string tileio::line_to_string(const TILE *const tiles, size_t length) {
  std::vector<char> res;
  res.reserve(3 * (length + 1) + 1);

  const TILE *const end = tiles + length;
  for (const TILE *it = tiles; it != end; ++it) {
    std::array<char, 3> tmp = _push_correct_char_l1(*it);
    res.push_back(tmp[0]);
    res.push_back(tmp[1]);
    res.push_back(tmp[2]);
  }
  res.push_back('\n');

  for (const TILE *it = tiles; it != end; ++it) {
    std::array<char, 3> tmp = _push_correct_char_l2(*it);
    res.push_back(tmp[0]);
    res.push_back(tmp[1]);
    res.push_back(tmp[2]);
  }
  res.push_back('\n');

  for (const TILE *it = tiles; it != end; ++it) {
    std::array<char, 3> tmp = _push_correct_char_l3(*it);
    res.push_back(tmp[0]);
    res.push_back(tmp[1]);
    res.push_back(tmp[2]);
  }
  res.push_back('\n');
  res.push_back('\0');

  return std::string(res.begin(), res.end());
}
#endif // TILES_CPP_19231293789127389127

#ifndef TILES_CPP_19231293789127389127
#define TILES_CPP_19231293789127389127
#include "tiles.hpp"
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

std::array<char, 3> _push_correct_char_l1(TILE tile) {
  return {'#', is_present(tile & TILE::UP) ? ' ' : '#', '#'};
}

std::array<char, 3> _push_correct_char_l2(TILE tile) {
  return {
      is_present(tile & TILE::LEFT) ? ' ' : '#',
      tile != TILE::WALL ? ' ' : '#',
      is_present(tile & TILE::RIGHT) ? ' ' : '#',

  };
}

std::array<char, 3> _push_correct_char_l3(TILE tile) {
  return {'#', is_present(tile & TILE::DOWN) ? ' ' : '#', '#'};
}

std::string line_to_string(const TILE *const tiles, size_t length) {
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

#ifndef ETileS_CPP_19231293789127389127
#define ETileS_CPP_19231293789127389127
#include <stdio.h>
#include "tile.h"


ETile inverse_tile(ETile t) {
        switch(t) {
                case TILE_LEFT:
                        return TILE_RIGHT;
                case TILE_RIGHT:
                        return TILE_LEFT;
                case TILE_UP:
                        return TILE_DOWN;
                case TILE_DOWN:
                        return TILE_UP;
                default:
                        return t;
        };
}

static void _push_correct_char_l1(ETile tile, char ** str_p, char WALLIO, char EMPTYIO) {
	*((*str_p)++) = WALLIO;
        *((*str_p)++) = (tile & TILE_UP) ? EMPTYIO : WALLIO;
	*((*str_p)++) = WALLIO;
}

static void _push_correct_char_l2(ETile tile, char ** str_p, char WALLIO, char EMPTYIO) {
        *((*str_p)++) = (tile & TILE_LEFT)  ? EMPTYIO : WALLIO;
        *((*str_p)++) = (tile != TILE_WALL) ? EMPTYIO : WALLIO;
        *((*str_p)++) = (tile & TILE_RIGHT) ? EMPTYIO : WALLIO;
}

static void _push_correct_char_l3(ETile tile, char ** str_p, char WALLIO, char EMPTYIO) {
	*((*str_p)++) = WALLIO;
        *((*str_p)++) = (tile & TILE_DOWN) ? EMPTYIO : WALLIO;
	*((*str_p)++) = WALLIO;
}

void tiles_to_wall_string(const ETile * tiles, size_t length, char ** str_p, char WALL, char EMPTY) {
	const ETile *end = tiles + length;
	for (const ETile *it = tiles; it != end; ++it) {
		_push_correct_char_l1(*it, str_p, WALL, EMPTY);
	}
	*((*str_p)++) = '\n';

	for (const ETile *it = tiles; it != end; ++it) {
		_push_correct_char_l2(*it, str_p, WALL, EMPTY);
	}
	*((*str_p)++) = '\n';

	for (const ETile *it = tiles; it != end; ++it) {
		_push_correct_char_l3(*it, str_p, WALL, EMPTY);
	}
	*((*str_p)++) = '\n';
}

void tiles_to_path_string(const ETile * tiles, size_t length, wchar_t**wstr_p) {
	static const wchar_t char_table[] = { L' ', L'╸', L'╺', L'━', L'╹', L'┛', L'┗', L'┻', L'╻', L'┓', L'┏', L'┳', L'┃', L'┫', L'┣', L'╋'};
	for(const ETile * it = tiles, *end=tiles+length; it != end; ++it) {
		*((*wstr_p)++) = char_table[*it];
	}
	*((*wstr_p)++) = '\n';
}

#endif // ETileS_CPP_19231293789127389127

#ifndef ETileS_CPP_19231293789127389127
#define ETileS_CPP_19231293789127389127
#include <stdio.h>
#include "tile.h"


ETile inverse_tile(ETile t) {
        switch(t) {
                case LEFT:
                        return RIGHT;
                case RIGHT:
                        return LEFT;
                case UP:
                        return DOWN;
                case DOWN:
                        return UP;
                default:
                        return t;
        };
}

const char WALLIO  = '#';
const char EMPTYIO = ' ';

void _push_correct_char_l1(ETile tile, char ** str_p) {
	*((*str_p)++) = WALLIO;
        *((*str_p)++) = (tile & UP) ? EMPTYIO : WALLIO;
	*((*str_p)++) = WALLIO;
}

void _push_correct_char_l2(ETile tile, char ** str_p) {
        *((*str_p)++) = (tile & LEFT)  ? EMPTYIO : WALLIO;
        *((*str_p)++) = (tile != WALL) ? EMPTYIO : WALLIO;
        *((*str_p)++) = (tile & RIGHT) ? EMPTYIO : WALLIO;
}

void _push_correct_char_l3(ETile tile, char ** str_p) {
	*((*str_p)++) = WALLIO;
        *((*str_p)++) = (tile & DOWN) ? EMPTYIO : WALLIO;
	*((*str_p)++) = WALLIO;
}

void tiles_to_wall_string(const ETile * tiles, size_t length, char ** str_p) {
	const ETile *end = tiles + length;
	for (const ETile *it = tiles; it != end; ++it) {
		_push_correct_char_l1(*it, str_p);
	}
	*((*str_p)++) = '\n';

	for (const ETile *it = tiles; it != end; ++it) {
		_push_correct_char_l2(*it, str_p);
	}
	*((*str_p)++) = '\n';

	for (const ETile *it = tiles; it != end; ++it) {
		_push_correct_char_l3(*it, str_p);
	}
	*((*str_p)++) = '\n';
}

#endif // ETileS_CPP_19231293789127389127

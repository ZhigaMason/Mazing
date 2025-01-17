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

const char WALLIO  = 'X';
const char EMPTYIO = ' ';

const char * _push_correct_char_l1(ETile tile) {
        static char ret[]= {WALLIO, WALLIO, WALLIO, '\0'};
        ret[1] = (tile & UP) ? EMPTYIO : WALLIO;
        return ret;
}

const char * _push_correct_char_l2(ETile tile) {
        static char ret[]= {WALLIO, WALLIO, WALLIO, '\0'};
        ret[0] = (tile & LEFT) ? EMPTYIO : WALLIO;
        ret[1] = (tile != WALL) ? EMPTYIO : WALLIO;
        ret[2] = (tile & RIGHT) ? EMPTYIO : WALLIO;
        return ret;
}

const char * _push_correct_char_l3(ETile tile) {
        static char ret[]= {WALLIO, WALLIO, WALLIO, '\0'};
        ret[1] = (tile & DOWN) ? EMPTYIO : WALLIO;
        return ret;
}

void print_tiles(const ETile * tiles, size_t length) {
	const ETile *end = tiles + length;
	for (const ETile *it = tiles; it != end; ++it) {
		printf("%s", _push_correct_char_l1(*it));
	}
	printf("\n");

	for (const ETile *it = tiles; it != end; ++it) {
		printf("%s", _push_correct_char_l2(*it));
	}
	printf("\n");

	for (const ETile *it = tiles; it != end; ++it) {
		printf("%s", _push_correct_char_l3(*it));
	}
	printf("\n");
}
#endif // ETileS_CPP_19231293789127389127

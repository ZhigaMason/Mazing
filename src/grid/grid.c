#ifndef GRID_C_123927891273812731293719
#define GRID_C_123927891273812731293719
#include "grid.h"
#include "data_structures/stack.h"
#include "data_structures/random_container.h"
#include <stdlib.h>
#include <assert.h>

enum CMP _cmp_coords(TCoords l, TCoords r) {
  	return l.x == r.x ? l.y > l.x : -(l.y > l.x);
}

struct pair_tiles tiles_from_step(TCoords src, TCoords dst) {
	int dx = src.x - dst.x, dy = src.y - dst.y;
	struct pair_tiles ret = {.f = WALL, .s=WALL};
	if(dx == 1) {
		ret.f |= LEFT;
		ret.s |= RIGHT;
	}
	else if(dx == -1) {
		ret.f |= RIGHT;
		ret.s |= LEFT;
	}

	if(dy == 1) {
		ret.f |= UP;
		ret.s |= DOWN;
	}
	else if(dy == -1) {
		ret.f |= DOWN;
		ret.s |= UP;
	}

	return ret;
}

void clean_grid(PtrGrid * ppg) {
	PtrGrid pg = *ppg;
	if(!pg || !pg->data) return;
	ETile ** end = pg->data + pg->height;
	for(ETile ** it = pg->data; it != end; /*++it*/) {
		ETile ** tmp = it;
		++it;
		free(*tmp);
	}
	free(pg->data);
	free(pg);
	*ppg = nullptr;
}

PtrGrid make_grid(int height, int width, TCoords start, TCoords exit) {
	if(height < 1 || width < 1 ||
		start.x < 0 || exit.x < 0 ||
		start.x >= width || exit.x >=width ||
		start.y < 0 || exit.y < 0 ||
		start.y >= height || exit.y >= height ||
		(start.x == exit.x && start.y == exit.y)
	) return nullptr;

	PtrGrid ret = malloc(sizeof(*ret));
	{
		TGrid tmp = {.height=height, .width=width, .start=start, .exit=exit};
		*ret = tmp;
	}
	ret->data = calloc(height, sizeof(*ret->data));
	if(!ret->data) {
		free(ret->data);
		return nullptr;
	}
	ETile ** end = ret->data + ret->height;
	for(ETile ** it = ret->data; it != end; ++it) {
		*it = calloc(width, sizeof(*ret->data));
		if(!*it) {
			ret->height = it - ret->data + 1;
			clean_grid(&ret);
			return nullptr;
		}
	}
	return ret;
}

PtrGrid make_grid_corners(int height, int width) {
	TCoords start={0, 0}, exit = {width - 1, height - 1};
	return make_grid(height, width, start, exit);
}

void print_grid(PtrCGrid pg) {
	ETile ** end   = pg->data + pg->height;
	ETile ** start = pg->data + pg->start.y;
	ETile ** exit  = pg->data + pg->exit.y;
	for(ETile ** it = pg->data; it != end; ++it) {
		if(start == it && exit == it) {
			print_tiles_and_two_chars(
				*it, pg->width,
				pg->exit.x,  'E',
				pg->start.x, 'S'
			);
		}
		else if(start != it && exit == it) {
			print_tiles_and_single_char(
				*it, pg->width, pg->exit.x, 'E'
			);
		}
		else if(start == it && exit != it) {
			print_tiles_and_single_char(
				*it, pg->width, pg->start.x, 'S'
			);
		}
		else{
			print_tiles(*it, pg->width);
		}
	}
}

TCoords * _neighbors(PtrCGrid pg, TCoords cs) {
	static TCoords ret[] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, 0}};
	uint8_t idx = 0;
	if(cs.x > 0) {
		ret[idx].x = cs.x - 1;
		ret[idx++].y = cs.y;
	}
	if(cs.x < pg->width - 1) {
		ret[idx].x = cs.x + 1;
		ret[idx++].y = cs.y;
	}
	if(cs.y > 0) {
		ret[idx].x = cs.x;
		ret[idx++].y = cs.y - 1;
	}
	if(cs.y < pg->height - 1) {
		ret[idx].x = cs.x;
		ret[idx++].y = cs.y + 1;
	}

	ret[4].y = idx;
	ret[idx].x = -1;
	return ret;
}

void _clean_probability_space(PtrProbaSp *pps) { clean_grid(pps); }

PtrProbaSp _make_blank_probability_space(PtrCGrid pg) {
	PtrGrid ret = malloc(sizeof(*ret));
	ret->width  = pg->width;
	ret->height = pg->height;
	ret->data = calloc(ret->height, sizeof(*ret->data));
	if(!ret->data) {
		free(ret->data);
		return nullptr;
	}
	ETile ** end = ret->data + ret->height;
	for(ETile ** it = ret->data; it != end; ++it) {
		*it = calloc(ret->width, sizeof(*ret->data));
		if(!*it) {
			ret->height = it - ret->data + 1;
			_clean_probability_space(&ret);
			return nullptr;
		}

		ETile * end = *it + ret->width;
		for(ETile * jt = *it; jt != end; ++jt)
			*jt = EMPTY;
	}

        for(int i = 0; i < ret->height; ++i) {
                ret->data[i][0]         &= ~LEFT;
                ret->data[i][ret->width - 1] &= ~RIGHT;
        }

        for(int i = 0; i < ret->width; ++i) {
                ret->data[0][i]               &= ~UP;
                ret->data[ret->height - 1][i] &= ~DOWN;
        }
	return ret;
}

PtrProbaSp _generate_path(PtrGrid pg) {
	TCoordsStack st = init_stack();
	push_stack(&st, pg->start);

	PtrGrid grid_visited = make_grid_corners(pg->height, pg->width);
	uint8_t ** visited = grid_visited->data;
	visited[pg->start.y][pg->start.x] = 1;

	while(st.size) {
		TCoords curr = top_stack(&st);

		TCoords *adj = _neighbors(pg, curr);
		int length = adj[4].y;

		while(length) {
			size_t idx = rand() % length;
			TCoords n = adj[idx];
			if(visited[n.y][n.x] == 0) {
				push_stack(&st, n);
				visited[n.y][n.x] = 1;
				if(n.x == pg->exit.x && n.y == pg->exit.y)
					goto __path_generate_construction;
				break;
			}
			else {
				adj[idx] = adj[length - 1];
				adj[--length].x = -1;
			}
		}

		if(!length)
			pop_stack(&st);
	}
__path_generate_construction:
	PtrProbaSp pss = nullptr;
	if(!st.size) goto __path_generate_clean;

	pss = _make_blank_probability_space(pg);
	if(!pss) goto __path_generate_clean;
	ETile ** grid = pg->data;
	ETile ** prob = pss->data;
	for(const TCoords *it = st.data, *end=st.data + st.size - 1; it != end; ++it) {
		const TCoords *pit = it + 1;
		struct pair_tiles tiles = tiles_from_step(*pit, *it);
		grid[pit->y][pit->x] |= tiles.f;
		grid[it->y][it->x]   |= tiles.s;
		prob[pit->y][pit->x] &= ~tiles.f;
		prob[it->y][it->x]   &= ~tiles.s;
	}

__path_generate_clean:
	clean_stack(&st);
	clean_grid(&grid_visited);
	return pss;
}

void _colapse_wave_function(PtrGrid pg, PtrProbaSp pps) {
	PtrGrid grid_visited = make_grid_corners(pg->height, pg->width);
	uint8_t ** visited = grid_visited->data;
	visited[pg->start.y][pg->start.x] = 1;
	visited[pg->exit.y][pg->exit.x] = 1;

	TCoordsRandContainer rc = init_random_container();
	push_random(&rc, pg->start);
	push_random(&rc, pg->exit);

	while(rc.size) {
		TCoords curr = pop_random(&rc);
		pg->data[curr.y][curr.x] |= (rand() & pps->data[curr.y][curr.x]);
		ETile curr_tile = pg->data[curr.y][curr.x];

		TCoords * adj = _neighbors(pg, curr);
		int length = adj[4].y;
		for(const TCoords * it = adj, *end=adj+length; it != end; ++it) {
			struct pair_tiles tiles = tiles_from_step(curr, *it);
			ETile new_tile = curr_tile & tiles.s;
			pg->data[it->y][it->x]  |= new_tile;
			pps->data[it->y][it->x] &= ~tiles.f;
			if(!visited[it->y][it->x]) {
				visited[it->y][it->x] = 1;
				push_random(&rc, *it);
			}
		}
	}

	clean_random(&rc);
	clean_grid(&grid_visited);
}

void fill_grid(PtrGrid pg, long seed) {
	srand(seed);
	PtrProbaSp pps = _generate_path(pg);
	_colapse_wave_function(pg, pps);
	_clean_probability_space(&pps);
}

#endif  //GRID_C_123927891273812731293719

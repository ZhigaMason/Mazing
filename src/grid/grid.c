#ifndef GRID_C_123927891273812731293719
#define GRID_C_123927891273812731293719
#include "grid.h"
#include "data_structures/stack.h"
#include "data_structures/random_container.h"
#include <stdlib.h>
#include <assert.h>

struct pair_tiles tiles_from_step(TCoords src, TCoords dst) {
	int dx = src.x - dst.x, dy = src.y - dst.y;
	struct pair_tiles ret = {.f = TILE_WALL, .s=TILE_WALL};
	if(dx == 1) {
		ret.f |= TILE_LEFT;
		ret.s |= TILE_RIGHT;
	}
	else if(dx == -1) {
		ret.f |= TILE_RIGHT;
		ret.s |= TILE_LEFT;
	}

	if(dy == 1) {
		ret.f |= TILE_UP;
		ret.s |= TILE_DOWN;
	}
	else if(dy == -1) {
		ret.f |= TILE_DOWN;
		ret.s |= TILE_UP;
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
	*ppg = NULL;
}

void clear_grid(PtrGrid pg) {
	for(ETile ** it=pg->data, **end=pg->data + pg->height; it != end; ++it) {
		for(ETile * jt = *it, *jend=*it + pg->width; jt != jend; ++jt)
			*jt = TILE_WALL;
	}
}

PtrGrid make_grid(int height, int width, TCoords start, TCoords exit) {
	if(height < 1 || width < 1 ||
		start.x < 0 || exit.x < 0 ||
		start.x >= width || exit.x >=width ||
		start.y < 0 || exit.y < 0 ||
		start.y >= height || exit.y >= height ||
		(start.x == exit.x && start.y == exit.y)
	) return NULL;

	PtrGrid ret = malloc(sizeof(*ret));
	{
		TGrid tmp = {.height=height, .width=width, .start=start, .exit=exit};
		*ret = tmp;
	}
	ret->data = calloc(height, sizeof(*ret->data));
	if(!ret->data) {
		free(ret->data);
		return NULL;
	}
	ETile ** end = ret->data + ret->height;
	for(ETile ** it = ret->data; it != end; ++it) {
		*it = calloc(width, sizeof(*ret->data));
		if(!*it) {
			ret->height = it - ret->data + 1;
			clean_grid(&ret);
			return NULL;
		}
	}
	return ret;
}

PtrGrid make_grid_corners(int height, int width) {
	TCoords start={0, 0}, exit = {width - 1, height - 1};
	return make_grid(height, width, start, exit);
}

char *  grid_to_wall_string(PtrCGrid pg, char WALL, char EMPTY) {
	ETile ** end   = pg->data + pg->height;
	char * ret = calloc(pg->height * pg->width * 9 + pg->height * 3 + 1, sizeof(char));
	char * str = ret;
	for(ETile ** it = pg->data; it != end; ++it) {
		tiles_to_wall_string(*it, pg->width, &str, WALL, EMPTY);
	}
	*str = '\0';
	ret[pg->width * 3 + 2 + pg->start.y * pg->width * 9 + pg->start.y * 3 + pg->start.x * 3] = 'S';
	ret[pg->width * 3 + 2 + pg->exit.y  * pg->width * 9 + pg->exit.y * 3  + pg->exit.x * 3]  = 'E';
	return ret;
}

wchar_t * grid_to_path_string(PtrCGrid pg) {
	ETile ** end   = pg->data + pg->height;
	wchar_t * ret = calloc(pg->height * pg->width + pg->height + 1, sizeof(wchar_t));
	wchar_t * str = ret;
	for(ETile ** it = pg->data; it != end; ++it) {
		tiles_to_path_string(*it, pg->width, &str);
	}
	*str = '\0';
	ret[pg->width * pg->start.y + pg->start.y + pg->start.x] = 'S';
	ret[pg->width * pg->exit.y + pg->exit.y + pg->exit.x]    = 'E';
	return ret;
}

TCoords * _neighbors(PtrCGrid pg, TCoords cs) {
	static TCoords ret[] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, 0}};
	int idx = 0;
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
		return NULL;
	}
	ETile ** end = ret->data + ret->height;
	for(ETile ** it = ret->data; it != end; ++it) {
		*it = calloc(ret->width, sizeof(*ret->data));
		if(!*it) {
			ret->height = it - ret->data + 1;
			_clean_probability_space(&ret);
			return NULL;
		}

		ETile * end = *it + ret->width;
		for(ETile * jt = *it; jt != end; ++jt)
			*jt = TILE_EMPTY;
	}

        for(int i = 0; i < ret->height; ++i) {
                ret->data[i][0]              &= ~TILE_LEFT;
                ret->data[i][ret->width - 1] &= ~TILE_RIGHT;
        }

        for(int i = 0; i < ret->width; ++i) {
                ret->data[0][i]               &= ~TILE_UP;
                ret->data[ret->height - 1][i] &= ~TILE_DOWN;
        }
	return ret;
}

PtrProbaSp _generate_path(PtrGrid pg) {
	TCoordsStack st = init_stack();
	push_stack(&st, pg->start);

	PtrGrid grid_visited = make_grid_corners(pg->height, pg->width);
	ETile ** visited = grid_visited->data;
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
__path_generate_construction:;
	PtrProbaSp pss = NULL;
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

__path_generate_clean:;
	clean_stack(&st);
	clean_grid(&grid_visited);
	return pss;
}

void _colapse_wave_function(PtrGrid pg, PtrProbaSp pps) {
	PtrGrid grid_visited = make_grid_corners(pg->height, pg->width);
	ETile ** visited = grid_visited->data;
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
			ETile new_tile = inverse_tile(curr_tile & tiles.f);
			pg->data[it->y][it->x]  |= new_tile;
			pps->data[it->y][it->x] &= ~tiles.s;
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

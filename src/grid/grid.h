#ifndef GRID_H_12837891273891278
#define GRID_H_12837891273891278
#include "../tile/tile.h"

enum CMP : int8_t {LESS = -1, EQUAL = 0, GREAT = 1};

typedef struct coords_t {
	int x, y;
} TCoords;

enum CMP _cmp_coords(TCoords l, TCoords r);

struct pair_tiles {
  ETile f, s;
};

struct pair_tiles tiles_from_step(TCoords src, TCoords dst);

typedef struct grid_t {
	ETile ** data;
	int width, height;
	TCoords start, exit;
} TGrid;

typedef TGrid* PtrGrid;
typedef TGrid* PtrProbaSp;
typedef const TGrid* PtrCGrid;

void clean_grid(PtrGrid *);
PtrGrid make_grid(int height, int width, TCoords start, TCoords finish);
PtrGrid make_grid_corners(int height, int width);
char * grid_to_wall_string(PtrCGrid);

TCoords * _neighbors(PtrCGrid, TCoords);
void _clean_probability_space(PtrProbaSp *);
PtrProbaSp _make_blank_probability_space(PtrCGrid);
PtrProbaSp  _generate_path(PtrGrid);
void _colapse_wave_function(PtrGrid, PtrProbaSp);

void fill_grid(PtrGrid, long);

#endif //GRID_HPP_12837891273891278

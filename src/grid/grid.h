#ifndef GRID_H_12837891273891278
#define GRID_H_12837891273891278
#include "../tile/tile.h"

typedef struct coords_t {
	int x, y;
} TCoords;

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
void clear_grid(PtrGrid);
PtrGrid make_grid(int height, int width, TCoords start, TCoords finish);
PtrGrid make_grid_corners(int height, int width);
char *    grid_to_wall_string(PtrCGrid, char, char);
wchar_t * grid_to_path_string(PtrCGrid);

TCoords * _neighbors(PtrCGrid, TCoords);
void _clean_probability_space(PtrProbaSp *);
PtrProbaSp _make_blank_probability_space(PtrCGrid);
PtrProbaSp  _generate_path(PtrGrid);
void _colapse_wave_function(PtrGrid, PtrProbaSp);

void fill_grid(PtrGrid, long);

#endif //GRID_HPP_12837891273891278

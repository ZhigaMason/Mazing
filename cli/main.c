#include "../src/grid/grid.h"
#include <locale.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	setlocale(LC_ALL, "");
	fwide(stdout, 1);
	long seed = 0;
	int h = 10, w = 10;
	bool defined_start_exit = false;
	TCoords s = {0 ,0}, e = {9, 9};
	if(argc >= 2) {
		seed = strtol(argv[1], nullptr, 16);
		wprintf(L"Using seed: %lxd\n", seed);
	}
	else wprintf(L"Using default seed.\n");

	if(argc >= 4) {
		h = strtol(argv[2], nullptr, 10);
		w = strtol(argv[3], nullptr, 10);
		wprintf(L"Maze: %dx%d\n", h, w);
	}

	if(argc == 8) {
		defined_start_exit = true;
		s.x = strtol(argv[4], nullptr, 10);
		s.y = strtol(argv[5], nullptr, 10);
		wprintf(L"Start: %d, %d\n", s.x, s.y);

		e.x = strtol(argv[6], nullptr, 10);
		e.y = strtol(argv[7], nullptr, 10);
		wprintf(L"Exit:  %d, %d\n", e.x, e.y);
	}

	PtrGrid g = nullptr;
	if(defined_start_exit) g = make_grid(h, w, s, e);
	else g = make_grid_corners(h, w);
	if(!g){
		wprintf(L"An error occured\n");
		return 1;
	}

	fill_grid(g, seed);


	char * str = grid_to_wall_string(g);
	wprintf(L"%s\n\n", str);

	wchar_t * wstr = grid_to_path_string(g);
	wprintf(L"%ls", wstr);

	free(str);
	free(wstr);
	clean_grid(&g);
        return 0;
}

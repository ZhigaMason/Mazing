#include "../src/tile/tile.h"

int main(int argc, char ** argv) {
	ETile arr[3] = {RIGHT, LEFTRIGHT, LEFTDOWN};
	print_tiles(arr, 3);
        return 0;
}

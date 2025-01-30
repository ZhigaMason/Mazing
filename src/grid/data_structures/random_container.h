#ifndef RANDOM_CONTAINER_H_12312312312
#define RANDOM_CONTAINER_H_12312312312
#include "../grid.h"

typedef struct coords_random_container_t {
	TCoords * data;
	size_t capacity, size;
} TCoordsRandContainer;
typedef TCoordsRandContainer* PtrCoordsRandContainer;
typedef TCoordsRandContainer* PtrCCoordsRandomContainer;

TCoordsRandContainer init_random_container(void);

void clean_random(PtrCoordsRandContainer);

void swap_random(PtrCoordsRandContainer);

void push_random(PtrCoordsRandContainer, TCoords);
TCoords top_random(PtrCCoordsRandomContainer);
TCoords pop_random(PtrCoordsRandContainer);

#endif//RANDOM_CONTAINER_H_12312312312

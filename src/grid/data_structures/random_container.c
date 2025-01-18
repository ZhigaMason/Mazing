#ifndef RANDOM_CONTAINER_C_131123123123
#define RANDOM_CONTAINER_C_131123123123
#include "random_container.h"

TCoordsRandContainer init_random_container() {
	TCoordsRandContainer ret = {.data=nullptr, .capacity=0, .size=0};
	return ret;
}

void clean_random(PtrCoordsRandContainer rc) {
	if(rc->data) {
		free(rc->data);
		rc->data = nullptr;
		rc->capacity = rc->size = 0;
	}
}

void swap_random(PtrCoordsRandContainer rc) {
	size_t idx = rand() % rc->size;
	TCoords tmp = rc->data[idx];
	rc->data[idx] = rc->data[rc->size - 1];
	rc->data[rc->size - 1] = tmp;
}

void push_random(PtrCoordsRandContainer rc, TCoords c) {
	if(rc->capacity <= rc->size) {
		rc->capacity = rc->capacity*2 + 5;
		rc->data = realloc(rc->data, rc->capacity * sizeof(*rc->data));
	}
	rc->data[rc->size++] = c;
}

TCoords top_random(PtrCCoordsRandomContainer rc) {
	swap_random(rc);
	return rc->data[rand() % rc->size];
}

TCoords pop_random(PtrCoordsRandContainer rc) {
	swap_random(rc);
	return rc->data[--rc->size];
}
#endif//RANDOM_CONTAINER_C_131123123123

#ifndef STACK_H_12398120381209381092801
#define STACK_H_12398120381209381092801
#include "../grid.h"

typedef struct coords_stack_t {
	TCoords * data;
	size_t capacity, size;
} TCoordsStack;
typedef TCoordsStack* PtrCoordsStack;
typedef TCoordsStack* PtrCCoordsStack;

TCoordsStack init_stack();

void clean_stack(PtrCoordsStack);

void push_stack(PtrCoordsStack, TCoords);
TCoords top_stack(PtrCCoordsStack);
TCoords pop_stack(PtrCoordsStack);


#endif//STACK_H_12398120381209381092801

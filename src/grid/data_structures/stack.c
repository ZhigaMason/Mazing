#ifndef STACK_C_123129378912739179231
#define STACK_C_123129378912739179231
#include "stack.h"

TCoordsStack init_stack() {
	TCoordsStack ret = {.data=nullptr, .capacity=0, .size=0};
	return ret;
}

void clean_stack(PtrCoordsStack st) {
	if(st->data) {
		free(st->data);
		st->data = nullptr;
		st->capacity = st->size = 0;
	}
}

void push_stack(PtrCoordsStack st, TCoords c) {
	if(st->capacity <= st->size) {
		st->capacity = st->capacity*2 + 5;
		st->data = realloc(st->data, st->capacity * sizeof(*st->data));
	}
	st->data[st->size++] = c;
}

TCoords top_stack(PtrCCoordsStack st) {
	return st->data[st->size - 1];
}

TCoords pop_stack(PtrCoordsStack st) {
	st->size -= 1;
	return st->data[st->size];
}
#endif//STACK_C_123129378912739179231

#ifndef TILE_PYTHON_CLASS_H_940F12F87N9NF41297F
#define TILE_PYTHON_CLASS_H_940F12F87N9NF41297F
#include <Python.h>
#include "../../tile/tile.h"

typedef struct {
	PyObject_HEAD;
	ETile q_val;
} PyGridTile;

void PyGridTile_del(PyGridTile * self);
PyObject * PyGridTile_repr(PyGridTile * self);
void PyGridTile_initialize_class();

extern PyTypeObject tile_type;
#endif//TILE_PYTHON_CLASS_H_940F12F87N9NF41297F

#ifndef TILE_PYTHON_CLASS_H_940F12F87N9NF41297F
#define TILE_PYTHON_CLASS_H_940F12F87N9NF41297F
#include <Python.h>
#include "../../tile/tile.h"

typedef struct {
	PyObject_HEAD;
	ETile q_val;
} PyMazeTile;

void PyMazeTile_del(PyMazeTile * self);
PyObject * PyMazeTile_repr(PyMazeTile * self);
void PyMazeTile_initialize_class();

extern PyTypeObject PyMazeTile_Type;
extern PyObject * _PyTile_Objects[16];
#endif//TILE_PYTHON_CLASS_H_940F12F87N9NF41297F

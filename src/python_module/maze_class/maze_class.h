#ifndef GRID_PYTHON_CLASS_H_123S89F7DF987DG9S7DG
#define GRID_PYTHON_CLASS_H_123S89F7DF987DG9S7DG
#include <Python.h>
#include "../../grid/grid.h"

typedef struct {
	PyObject_HEAD
	PtrGrid q_grid;
	uint8_t q_is_generated;
} PyMaze;

int PyMaze_init(PyMaze *self, PyObject *args, PyObject *kwargs);

void PyMaze_del(PyMaze * self);

PyObject * PyMaze_str(PyMaze * self);

PyObject * PyMaze_repr(PyMaze * self);

// Getters and setters
PyObject * PyMaze_get_start(PyMaze * self, void * closure);

int PyMaze_set_start(PyMaze * self, PyObject * value, void * closure);

PyObject * PyMaze_get_exit(PyMaze * self, void * closure);

int PyMaze_set_exit(PyMaze * self, PyObject * value, void * closure);

PyObject * PyMaze_get_is_generated(PyMaze * self, void * closure);

int PyMaze_set_is_generated(PyMaze * self, PyObject * value, void * closure);

extern PyGetSetDef PyMaze_getset[];

PyObject * PyMaze_getitem(PyMaze * self, PyObject * key);

int PyMaze_setitem(PyMaze * self, PyObject * key, PyObject * val);

extern PyMappingMethods PyMaze_mappings;

// Methods
PyMaze * PyMaze_fill_maze(PyMaze * self, PyObject * args, PyObject * kwargs);

PyMaze * PyMaze_clear_maze(PyMaze * self, PyObject * args);

PyMaze * PyMaze_set_start_safe(PyMaze * self, PyObject * args);

PyMaze * PyMaze_set_exit_safe(PyMaze * self, PyObject * args);

PyObject * PyMaze_to_string(PyMaze * self, PyObject * args, PyObject * kwargs);

extern PyMethodDef PyMaze_methods[];

extern PyTypeObject PyMaze_Type;

#endif//GRID_PYTHON_CLASS_H_123S89F7DF987DG9S7DG

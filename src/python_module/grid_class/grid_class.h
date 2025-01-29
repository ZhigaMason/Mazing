#ifndef GRID_PYTHON_CLASS_H_123S89F7DF987DG9S7DG
#define GRID_PYTHON_CLASS_H_123S89F7DF987DG9S7DG
#include <Python.h>
#include "../../grid/grid.h"

typedef struct {
	PyObject_HEAD;
	PtrGrid q_grid;
	bool q_is_filled;
} PyGrid;

int PyGrid_init(PyGrid *self, PyObject *args, PyObject *kwargs);

void PyGrid_del(PyGrid * self);

PyObject * PyGrid_str(PyGrid * self);

PyObject * PyGrid_repr(PyGrid * self);

// Getters and setters
PyObject * PyGrid_get_start(PyGrid * self, void * closure);

int PyGrid_set_start(PyGrid * self, PyObject * value, void * closure);

PyObject * PyGrid_get_exit(PyGrid * self, void * closure);

int PyGrid_set_exit(PyGrid * self, PyObject * value, void * closure);

PyObject * PyGrid_get_is_filled(PyGrid * self, void * closure);

int PyGrid_set_is_filled(PyGrid * self, PyObject * value, void * closure);

extern PyGetSetDef PyGrid_getset[];

PyGrid * PyGrid_fill_maze(PyGrid * self, PyObject * args, PyObject * kwargs);

PyGrid * PyGrid_clear_maze(PyGrid * self, PyObject * args);

PyGrid * PyGrid_set_start_safe(PyGrid * self, PyObject * args);

PyGrid * PyGrid_set_exit_safe(PyGrid * self, PyObject * args);

extern PyMethodDef PyGrid_methods[];

extern PyTypeObject grid_type;

#endif//GRID_PYTHON_CLASS_H_123S89F7DF987DG9S7DG

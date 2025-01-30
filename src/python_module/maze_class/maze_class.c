#ifndef GRID_PYTHON_CLASS_C_123S89F7DF987DG9S7DG
#define GRID_PYTHON_CLASS_C_123S89F7DF987DG9S7DG
#include <Python.h>
#include "./maze_class.h"
#include "../tile_class/tile_class.h"
#include "../../grid/grid.h"


PyObject * _coords_t_to_pytuple(TCoords c) {
	PyObject * ret = PyTuple_New(2);
	PyTuple_SET_ITEM(ret, 0, PyLong_FromLong(c.x));
	PyTuple_SET_ITEM(ret, 1, PyLong_FromLong(c.y));
	return ret;
}

static int _pytuple_to_coords_t(PyObject * o, TCoords * c_ptr, const char * non_tuple_error, const char * invalid_num_entries, const char * non_positive_entries) {
	if(!PyTuple_Check(o)) {
		PyErr_SetString(PyExc_ValueError, non_tuple_error);
		return -1;
	}

	if(PyTuple_Size(o) != 2) {
		PyErr_SetString(PyExc_ValueError, invalid_num_entries);
		return -1;
	}

	int x = PyLong_AsLong(PyTuple_GET_ITEM(o, 0));
	int y = PyLong_AsLong(PyTuple_GET_ITEM(o, 1));
	if(x < 0 || y < 0) {
		PyErr_SetString(PyExc_ValueError, non_positive_entries);
		return -1;
	}
	c_ptr->x = x;
	c_ptr->y = y;
	return 0;
}

int PyMaze_init(PyMaze *self, PyObject *args, PyObject *kwargs) {
	static char * kws[] = {
		"height", "width", "start", "exit", "fill", "seed", NULL
	};

	int height = 10, width = 10;
	int dofill_grid = 0;
	long seed = 1;
	PyObject *start_tpl=NULL, *exit_tpl = NULL;

	if(!PyArg_ParseTupleAndKeywords(args, kwargs, "|ii$OOpl:__init__", kws, &height, &width, &start_tpl, &exit_tpl, &dofill_grid, &seed)) {
		return -1;
	}

	if(height < 1 || width < 1) {
		PyErr_SetString(PyExc_ValueError, "Expected height and width of grid to be greater than one.");
		return -1;
	}

	TCoords start = {.x=0, .y=0};
	TCoords exit  = {.x=width-1, .y=height-1};

	if(start_tpl && _pytuple_to_coords_t(
		start_tpl, &start, "Expected tuple for start", "Expected start tuple to have 2 entries", "Expected start entries to be positive integers"
	)) return -1;

	if(start.x >= width && start.y >= height) {
		PyErr_SetString(PyExc_IndexError, "Expected start to be inside of maze");
		return -1;
	}

	if(exit_tpl && _pytuple_to_coords_t(
		exit_tpl, &exit, "Expected tuple for exit", "Expected exit tuple to have 2 entries", "Expected exit entries to be positive integers"
	)) return -1;

	if(exit.x >= width && exit.y >= height) {
		PyErr_SetString(PyExc_IndexError, "Expected exit to be inside of maze");
		return -1;
	}

	if(exit.x == start.x && exit.y == start.y) {
		PyErr_SetString(PyExc_ValueError, "Start and finish cannot be on the same tile");
		return -1;
	}


	self->q_grid = make_grid(
		height, width,
		start, exit
	);
	self->q_is_generated = 0;

	if(!self->q_grid) {
		PyErr_SetString(PyExc_MemoryError, "Memory allocation error occured.");
		return -1;
	}


	if(dofill_grid) {
		fill_grid(self->q_grid, seed);
		self->q_is_generated = 1;
	}

	return 0;
}

void PyMaze_del(PyMaze * self) {
	clean_grid(&(self->q_grid));
	Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject * PyMaze_str(PyMaze * self) {
	char * str = grid_to_wall_string(self->q_grid, '#', ' ');
	PyObject * ret = PyUnicode_FromStringAndSize(str, self->q_grid->height * self->q_grid->width * 9 + self->q_grid->height * 3);
	free(str);
	return ret;
}

PyObject * PyMaze_repr(PyMaze * self) {
	wchar_t * str = grid_to_path_string(self->q_grid);
	PyObject * ret = PyUnicode_FromWideChar(str, self->q_grid->height * self->q_grid->width + self->q_grid->height);
	free(str);
	return ret;
}

PyObject * PyMaze_get_start(PyMaze * self, void * closure) {
	return _coords_t_to_pytuple(self->q_grid->start);
}

int PyMaze_set_start(PyMaze * self, PyObject * value, void * closure) {
	if(_pytuple_to_coords_t(
		value, &self->q_grid->start, "Expected tuple for start", "Expected start tuple to have 2 entries", "Expected start entries to be positive integers"
	)) return -1;
	return 0;
}

PyObject * PyMaze_get_exit(PyMaze * self, void * closure) {
	return _coords_t_to_pytuple(self->q_grid->exit);
}

int PyMaze_set_exit(PyMaze * self, PyObject * value, void * closure) {
	if(_pytuple_to_coords_t(
		value, &self->q_grid->exit, "Expected tuple for exit", "Expected exit tuple to have 2 entries", "Expected exit entries to be positive integers"
	)) return -1;
	return 0;
}

PyObject * PyMaze_get_is_generated(PyMaze * self, void * closure) {
	return PyBool_FromLong(self->q_is_generated);
}

int PyMaze_set_is_generated(PyMaze * self, PyObject * value, void * closure) {
	if(!PyBool_Check(value)) return -1;
	self->q_is_generated = Py_IsTrue(value);
	return 0;
}

PyGetSetDef PyMaze_getset[] = {
    {"start",        (getter)PyMaze_get_start,         (setter)PyMaze_set_start,         "Start coordinates in the maze", NULL},
    {"exit",         (getter)PyMaze_get_exit,          (setter)PyMaze_set_exit,          "Exit coordinates in the maze",  NULL},
    {"is_generated", (getter)PyMaze_get_is_generated,  (setter)PyMaze_set_is_generated,  "Bool value if maze is filled",  NULL},
    {NULL}
};

PyObject * PyMaze_getitem(PyMaze * self, PyObject * key) {
	TCoords c;
        if (_pytuple_to_coords_t(
                key, &c, "Expected tuple for start",
                "Expected start tuple to have 2 entries",
                "Expected start entries to be positive integers") < 0)
		return NULL;
	if(c.x >= self->q_grid->width || c.y >= self->q_grid->height) {
		PyErr_SetString(PyExc_IndexError, "Expected coordinates to be inside of maze");
		return NULL;
	}
	return _PyTile_Objects[self->q_grid->data[c.y][c.x]];
}

int PyMaze_setitem(PyMaze * self, PyObject * key, PyObject * val) {
	TCoords c;
        if (_pytuple_to_coords_t(
                key, &c, "Expected tuple for start",
                "Expected start tuple to have 2 entries",
                "Expected start entries to be positive integers") < 0)
		return -1;
	if(c.x >= self->q_grid->width || c.y >= self->q_grid->height) {
		PyErr_SetString(PyExc_IndexError, "Expected coordinates to be inside of maze");
		return -1;
	}
	if(!Py_IS_TYPE(val, &PyMazeTile_Type)) {
		PyErr_SetString(PyExc_ValueError, "Expected to set TILE into grid");
	}

	self->q_grid->data[c.y][c.x] = ((PyMazeTile *) val)->q_val;
	return 0;
}

PyMappingMethods PyMaze_mappings = {
    .mp_subscript = (binaryfunc)PyMaze_getitem,
    .mp_ass_subscript = (objobjargproc)PyMaze_setitem,  // __setitem__
};

PyMaze * PyMaze_fill_maze(PyMaze * self, PyObject * args, PyObject * kwargs) {
	static char * kws[]  = { "seed" };
	if(!self->q_grid) {
		return NULL;
	}
	long seed = 1;
	if(!PyArg_ParseTupleAndKeywords(args, kwargs, "|l: fill_maze", kws, &seed)){
		return NULL;
	}
	fill_grid(self->q_grid, seed);
	self->q_is_generated = 1;
	Py_INCREF(self);
	return self;
}

PyMaze * PyMaze_clear_maze(PyMaze * self, PyObject * args) {
	clear_grid(self->q_grid);
	self->q_is_generated = 0;
	Py_INCREF(self);
	return self;
}

PyMaze * PyMaze_set_start_safe(PyMaze * self, PyObject * args) {
	int x, y;

	if(!PyArg_ParseTuple(args, "ii", &x, &y)) {
		return NULL;
	}

	if( x < 0 || x >= self->q_grid->width || y >= self->q_grid->height || y < 0) {
		PyErr_SetString(PyExc_IndexError, "Expected start to be inside of maze");
		return NULL;
	}

	if(self->q_is_generated && self->q_grid->data[y][x] == TILE_WALL) {
		PyErr_SetString(PyExc_AttributeError, "Setting start into wall tile. Change self.is_filled attribute or replace tile");
		return NULL;
	}

	if(self->q_grid->exit.x == x && self->q_grid->exit.y == y) {
		PyErr_SetString(PyExc_ValueError, "Start and exit cannot be on the same tile");
		return NULL;
	}
	self->q_grid->start.x = x;
	self->q_grid->start.y = y;

	Py_INCREF(self);
	return self;
}

PyMaze * PyMaze_set_exit_safe(PyMaze * self, PyObject * args) {
	int x, y;

	if(!PyArg_ParseTuple(args, "ii", &x, &y)) {
		return NULL;
	}

	if( x < 0 || x >= self->q_grid->width || y >= self->q_grid->height || y < 0) {
		PyErr_SetString(PyExc_IndexError, "Expected exit to be inside of maze");
		return NULL;
	}

	if(self->q_is_generated && self->q_grid->data[y][x] == TILE_WALL) {
		PyErr_SetString(PyExc_AttributeError, "Setting exit into wall tile. Change self.is_filled attribute or replace tile");
		return NULL;
	}

	if(self->q_grid->start.x == x && self->q_grid->start.y == y) {
		PyErr_SetString(PyExc_ValueError, "Start and exit cannot be on the same tile");
		return NULL;
	}
	self->q_grid->exit.x = x;
	self->q_grid->exit.y = y;

	Py_INCREF(self);
	return self;
}

PyObject * PyMaze_to_string(PyMaze * self, PyObject * args, PyObject * kwargs) {
	static char * kws[] = {
		"wall", "empty", NULL
	};

	int WALL = '#', EMPTY = ' ';

	if(!PyArg_ParseTupleAndKeywords(args, kwargs, "|CC:to_string", kws, &WALL, &EMPTY)) {
		return NULL;
	}

	char * str = grid_to_wall_string(self->q_grid, WALL, EMPTY);
	PyObject * ret = PyUnicode_FromStringAndSize(str, self->q_grid->height * self->q_grid->width * 9 + self->q_grid->height * 3);
	free(str);
	return ret;
}

PyMethodDef PyMaze_methods[] = {
	{"generate",   (PyCFunction) PyMaze_fill_maze,      METH_VARARGS | METH_KEYWORDS, "Fills maze with randomly generated maze"},
	{"clear_maze", (PyCFunction) PyMaze_clear_maze,     METH_NOARGS,                  "Clears maze"},
	{"set_start",  (PyCFunction) PyMaze_set_start_safe, METH_VARARGS,                 "Performs itegrity checks and sets start"},
	{"set_exit",   (PyCFunction) PyMaze_set_exit_safe,  METH_VARARGS,                 "Performs itegrity checks and sets exit"},
	{"to_string",  (PyCFunction) PyMaze_to_string,      METH_VARARGS | METH_KEYWORDS, "Serialize maze"},
	{NULL}
};

PyTypeObject PyMaze_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name            = "labyrinth_generator.Maze",
	.tp_basicsize       = sizeof(PyMaze),
	.tp_itemsize        = 0,
	.tp_dealloc         = (destructor) PyMaze_del,
	.tp_new             = PyType_GenericNew,
	.tp_init            = (initproc) PyMaze_init,
	.tp_flags           = Py_TPFLAGS_DEFAULT,
	.tp_repr            = (reprfunc) PyMaze_repr,
	.tp_str             = (reprfunc) PyMaze_str,
	.tp_methods         = PyMaze_methods,
	.tp_getset          = PyMaze_getset,
	.tp_as_mapping      = &PyMaze_mappings
};
#endif//GRID_PYTHON_CLASS_C_123S89F7DF987DG9S7DG

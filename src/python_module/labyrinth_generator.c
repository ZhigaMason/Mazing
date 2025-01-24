#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../grid/grid.h"

typedef struct {
	PyObject_HEAD;
	PtrGrid q_grid;
	bool q_is_filled;
} PyGrid;

static PyObject * _coords_t_to_pytuple(TCoords c) {
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

static int PyGrid_init(PyGrid *self, PyObject *args, PyObject *kwargs) {
	static char * kws[] = {
		"height", "width", "start", "exit", "fill_maze", "seed", NULL
	};

	int height = 10, width = 10;
	int dofill_grid = 0;
	long seed = 1;
	PyObject *start_tpl=nullptr, *exit_tpl = nullptr;

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
	self->q_is_filled = false;

	if(!self->q_grid) {
		PyErr_SetString(PyExc_MemoryError, "Memory allocation error occured.");
		return -1;
	}


	if(dofill_grid) {
		fill_grid(self->q_grid, seed);
		self->q_is_filled = true;
	}

	return 0;
}

static void PyGrid_del(PyGrid * self) {
	clean_grid(&(self->q_grid));
	Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject * PyGrid_str(PyGrid * self) {
	char * str = grid_to_wall_string(self->q_grid);
	PyObject * ret = PyUnicode_FromStringAndSize(str, self->q_grid->height * self->q_grid->width * 9 + self->q_grid->height * 3);
	free(str);
	return ret;
}

static PyObject * PyGrid_repr(PyGrid * self) {
	wchar_t * str = grid_to_path_string(self->q_grid);
	PyObject * ret = PyUnicode_FromWideChar(str, self->q_grid->height * self->q_grid->width + self->q_grid->height);
	free(str);
	return ret;
}

static PyObject * PyGrid_get_start(PyGrid * self, void * closure) {
	return _coords_t_to_pytuple(self->q_grid->start);
}

static int PyGrid_set_start(PyGrid * self, PyObject * value, void * closure) {
	if(_pytuple_to_coords_t(
		value, &self->q_grid->start, "Expected tuple for start", "Expected start tuple to have 2 entries", "Expected start entries to be positive integers"
	)) return -1;
	return 0;
}

static PyObject * PyGrid_get_exit(PyGrid * self, void * closure) {
	return _coords_t_to_pytuple(self->q_grid->exit);
}

static int PyGrid_set_exit(PyGrid * self, PyObject * value, void * closure) {
	if(_pytuple_to_coords_t(
		value, &self->q_grid->exit, "Expected tuple for exit", "Expected exit tuple to have 2 entries", "Expected exit entries to be positive integers"
	)) return -1;
	return 0;
}

static PyObject * PyGrid_get_is_filled(PyGrid * self, void * closure) {
	return PyBool_FromLong(self->q_is_filled);
}

static int PyGrid_set_is_filled(PyGrid * self, PyObject * value, void * closure) {
	if(!PyBool_Check(value)) return -1;
	self->q_is_filled = Py_IsTrue(value);
	return 0;
}

static PyGetSetDef PyGrid_getset[] = {
    {"start",      (getter)PyGrid_get_start,      (setter)PyGrid_set_start,      "Start coordinates in the maze", NULL},
    {"exit",       (getter)PyGrid_get_exit,       (setter)PyGrid_set_exit,       "Exit coordinates in the maze",  NULL},
    {"is_filled",  (getter)PyGrid_get_is_filled,  (setter)PyGrid_set_is_filled,  "Bool value if maze is filled",  NULL},
    {NULL}
};

static PyGrid * PyGrid_fill_maze(PyGrid * self, PyObject * args, PyObject * kwargs) {
	static char * kws[]  = { "seed" };
	if(!self->q_grid) {
		return nullptr;
	}
	long seed = 1;
	if(!PyArg_ParseTupleAndKeywords(args, kwargs, "|l: fill_maze", kws, &seed)){
		return nullptr;
	}
	fill_grid(self->q_grid, seed);
	self->q_is_filled = true;
	Py_INCREF(self);
	return self;
}

static PyGrid * PyGrid_clear_maze(PyGrid * self, PyObject * args) {
	clear_grid(self->q_grid);
	self->q_is_filled = false;
	Py_INCREF(self);
	return self;
}

static PyGrid * PyGrid_set_start_safe(PyGrid * self, PyObject * args) {
	int x, y;

	if(!PyArg_ParseTuple(args, "ii", &x, &y)) {
		return nullptr;
	}

	if( x < 0 || x >= self->q_grid->width || y >= self->q_grid->height || y < 0) {
		PyErr_SetString(PyExc_IndexError, "Expected start to be inside of maze");
		return nullptr;
	}

	if(self->q_is_filled && self->q_grid->data[y][x] == WALL) {
		PyErr_SetString(PyExc_AttributeError, "Setting start into wall tile. Change self.is_filled attribute or replace tile");
		return nullptr;
	}

	if(self->q_grid->exit.x == x && self->q_grid->exit.y == y) {
		PyErr_SetString(PyExc_ValueError, "Start and exit cannot be on the same tile");
		return nullptr;
	}
	self->q_grid->start.x = x;
	self->q_grid->start.y = y;

	Py_INCREF(self);
	return self;
}

static PyGrid * PyGrid_set_exit_safe(PyGrid * self, PyObject * args) {
	int x, y;

	if(!PyArg_ParseTuple(args, "ii", &x, &y)) {
		return nullptr;
	}

	if( x < 0 || x >= self->q_grid->width || y >= self->q_grid->height || y < 0) {
		PyErr_SetString(PyExc_IndexError, "Expected exit to be inside of maze");
		return nullptr;
	}

	if(self->q_is_filled && self->q_grid->data[y][x] == WALL) {
		PyErr_SetString(PyExc_AttributeError, "Setting exit into wall tile. Change self.is_filled attribute or replace tile");
		return nullptr;
	}

	if(self->q_grid->start.x == x && self->q_grid->start.y == y) {
		PyErr_SetString(PyExc_ValueError, "Start and exit cannot be on the same tile");
		return nullptr;
	}
	self->q_grid->exit.x = x;
	self->q_grid->exit.y = y;

	Py_INCREF(self);
	return self;
}

static PyMethodDef PyGrid_methods[] = {
	{"fill_maze",  (PyCFunction) PyGrid_fill_maze,      METH_VARARGS | METH_KEYWORDS, "Fills maze with randomly generated maze"},
	{"clear_maze", (PyCFunction) PyGrid_clear_maze,     METH_NOARGS,                  "Clears maze"},
	{"set_start",  (PyCFunction) PyGrid_set_start_safe, METH_VARARGS,                 "Performs itegrity checks and sets start"},
	{"set_exit",   (PyCFunction) PyGrid_set_exit_safe,  METH_VARARGS,                 "Performs itegrity checks and sets exit"},
	{NULL}
};

static PyMemberDef PyGrid_members[] = {
	{NULL}
};

static PyTypeObject grid_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name            = "labyrinth_generator.Grid",
	.tp_basicsize       = sizeof(PyGrid),
	.tp_itemsize        = 0,
	.tp_dealloc         = (destructor) PyGrid_del,
	.tp_new             = PyType_GenericNew,
	.tp_init            = (initproc) PyGrid_init,
	.tp_flags           = Py_TPFLAGS_DEFAULT,
	.tp_repr            = (reprfunc) PyGrid_repr,
	.tp_str             = (reprfunc) PyGrid_str,
	.tp_members         = PyGrid_members,
	.tp_methods         = PyGrid_methods,
	.tp_getset          = PyGrid_getset
};

static PyMethodDef labyrinth_generator_module_methods[] = {
	{NULL}
};

static PyModuleDef labyrinth_generator_module = {
	PyModuleDef_HEAD_INIT,
	.m_name    ="labyrinth_generator",
	.m_doc     ="Docstring",
	.m_size    =-1,
	.m_methods =labyrinth_generator_module_methods
};

PyMODINIT_FUNC PyInit_labyrinth_generator() {
	PyObject * module = PyModule_Create(&labyrinth_generator_module);

	if (PyType_Ready(&grid_type) < 0) {
		Py_DECREF(module);
		return nullptr;
	}

	if (PyModule_AddObject(module, "Grid", (PyObject *)&grid_type) < 0) {
		Py_DECREF(&grid_type);
		Py_DECREF(module);
		return NULL;
	}
	return module;
}

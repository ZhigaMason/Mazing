#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../grid/grid.h"

typedef struct {
	PyObject_HEAD;
	PtrGrid q_grid;
} PyGrid;

static PyObject * _coords_t_to_pytuple(TCoords c) {
	PyObject * ret = PyTuple_New(2);
	PyTuple_SET_ITEM(ret, 0, PyLong_FromLong(c.x));
	PyTuple_SET_ITEM(ret, 1, PyLong_FromLong(c.y));
	return ret;
}

static int PyGrid_init(PyGrid *self, PyObject *args, PyObject *kwargs) {
	static char * kws[] = {
		"height", "width", "start", "exit", "fill_grid", "seed", NULL
	};

	int height = 10, width = 10;
	long seed = 1;
	bool dofill_grid = false;
	PyObject *start_tpl=nullptr, *exit_tpl = nullptr;

	if(!PyArg_ParseTupleAndKeywords(args, kwargs, "|ii$OOpl: ", kws, &height, &width, &start_tpl, &exit_tpl, &dofill_grid, &seed)) {
		return -1;
	}

	if(height < 1 || width < 1) {
		PyErr_SetString(PyExc_ValueError, "Expected height and width of grid to be strictly positive.");
		return -1;
	}

	TCoords start = {.x=0, .y=0};
	TCoords exit  = {.x=width-1, .y=height-1};

	if(start_tpl) {
		if(!PyTuple_Check(start_tpl)) {
			PyErr_SetString(PyExc_ValueError, "Expected tuple for start");
			return -1;
		}

		if(PyTuple_Size(start_tpl) != 2) {
			PyErr_SetString(PyExc_ValueError, "Expected start tuple to have 2 entries");
			return -1;
		}

		int x = PyLong_AsLong(PyTuple_GET_ITEM(start_tpl, 0));
		int y = PyLong_AsLong(PyTuple_GET_ITEM(start_tpl, 1));
		if(x < 0 || y < 0) {
			PyErr_SetString(PyExc_ValueError, "Expected start entries to be positive");
			return -1;
		}
		start.x = x;
		start.y = y;
	}

	if(exit_tpl) {
		if(!PyTuple_Check(start_tpl)) {
			PyErr_SetString(PyExc_ValueError, "Expected tuple for exit");
			return -1;
		}

		if(PyTuple_Size(exit_tpl) != 2) {
			PyErr_SetString(PyExc_ValueError, "Expected exit tuple to have 2 entries");
			return -1;
		}

		int x = PyLong_AsLong(PyTuple_GET_ITEM(exit_tpl, 0));
		int y = PyLong_AsLong(PyTuple_GET_ITEM(exit_tpl, 1));
		if(x < 0 || y < 0) {
			PyErr_SetString(PyExc_ValueError, "Expected exit entries to be positive");
			return -1;
		}
		exit.x = x;
		exit.y = y;
	}

	if(exit.x == start.x && exit.y == start.y) {
		PyErr_SetString(PyExc_ValueError, "Start and finish cannot be on the same tile");
		return -1;
	}


	self->q_grid = make_grid(
		height, width,
		start, exit
	);

	if(!self->q_grid) {
		PyErr_SetString(PyExc_MemoryError, "Memory allocation error occured.");
		return -1;
	}


	if(dofill_grid)
		fill_grid(self->q_grid, seed);

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
	if(!ret) {
		PyErr_Print();
	}
	return ret;
}

static PyObject * PyGrid_repr(PyGrid * self) {
	wchar_t * str = grid_to_path_string(self->q_grid);
	PyObject * ret = PyUnicode_FromWideChar(str, self->q_grid->height * self->q_grid->width + self->q_grid->height);
	free(str);
	if(!ret) {
		PyErr_Print();
	}
	return ret;
}

static PyGrid * fill_maze(PyGrid * self, PyObject * args, PyObject * kwargs) {
	static char * kws[]  = { "seed" };
	if(!self->q_grid) {
		return nullptr;
	}
	long seed = 1;
	if(!PyArg_ParseTupleAndKeywords(args, kwargs, "|l: fill_maze", kws, &seed)){
		PyErr_Print();
	}
	fill_grid(self->q_grid, seed);
	Py_INCREF(self);
	return self;
}
static PyMethodDef PyGrid_methods[] = {
	{"fill_maze",  (PyCFunction) fill_maze, METH_VARARGS | METH_KEYWORDS, "Fills maze with randomly generated maze"},
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

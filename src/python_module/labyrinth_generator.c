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
		"height", "width", "start", "exit", NULL
	};

	int height, width;
	PyObject *start_tpl=nullptr, *exit_tpl = nullptr;

	if(!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|$OO: ", kws, &height, &width, &start_tpl, &exit_tpl)) {
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

	return 0;
}

static void PyGrid_del(PyGrid * self) {
	clean_grid(&(self->q_grid));
	Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyMethodDef PyGrid_methods[] = {
	{NULL}
};

static PyMemberDef PyGrid_members[] = {
	{NULL}
};

static PyTypeObject grid_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "labyrinth_generator.Grid",
	.tp_basicsize = sizeof(PyGrid),
	.tp_itemsize = 0,
	.tp_dealloc = (destructor) PyGrid_del,
	.tp_new = PyType_GenericNew,
	.tp_init = (initproc) PyGrid_init,
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_members = PyGrid_members,
	.tp_methods = PyGrid_methods,
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

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "./grid_class/grid_class.h"
#include "./tile_class/tile_class.h"

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
		goto clean_module;
	}

	if (PyModule_AddObject(module, "Grid", (PyObject *)&grid_type) < 0) {
		goto clean_grid_type;
	}

	PyGridTile_initialize_class();
	if (PyModule_AddObject(module, "TILE", (PyObject *)&tile_type) < 0) {
		goto clean_tile_type;
	}

	return module;

clean_tile_type:;
	printf("f4'\n");
	Py_DECREF(&tile_type);
clean_grid_type:;
	printf("f5'\n");
	Py_DECREF(&grid_type);
clean_module:;
	printf("f6'\n");
	Py_DECREF(module);
	return NULL;
}

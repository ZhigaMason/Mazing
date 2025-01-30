#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "./maze_class/maze_class.h"
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

PyMODINIT_FUNC PyInit_labyrinth_generator(void) {
	PyObject * module = PyModule_Create(&labyrinth_generator_module);

	if (PyType_Ready(&PyMaze_Type) < 0) {
		goto clean_module;
	}

	if (PyModule_AddObject(module, "Maze", (PyObject *)&PyMaze_Type) < 0) {
		goto clean_grid_type;
	}

	PyMazeTile_initialize_class();
	if (PyModule_AddObject(module, "TILE", (PyObject *)&PyMazeTile_Type) < 0) {
		goto clean_tile_type;
	}

	return module;

clean_tile_type:;
	printf("f4'\n");
	Py_DECREF(&PyMazeTile_Type);
clean_grid_type:;
	printf("f5'\n");
	Py_DECREF(&PyMaze_Type);
clean_module:;
	printf("f6'\n");
	Py_DECREF(module);
	return NULL;
}

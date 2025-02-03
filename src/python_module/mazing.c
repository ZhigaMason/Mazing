#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "./maze_class/maze_class.h"
#include "./tile_class/tile_class.h"


static PyModuleDef labyrinth_generator_module = {
	PyModuleDef_HEAD_INIT,
	.m_name    ="mazing",
	.m_doc     ="Docstring",
	.m_size    =-1,
	.m_methods =NULL
};

PyMODINIT_FUNC PyInit_mazing(void) {
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
	Py_DECREF(&PyMazeTile_Type);
clean_grid_type:;
	Py_DECREF(&PyMaze_Type);
clean_module:;
	Py_DECREF(module);
	return NULL;
}

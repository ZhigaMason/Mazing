#ifndef TILE_PYTHON_CLASS_C_1837678126381263812763
#define TILE_PYTHON_CLASS_C_1837678126381263812763
#include <Python.h>
#include "./tile_class.h"

PyTypeObject PyMazeTile_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name            = "labyrinth_generator.Tile",
	.tp_basicsize       = sizeof(PyMazeTile),
	.tp_dealloc         = (destructor) PyMazeTile_del,
	.tp_new             = 0,
	.tp_init            = 0,
	.tp_flags           = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_DISALLOW_INSTANTIATION,
	.tp_repr            = (reprfunc) PyMazeTile_repr,
	.tp_str             = (reprfunc) PyMazeTile_repr,
	.tp_doc             = "Tile grid class with constant tiles"
};

PyObject * _PyTile_Objects[16] = {0};

PyObject * PyMazeTile_repr(PyMazeTile * self) {
	static const wchar_t char_table[] = { L' ', L'╸', L'╺', L'━', L'╹', L'┛', L'┗', L'┻', L'╻', L'┓', L'┏', L'┳', L'┃', L'┫', L'┣', L'╋'};
	if(self->q_val >= 16) {
		PyErr_SetString(PyExc_AttributeError, "Unexpected tile representation");
		return nullptr;
	}
	return PyUnicode_FromWideChar(&char_table[self->q_val], 1);
}

void PyMazeTile_del(PyMazeTile * self) {
	Py_TYPE(self)->tp_free((PyObject *) self);
}


void PyMazeTile_initialize_class() {
	if(!PyMazeTile_Type.tp_dict) PyMazeTile_Type.tp_dict = PyDict_New();

	if(PyType_Ready(&PyMazeTile_Type) < 0) {
		return;
	}

        struct name_val {
		const char *name;
		ETile val;
        };

	static struct name_val arr[16] = {
		{"EMPTY",         TILE_EMPTY},
		{"UP",            TILE_UP},
		{"DOWN",          TILE_DOWN},
		{"LEFT",          TILE_LEFT},
		{"RIGHT",         TILE_RIGHT},
		{"HOR",           TILE_LEFT  | TILE_RIGHT},
		{"VER",	          TILE_UP    | TILE_DOWN},
		{"LEFT_UP",       TILE_LEFT  | TILE_UP},
		{"LEFT_DOWN",     TILE_LEFT  | TILE_DOWN},
		{"RIGHT_UP",      TILE_RIGHT | TILE_UP},
		{"RIGHT_DOWN",    TILE_RIGHT | TILE_DOWN},
		{"WALL_LEFT",     TILE_EMPTY - TILE_LEFT},
		{"WALL_RIGHT",    TILE_EMPTY - TILE_RIGHT},
		{"WALL_UP",       TILE_EMPTY - TILE_UP},
		{"WALL_DOWN",     TILE_EMPTY - TILE_DOWN},
		{"WALL",          TILE_WALL},
	};

	for(struct name_val * it = arr; it != (arr + 16); ++it) {
		PyMazeTile * tile = PyObject_New(PyMazeTile, &PyMazeTile_Type);

		if(tile) {
			tile->q_val = it->val;
			PyDict_SetItemString(PyMazeTile_Type.tp_dict, it->name, (PyObject *) tile);
			_PyTile_Objects[it->val] = (PyObject *) tile;
			Py_DECREF(tile);
		}
	}

}
#endif//TILE_PYTHON_CLASS_C_1837678126381263812763

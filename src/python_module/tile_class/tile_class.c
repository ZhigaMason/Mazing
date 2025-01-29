#ifndef TILE_PYTHON_CLASS_C_1837678126381263812763
#define TILE_PYTHON_CLASS_C_1837678126381263812763
#include <Python.h>
#include "./tile_class.h"

PyTypeObject tile_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name            = "labyrinth_generator.Tile",
	.tp_basicsize       = sizeof(PyGridTile),
	.tp_dealloc         = (destructor) PyGridTile_del,
	.tp_new             = 0,
	.tp_init            = 0,
	.tp_flags           = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_DISALLOW_INSTANTIATION,
	.tp_repr            = (reprfunc) PyGridTile_repr,
	.tp_str             = (reprfunc) PyGridTile_repr,
	.tp_doc             = "Tile grid class with constant tiles"
};

PyObject * PyGridTile_repr(PyGridTile * self) {
	static const wchar_t char_table[] = { L' ', L'╸', L'╺', L'━', L'╹', L'┛', L'┗', L'┻', L'╻', L'┓', L'┏', L'┳', L'┃', L'┫', L'┣', L'╋'};
	if(self->q_val >= 16) {
		PyErr_SetString(PyExc_AttributeError, "Unexpected tile representation");
		return nullptr;
	}
	return PyUnicode_FromWideChar(&char_table[self->q_val], 1);
}

void PyGridTile_del(PyGridTile * self) {
	Py_TYPE(self)->tp_free((PyObject *) self);
}


void PyGridTile_initialize_class() {
	if(!tile_type.tp_dict) tile_type.tp_dict = PyDict_New();

	if(PyType_Ready(&tile_type) < 0) {
		printf("L'achtung\n");
		return;
	}

        struct name_val {
		const char *name;
		ETile val;
        };

	static struct name_val arr[16] = {
		{"EMPTY",         EMPTY},
		{"UP",            UP},
		{"DOWN",          DOWN},
		{"LEFT",          LEFT},
		{"RIGHT",         RIGHT},
		{"HOR",           LEFT | RIGHT},
		{"VER",	          UP | DOWN},
		{"LEFT_UP",       LEFT | UP},
		{"LEFT_DOWN",     LEFT | DOWN},
		{"RIGHT_UP",      RIGHT | UP},
		{"RIGHT_DOWN",    RIGHT | DOWN},
		{"WALL_LEFT",     EMPTY - LEFT},
		{"WALL_RIGHT",    EMPTY - RIGHT},
		{"WALL_UP",       EMPTY - UP},
		{"WALL_DOWN",     EMPTY - DOWN},
		{"WALL",          WALL},
	};

	for(struct name_val * it = arr; it != (arr + 16); ++it) {
		PyGridTile * tile = PyObject_New(PyGridTile, &tile_type);

		if(tile) {
			tile->q_val = it->val;
			PyDict_SetItemString(tile_type.tp_dict, it->name, (PyObject *) tile);
			Py_DECREF(tile);
		}
	}

}
#endif//TILE_PYTHON_CLASS_C_1837678126381263812763

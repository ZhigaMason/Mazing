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

	PyGridTile * right_up_tile = PyObject_New(PyGridTile, &tile_type);

	if(right_up_tile) {
		right_up_tile->q_val = RIGHTUP;
		PyDict_SetItemString(tile_type.tp_dict, "RIGHT_UP", (PyObject *) right_up_tile);
		Py_DECREF(right_up_tile);
	}

}
#endif//TILE_PYTHON_CLASS_C_1837678126381263812763

/*
 * This file is a part of the psdl package.
 * Copyright (C) 2021 Ankith (ankith26)
 *
 * Distributed under the MIT license.
 */

#include "include/psdl.h"

#error "TODO"

static PyObject *
psdl_get_clipboard_text(PyObject *self)
{
    PyObject *ret;
    char *temp = SDL_GetClipboardText();
    if (!temp)
        return PSDL_RAISE_SDL_ERROR;

    ret = PyUnicode_FromString(temp);
    SDL_free(temp);
    return ret;
}

static PyObject *
psdl_has_clipboard_text(PyObject *self)
{
    return PyBool_FromLong(SDL_HasClipboardText());
}

static PyObject *
psdl_set_clipboard_text(PyObject *self, PyObject *args)
{
    char *s;
    if (!PyArg_ParseTuple(args, "s", &s))
        return NULL;

    if (SDL_SetClipboardText(s))
        return PSDL_RAISE_SDL_ERROR;
    Py_RETURN_NONE;
}

static PyMethodDef psdl_timer_methods[] = {
    {"get_clipboard_text", (PyCFunction)psdl_get_clipboard_text,
        METH_NOARGS, DOC_PLACEHOLDER},
    {"has_clipboard_text", (PyCFunction)psdl_has_clipboard_text,
        METH_NOARGS, DOC_PLACEHOLDER},
    {"set_clipboard_text", psdl_set_clipboard_text,
        METH_VARARGS, DOC_PLACEHOLDER},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef psdl_timer_module = {
    PyModuleDef_HEAD_INIT,
    "psdl.timer",
    DOC_PLACEHOLDER,
    -1,
    psdl_timer_methods,
};

PyMODINIT_FUNC
PyInit_timer(void)
{
    PyObject *mod = PyModule_Create(&psdl_timer_module);
    if (!mod)
        return NULL;

    PSDL_INCLUDE_SDL_ERROR;
    return mod;
}

/*
 * This file is a part of the psdl package.
 * Copyright (C) 2021 Ankith (ankith26)
 *
 * Distributed under the MIT license.
 */

#include "include/psdl.h"

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

PSDL_BEGIN_METH_DEF
    PSDL_EXPORT_FUNC(get_clipboard_text, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_clipboard_text, METH_NOARGS)
    PSDL_EXPORT_FUNC(set_clipboard_text, METH_VARARGS)
PSDL_END_METH_DEF
PSDL_MOD_INIT(clipboard)

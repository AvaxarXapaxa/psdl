/*
 * This file is a part of the psdl package.
 * Copyright (C) 2021 Ankith (ankith26)
 *
 * Distributed under the MIT license.
 */

#include "include/psdl.h"

static void
psdlWindow_dealloc(PyObject *self)
{
    SDL_DestroyWindow(((psdlWindow *)self)->win);
    PyObject_Del(self);
}

static PyTypeObject psdlWindow_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "psdl.Window",
    .tp_doc = DOC_PLACEHOLDER,
    .tp_basicsize = sizeof(psdlWindow),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = psdlWindow_dealloc,
};

static PyObject *
_psdl_window_from_window(SDL_Window *window)
{
    psdlWindow *ret;
    if (!window)
        return PSDL_RAISE_SDL_ERROR;

    ret = PyObject_New(psdlWindow, &psdlWindow_Type);
    if (!ret)
        return PyErr_NoMemory();

    ret->win = window;
    return (PyObject *)ret;
}

static PyObject *
psdl_create_window(PyObject *self, PyObject *args)
{
    const char *t;
    int x, y, w, h, f;

    if (!PyArg_ParseTuple(args, "siiiii", &t, &x, &y, &w, &h, &f))
        return NULL;
    return _psdl_window_from_window(SDL_CreateWindow(t, x, y, w, h, f));
}

static PyObject *
psdl_create_window_and_renderer(PyObject *self, PyObject *args)
{
    int w, h, f;
    SDL_Window *win;
    SDL_Renderer *ren;

    if (!PyArg_ParseTuple(args, "iii", &w, &h, &f))
        return NULL;

    if (SDL_CreateWindowAndRenderer(w, h, f, &win, &ren))
        return PSDL_RAISE_SDL_ERROR;

    /* TODO: Return the renderer as well in the tuple */
    return Py_BuildValue("(N)", _psdl_window_from_window(win));
}

/* See what to do about SDL_CreateWindowFrom */

static PyObject *
psdl_disable_screensaver(PyObject *self)
{
    SDL_DisableScreenSaver();
    Py_RETURN_NONE;
}

static PyObject *
psdl_enable_screensaver(PyObject *self)
{
    SDL_EnableScreenSaver();
    Py_RETURN_NONE;
}

/* TODO: GL stuff */

/* TODO THIS MODULE IS INCOMPLETE */

PSDL_BEGIN_METH_DEF
    PSDL_EXPORT_FUNC(create_window, METH_VARARGS)
    PSDL_EXPORT_FUNC(create_window_and_renderer, METH_VARARGS)
    PSDL_EXPORT_FUNC(disable_screensaver, METH_NOARGS)
    PSDL_EXPORT_FUNC(enable_screensaver, METH_NOARGS)
PSDL_END_METH_DEF

PSDL_BEGIN_MODINIT(video)
    PSDL_EXPORT_CLASS(Window)
PSDL_END_MODINIT

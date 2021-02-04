/*
 * This file is a part of the psdl package.
 * Copyright (C) 2021 Ankith (ankith26)
 *
 * Distributed under the MIT license.
 */
#define PSDL_SDL_MODULE
#include "include/psdl.h"

static PyObject *
psdl_init(PyObject *self, PyObject *args)
{
    int flags;
    if (!PyArg_ParseTuple(args, "i", &flags))
        return NULL;

    if (flags < 0)
        return PSDL_RAISE_MSG(PyExc_ValueError, "flags must be non-negative");

    if (SDL_Init((Uint32)flags) < 0)
        return PSDL_RAISE_SDL_ERROR;
    Py_RETURN_NONE;
}

static PyObject *
psdl_init_subsystem(PyObject *self, PyObject *args)
{
    int flags;
    if (!PyArg_ParseTuple(args, "i", &flags))
        return NULL;

    if (flags < 0)
        return PSDL_RAISE_MSG(PyExc_ValueError, "flags must be non-negative");

    if (SDL_InitSubSystem((Uint32)flags) < 0)
        return PSDL_RAISE_SDL_ERROR;
    Py_RETURN_NONE;
}

static PyObject *
psdl_quit(PyObject *self)
{
    SDL_Quit();
    Py_RETURN_NONE;
}

static PyObject *
psdl_quit_subsystem(PyObject *self, PyObject *args)
{
    int flags;
    if (!PyArg_ParseTuple(args, "i", &flags))
        return NULL;

    if (flags < 0)
        return PSDL_RAISE_MSG(PyExc_ValueError, "flags must be non-negative");

    SDL_QuitSubSystem((Uint32)flags);
    Py_RETURN_NONE;
}

static PyObject *
psdl_was_init(PyObject *self, PyObject *args)
{
    int flags;
    if (!PyArg_ParseTuple(args, "i", &flags))
        return NULL;

    return PyLong_FromLong(SDL_WasInit((Uint32)flags));
}

PSDL_BEGIN_METH_DEF
    PSDL_EXPORT_FUNC(init, METH_VARARGS)
    PSDL_EXPORT_FUNC(init_subsystem, METH_VARARGS)
    PSDL_EXPORT_FUNC(quit, METH_NOARGS)
    PSDL_EXPORT_FUNC(quit_subsystem, METH_VARARGS)
    PSDL_EXPORT_FUNC(was_init, METH_VARARGS)
PSDL_END_METH_DEF

PSDL_BEGIN_MODINIT(sdl)
    PSDL_EXPORT_SDL_CONST(INIT_TIMER)
    PSDL_EXPORT_SDL_CONST(INIT_AUDIO)
    PSDL_EXPORT_SDL_CONST(INIT_VIDEO)
    PSDL_EXPORT_SDL_CONST(INIT_JOYSTICK)
    PSDL_EXPORT_SDL_CONST(INIT_HAPTIC)
    PSDL_EXPORT_SDL_CONST(INIT_GAMECONTROLLER)
    PSDL_EXPORT_SDL_CONST(INIT_EVENTS)
    PSDL_EXPORT_SDL_CONST(INIT_EVERYTHING)

    psdlExc_SDLError = PyErr_NewException("psdl.error",
                                            PyExc_RuntimeError, NULL);
    Py_XINCREF(psdlExc_SDLError);
    if (PyModule_AddObject(mod, "error", psdlExc_SDLError) < 0) {
        Py_XDECREF(psdlExc_SDLError);
        Py_DECREF(mod);
        return NULL;
    }
PSDL_END_MODINIT

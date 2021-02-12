/*
 * This file is a part of the psdl package.
 * Copyright (C) 2021 Ankith (ankith26)
 *
 * Distributed under the MIT license.
 */
#ifndef _PSDL_H
#define _PSDL_H

/* Support Py_LIMITED_API, support python >= 3.5 for now */

/* There appear to be issues with Py_LIMITED_API and TypeObjects, so
 * dont use Py_LIMITED_API */
//#define Py_LIMITED_API 0x03050000

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <SDL.h>
#include "objects.h"

#define DOC_PLACEHOLDER "Docs coming soon!"

#define PSDL_RAISE_MSG(x, y) (PyErr_SetString((x), (y)), (PyObject *)NULL)
#define PSDL_RAISE(x) (PyErr_SetNone(x), (PyObject *)NULL)

#ifdef PSDL_NO_SDL_ERROR
#define _PSDL_INCLUDE_SDL_ERROR
#else /* ~PSDL_NO_SDL_ERROR */

static PyObject *psdlExc_SDLError = NULL;
#define PSDL_RAISE_SDL_ERROR PSDL_RAISE_MSG(psdlExc_SDLError, SDL_GetError())

#ifdef PSDL_SDL_MODULE
#define _PSDL_INCLUDE_SDL_ERROR
#else /* ~PSDL_SDL_MODULE */
#define _PSDL_INCLUDE_SDL_ERROR                                  \
    PyObject *_module = PyImport_ImportModule("psdl.sdl");       \
    if (!_module)                                                \
        return NULL;                                             \
    psdlExc_SDLError = PyObject_GetAttrString(_module, "error"); \
    Py_DECREF(_module);                                          \
    if (!psdlExc_SDLError)                                       \
        return NULL;

#endif /* ~PSDL_SDL_MODULE */
#endif /* ~PSDL_NO_SDL_ERROR */

#define PSDL_EXPORT_SDL_CONST(con)                       \
    if (PyModule_AddIntConstant(mod, #con, SDL_##con)) { \
        Py_DECREF(mod);                                  \
        return NULL;                                     \
    }

#define PSDL_TYPE_READY(t) \
    if (PyType_Ready(t)) { \
        Py_DECREF(mod);    \
        return NULL;       \
    }

#define PSDL_EXPORT_CLASS(t)                                        \
    PSDL_TYPE_READY(&psdl##t##_Type)                                \
    Py_INCREF(&psdl##t##_Type);                                     \
    if (PyModule_AddObject(mod, #t, (PyObject *)&psdl##t##_Type)) { \
        Py_DECREF(&psdl##t##_Type);                                 \
        Py_DECREF(mod);                                             \
        return NULL;                                                \
    }

#define PSDL_EXPORT_FUNC(f, t) {#f, (PyCFunction)psdl_##f, t, DOC_PLACEHOLDER},

#define PSDL_BEGIN_METH_DEF static PyMethodDef _module_methods[] = {
#define PSDL_END_METH_DEF {NULL, NULL, 0, NULL}};

#define _PSDL_MOD_DEF                     \
static struct PyModuleDef _module_def = { \
    PyModuleDef_HEAD_INIT,                \
    PSDL_MOD_NAME,                        \
    DOC_PLACEHOLDER,                      \
    -1,                                   \
    _module_methods,                      \
};

#define PSDL_BEGIN_MODINIT(modname)      \
_PSDL_MOD_DEF                            \
PyMODINIT_FUNC                           \
PyInit_##modname(void)                   \
{                                        \
    PyObject *mod;                       \
    _PSDL_INCLUDE_SDL_ERROR              \
    mod = PyModule_Create(&_module_def); \
    if (!mod) return NULL;

#define PSDL_END_MODINIT return mod; }

#define PSDL_MOD_INIT(modname)            \
_PSDL_MOD_DEF                             \
PyMODINIT_FUNC                            \
PyInit_##modname(void)                    \
{                                         \
    _PSDL_INCLUDE_SDL_ERROR               \
    return PyModule_Create(&_module_def); \
}

/* Compare whether a Python string and C string are equal or not */
#define PSDL_PY_C_STREQ(a, b) !PyUnicode_CompareWithASCIIString((a), (b))

#endif /* _PSDL_H */

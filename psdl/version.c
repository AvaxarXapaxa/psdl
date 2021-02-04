/*
 * This file is a part of the psdl package.
 * Copyright (C) 2021 Ankith (ankith26)
 *
 * Distributed under the MIT license.
 */

#define PSDL_NO_SDL_ERROR
#include "include/psdl.h"
#include <SDL_revision.h>


static PyObject *
_tuple_from_version(SDL_version v)
{
    return Py_BuildValue("iii", v.major, v.minor, v.patch);
}

static PyObject *
psdl_version(PyObject *self)
{
    SDL_version ver;
    SDL_VERSION(&ver);
    return _tuple_from_version(ver);
}

static PyObject *
psdl_get_version(PyObject *self)
{
    SDL_version ver;
    SDL_GetVersion(&ver);
    return _tuple_from_version(ver);
}

static PyObject *
psdl_get_revision(PyObject *self)
{
    return PyUnicode_FromString(SDL_GetRevision());
}

static PyObject *
psdl_get_revision_number(PyObject *self)
{
    return PyLong_FromLong(SDL_GetRevisionNumber());
}

static PyObject *
psdl_revision(PyObject *self)
{
    return PyUnicode_FromString(SDL_REVISION);
}

static PyObject *
psdl_version_atleast(PyObject *self, PyObject *args)
{
    int a, b, c;
    if (!PyArg_ParseTuple(args, "iii", &a, &b, &c))
        return NULL;

    return PyBool_FromLong(SDL_VERSION_ATLEAST(a, b, c));
}

PSDL_BEGIN_METH_DEF
    PSDL_EXPORT_FUNC(version, METH_NOARGS)
    PSDL_EXPORT_FUNC(get_version, METH_NOARGS)
    PSDL_EXPORT_FUNC(get_revision, METH_NOARGS)
    PSDL_EXPORT_FUNC(get_revision_number, METH_NOARGS)
    PSDL_EXPORT_FUNC(revision, METH_NOARGS)
    PSDL_EXPORT_FUNC(version_atleast, METH_VARARGS)
PSDL_END_METH_DEF
PSDL_MOD_INIT(version)

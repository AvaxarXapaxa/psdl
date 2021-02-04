/*
 * This file is a part of the psdl package.
 * Copyright (C) 2021 Ankith (ankith26)
 *
 * Distributed under the MIT license.
 */

#define PSDL_NO_SDL_ERROR
#include "include/psdl.h"

static PyObject *
psdl_get_platform(PyObject *self)
{
    return PyUnicode_FromString(SDL_GetPlatform());
}

static PyObject *
psdl_get_CPU_cacheline_size(PyObject *self)
{
    return PyLong_FromLong(SDL_GetCPUCacheLineSize());
}

static PyObject *
psdl_get_CPU_count(PyObject *self)
{
    return PyLong_FromLong(SDL_GetCPUCount());
}

static PyObject *
psdl_get_system_RAM(PyObject *self)
{
#if SDL_VERSION_ATLEAST(2, 0, 1)
    return PyLong_FromLong(SDL_GetSystemRAM());
#else
    return PSDL_RAISE_MSG(PyExc_NotImplementedError,
        "This function needs SDL version 2.0.1 or above");
#endif
}

static PyObject *
psdl_has_3DNow(PyObject *self)
{
    return PyBool_FromLong(SDL_Has3DNow());
}

static PyObject *
psdl_has_AVX(PyObject *self)
{
#if SDL_VERSION_ATLEAST(2, 0, 2)
    return PyBool_FromLong(SDL_HasAVX());
#else
    return PSDL_RAISE_MSG(PyExc_NotImplementedError,
        "This function needs SDL version 2.0.2 or above");
#endif
}

static PyObject *
psdl_has_AVX2(PyObject *self)
{
#if SDL_VERSION_ATLEAST(2, 0, 4)
    return PyBool_FromLong(SDL_HasAVX2());
#else
    return PSDL_RAISE_MSG(PyExc_NotImplementedError,
        "This function needs SDL version 2.0.4 or above");
#endif
}

static PyObject *
psdl_has_AltiVec(PyObject *self)
{
    return PyBool_FromLong(SDL_HasAltiVec());
}

static PyObject *
psdl_has_MMX(PyObject *self)
{
    return PyBool_FromLong(SDL_HasMMX());
}

static PyObject *
psdl_has_RDTSC(PyObject *self)
{
    return PyBool_FromLong(SDL_HasRDTSC());
}

static PyObject *
psdl_has_SSE(PyObject *self)
{
    return PyBool_FromLong(SDL_HasSSE());
}

static PyObject *
psdl_has_SSE2(PyObject *self)
{
    return PyBool_FromLong(SDL_HasSSE2());
}

static PyObject *
psdl_has_SSE3(PyObject *self)
{
    return PyBool_FromLong(SDL_HasSSE3());
}

static PyObject *
psdl_has_SSE41(PyObject *self)
{
    return PyBool_FromLong(SDL_HasSSE41());
}

static PyObject *
psdl_has_SSE42(PyObject *self)
{
    return PyBool_FromLong(SDL_HasSSE42());
}

static PyObject *
psdl_get_power_info(PyObject *self)
{
    int sec, per;
    SDL_PowerState pstate = SDL_GetPowerInfo(&sec, &per);
    return Py_BuildValue("(iii)", pstate, sec, per);
}

PSDL_BEGIN_METH_DEF
    PSDL_EXPORT_FUNC(get_platform, METH_NOARGS)
    PSDL_EXPORT_FUNC(get_CPU_cacheline_size, METH_NOARGS)
    PSDL_EXPORT_FUNC(get_CPU_count, METH_NOARGS)
    PSDL_EXPORT_FUNC(get_system_RAM, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_3DNow, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_AVX, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_AVX2, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_AVX2, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_AltiVec, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_MMX, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_RDTSC, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_SSE, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_SSE2, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_SSE3, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_SSE41, METH_NOARGS)
    PSDL_EXPORT_FUNC(has_SSE42, METH_NOARGS)
    PSDL_EXPORT_FUNC(get_power_info, METH_NOARGS)
PSDL_END_METH_DEF

PSDL_BEGIN_MODINIT(cpuinfo)
    PSDL_EXPORT_SDL_CONST(POWERSTATE_UNKNOWN)
    PSDL_EXPORT_SDL_CONST(POWERSTATE_ON_BATTERY)
    PSDL_EXPORT_SDL_CONST(POWERSTATE_NO_BATTERY)
    PSDL_EXPORT_SDL_CONST(POWERSTATE_CHARGING)
    PSDL_EXPORT_SDL_CONST(POWERSTATE_CHARGED)
PSDL_END_MODINIT

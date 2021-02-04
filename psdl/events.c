/*
 * This file is a part of the psdl package.
 * Copyright (C) 2021 Ankith (ankith26)
 *
 * Distributed under the MIT license.
 */

#include "include/psdl.h"

static PyObject *eventwatchlist;

#define PSDL_GET_EVENT(a) ((psdlEvent *)a)->ev
#define PSDL_EVENT_CHECK(a) PyObject_TypeCheck(a, &psdlEvent_Type)

/* For properly handing the memory stuff */
static void
_sdl_event_new_copy(SDL_Event *ev)
{
    switch (ev->type) {
        case SDL_DROPFILE:
#if SDL_VERSION_ATLEAST(2, 0, 5)
        case SDL_DROPBEGIN:
        case SDL_DROPCOMPLETE:
        case SDL_DROPTEXT:
#endif
            ev->drop.file = SDL_strdup(ev->drop.file);
            break;
        case SDL_USEREVENT:
            Py_XINCREF((PyObject *)ev->user.data1);
            Py_XINCREF((PyObject *)ev->user.data2);
    }
}

static void
psdlEvent_dealloc(PyObject *self)
{
    SDL_Event *ev = &PSDL_GET_EVENT(self);
    switch (ev->type) {
        case SDL_DROPFILE:
#if SDL_VERSION_ATLEAST(2, 0, 5)
        case SDL_DROPBEGIN:
        case SDL_DROPCOMPLETE:
        case SDL_DROPTEXT:
#endif
            SDL_free(ev->drop.file);
            break;
        case SDL_USEREVENT:
            Py_XDECREF((PyObject *)ev->user.data1);
            Py_XDECREF((PyObject *)ev->user.data2);
    }
    PyObject_Del(self);
}

static PyObject *
psdlEvent_getattro(PyObject *self, PyObject *uni)
{
    SDL_Event e = PSDL_GET_EVENT(self);

    if (!PyUnicode_Check(uni))
        return PSDL_RAISE_MSG(PyExc_ValueError, "argument must be str");

    if (PSDL_PY_C_STREQ(uni, "type"))
        return PyLong_FromLong(e.type);

    switch (e.type) {
#if SDL_VERSION_ATLEAST(2, 0, 4)
        case SDL_AUDIODEVICEADDED:
        case SDL_AUDIODEVICEREMOVED:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.adevice.timestamp);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.adevice.which);
            if (PSDL_PY_C_STREQ(uni, "iscapture"))
                return PyBool_FromLong(e.adevice.iscapture);
            break;
#endif
        case SDL_CONTROLLERAXISMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.caxis.timestamp);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.caxis.which);
            if (PSDL_PY_C_STREQ(uni, "axis"))
                return PyLong_FromLong(e.caxis.axis);
            if (PSDL_PY_C_STREQ(uni, "value"))
                return PyLong_FromLong(e.caxis.value);
            break;
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.cbutton.timestamp);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.cbutton.which);
            if (PSDL_PY_C_STREQ(uni, "button"))
                return PyLong_FromLong(e.cbutton.button);
            if (PSDL_PY_C_STREQ(uni, "state"))
                return PyLong_FromLong(e.cbutton.state);
            break;
        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
        case SDL_CONTROLLERDEVICEREMAPPED:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.cdevice.timestamp);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.cdevice.which);
            break;
        case SDL_DOLLARGESTURE:
        case SDL_DOLLARRECORD:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.dgesture.timestamp);
            if (PSDL_PY_C_STREQ(uni, "touch_id"))
                return PyLong_FromLong(e.dgesture.touchId);
            if (PSDL_PY_C_STREQ(uni, "gesture_id"))
                return PyLong_FromLong(e.dgesture.gestureId);
            if (PSDL_PY_C_STREQ(uni, "num_fingers"))
                return PyLong_FromLong(e.dgesture.numFingers);
            if (PSDL_PY_C_STREQ(uni, "error"))
                return PyFloat_FromDouble(e.dgesture.error);
            if (PSDL_PY_C_STREQ(uni, "x"))
                return PyFloat_FromDouble(e.dgesture.x);
            if (PSDL_PY_C_STREQ(uni, "y"))
                return PyFloat_FromDouble(e.dgesture.y);
            break;
        case SDL_DROPFILE:
#if SDL_VERSION_ATLEAST(2, 0, 5)
        case SDL_DROPBEGIN:
        case SDL_DROPCOMPLETE:
        case SDL_DROPTEXT:
#endif
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.drop.timestamp);
            if (PSDL_PY_C_STREQ(uni, "file"))
                return PyUnicode_FromString(e.drop.file);
            if (PSDL_PY_C_STREQ(uni, "window_id"))
                return PyLong_FromLong(e.drop.windowID);
            break;
        case SDL_FINGERMOTION:
        case SDL_FINGERUP:
        case SDL_FINGERDOWN:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.tfinger.timestamp);
            if (PSDL_PY_C_STREQ(uni, "touch_id"))
                return PyLong_FromLong(e.tfinger.touchId);
            if (PSDL_PY_C_STREQ(uni, "finger_id"))
                return PyLong_FromLong(e.tfinger.fingerId);
            if (PSDL_PY_C_STREQ(uni, "x"))
                return PyFloat_FromDouble(e.tfinger.x);
            if (PSDL_PY_C_STREQ(uni, "y"))
                return PyFloat_FromDouble(e.tfinger.y);
            if (PSDL_PY_C_STREQ(uni, "dx"))
                return PyFloat_FromDouble(e.tfinger.dx);
            if (PSDL_PY_C_STREQ(uni, "dy"))
                return PyFloat_FromDouble(e.tfinger.dy);
            if (PSDL_PY_C_STREQ(uni, "pressure"))
                return PyFloat_FromDouble(e.tfinger.pressure);
            break;
        case SDL_JOYAXISMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.jaxis.timestamp);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.jaxis.which);
            if (PSDL_PY_C_STREQ(uni, "axis"))
                return PyLong_FromLong(e.jaxis.axis);
            if (PSDL_PY_C_STREQ(uni, "value"))
                return PyLong_FromLong(e.jaxis.value);
            break;
        case SDL_JOYBALLMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.jball.timestamp);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.jball.which);
            if (PSDL_PY_C_STREQ(uni, "ball"))
                return PyLong_FromLong(e.jball.ball);
            if (PSDL_PY_C_STREQ(uni, "xrel"))
                return PyLong_FromLong(e.jball.xrel);
            if (PSDL_PY_C_STREQ(uni, "yrel"))
                return PyLong_FromLong(e.jball.yrel);
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.jbutton.timestamp);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.jbutton.which);
            if (PSDL_PY_C_STREQ(uni, "button"))
                return PyLong_FromLong(e.jbutton.button);
            if (PSDL_PY_C_STREQ(uni, "state"))
                return PyLong_FromLong(e.jbutton.state);
            break;
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.jdevice.timestamp);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.jdevice.which);
            break;
        case SDL_JOYHATMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.jhat.timestamp);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.jhat.which);
            if (PSDL_PY_C_STREQ(uni, "hat"))
                return PyLong_FromLong(e.jhat.hat);
            if (PSDL_PY_C_STREQ(uni, "value"))
                return PyLong_FromLong(e.jhat.value);
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.key.timestamp);
            if (PSDL_PY_C_STREQ(uni, "window_id"))
                return PyLong_FromLong(e.key.windowID);
            if (PSDL_PY_C_STREQ(uni, "state"))
                return PyLong_FromLong(e.key.state);
            if (PSDL_PY_C_STREQ(uni, "repeat"))
                return PyBool_FromLong(e.key.repeat);
            if (PSDL_PY_C_STREQ(uni, "scancode"))
                return PyLong_FromLong(e.key.keysym.scancode);
            if (PSDL_PY_C_STREQ(uni, "sym"))
                return PyLong_FromLong(e.key.keysym.sym);
            if (PSDL_PY_C_STREQ(uni, "mod"))
                return PyLong_FromLong(e.key.keysym.mod);
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.button.timestamp);
            if (PSDL_PY_C_STREQ(uni, "window_id"))
                return PyLong_FromLong(e.button.windowID);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.button.which);
            if (PSDL_PY_C_STREQ(uni, "button"))
                return PyLong_FromLong(e.button.button);
            if (PSDL_PY_C_STREQ(uni, "state"))
                return PyLong_FromLong(e.button.state);
#if SDL_VERSION_ATLEAST(2, 0, 2)
            if (PSDL_PY_C_STREQ(uni, "clicks"))
                return PyLong_FromLong(e.button.clicks);
#endif
            if (PSDL_PY_C_STREQ(uni, "x"))
                return PyLong_FromLong(e.button.x);
            if (PSDL_PY_C_STREQ(uni, "y"))
                return PyLong_FromLong(e.button.y);
            break;
        case SDL_MOUSEMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.motion.timestamp);
            if (PSDL_PY_C_STREQ(uni, "window_id"))
                return PyLong_FromLong(e.motion.windowID);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.motion.which);
            if (PSDL_PY_C_STREQ(uni, "state"))
                return PyLong_FromLong(e.motion.state);
            if (PSDL_PY_C_STREQ(uni, "x"))
                return PyLong_FromLong(e.motion.x);
            if (PSDL_PY_C_STREQ(uni, "y"))
                return PyLong_FromLong(e.motion.y);
            if (PSDL_PY_C_STREQ(uni, "xrel"))
                return PyLong_FromLong(e.motion.xrel);
            if (PSDL_PY_C_STREQ(uni, "yrel"))
                return PyLong_FromLong(e.motion.yrel);
            break;
        case SDL_MOUSEWHEEL:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.wheel.timestamp);
            if (PSDL_PY_C_STREQ(uni, "window_id"))
                return PyLong_FromLong(e.wheel.windowID);
            if (PSDL_PY_C_STREQ(uni, "which"))
                return PyLong_FromLong(e.wheel.which);
            if (PSDL_PY_C_STREQ(uni, "x"))
                return PyLong_FromLong(e.wheel.x);
            if (PSDL_PY_C_STREQ(uni, "y"))
                return PyLong_FromLong(e.wheel.y);
#if SDL_VERSION_ATLEAST(2, 0, 4)
            if (PSDL_PY_C_STREQ(uni, "direction"))
                return PyLong_FromLong(e.wheel.direction);
#endif
            break;
        case SDL_MULTIGESTURE:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.mgesture.timestamp);
            if (PSDL_PY_C_STREQ(uni, "touch_id"))
                return PyLong_FromLong(e.mgesture.touchId);
            if (PSDL_PY_C_STREQ(uni, "x"))
                return PyFloat_FromDouble(e.mgesture.x);
            if (PSDL_PY_C_STREQ(uni, "y"))
                return PyFloat_FromDouble(e.mgesture.y);
            if (PSDL_PY_C_STREQ(uni, "d_theta"))
                return PyFloat_FromDouble(e.mgesture.dTheta);
            if (PSDL_PY_C_STREQ(uni, "d_dist"))
                return PyFloat_FromDouble(e.mgesture.dDist);
            if (PSDL_PY_C_STREQ(uni, "num_fingers"))
                return PyLong_FromLong(e.mgesture.numFingers);
            break;
        case SDL_QUIT:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.quit.timestamp);
            break;
        case SDL_SYSWMEVENT:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.syswm.timestamp);
            /* TODO: Export driver-dependent data somehow */
            break;
        case SDL_TEXTEDITING:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.edit.timestamp);
            if (PSDL_PY_C_STREQ(uni, "window_id"))
                return PyLong_FromLong(e.edit.windowID);
            if (PSDL_PY_C_STREQ(uni, "text"))
                return PyUnicode_FromString(e.edit.text);
            if (PSDL_PY_C_STREQ(uni, "start"))
                return PyLong_FromLong(e.edit.start);
            if (PSDL_PY_C_STREQ(uni, "length"))
                return PyLong_FromLong(e.edit.length);
            break;
        case SDL_TEXTINPUT:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.text.timestamp);
            if (PSDL_PY_C_STREQ(uni, "window_id"))
                return PyLong_FromLong(e.text.windowID);
            if (PSDL_PY_C_STREQ(uni, "text"))
                return PyUnicode_FromString(e.text.text);
            break;
        case SDL_WINDOWEVENT:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.window.timestamp);
            if (PSDL_PY_C_STREQ(uni, "window_id"))
                return PyLong_FromLong(e.window.windowID);
            if (PSDL_PY_C_STREQ(uni, "event"))
                return PyLong_FromLong(e.window.event);
            if (PSDL_PY_C_STREQ(uni, "data1"))
                return PyLong_FromLong(e.window.data1);
            if (PSDL_PY_C_STREQ(uni, "data2"))
                return PyLong_FromLong(e.window.data2);
            break;
        default:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                return PyLong_FromLong(e.user.timestamp);
            if (PSDL_PY_C_STREQ(uni, "window_id"))
                return PyLong_FromLong(e.user.windowID);
            if (PSDL_PY_C_STREQ(uni, "code"))
                return PyLong_FromLong(e.user.code);
            if (PSDL_PY_C_STREQ(uni, "data1")) {
                if (!e.user.data1)
                    Py_RETURN_NONE;
                Py_INCREF((PyObject *)e.user.data1);
                return (PyObject *)e.user.data1;
            }
            if (PSDL_PY_C_STREQ(uni, "data2")) {
                if (!e.user.data2)
                    Py_RETURN_NONE;
                Py_INCREF((PyObject *)e.user.data2);
                return (PyObject *)e.user.data2;
            }
    }
    return PSDL_RAISE_MSG(PyExc_AttributeError, "No such attribute exists");
}

static int
psdlEvent_setattro(PyObject *self, PyObject *uni, PyObject *val)
{
    SDL_Event *e = &PSDL_GET_EVENT(self);

    if (!val) {
        PyErr_SetString(PyExc_RuntimeError, "cannot delete individual elements");
        return -1;
    }

    if (!PyUnicode_Check(uni)) {
        PyErr_SetString(PyExc_ValueError, "argument must be str");
        return -1;
    }

    switch (e->type) {
#if SDL_VERSION_ATLEAST(2, 0, 4)
        case SDL_AUDIODEVICEADDED:
        case SDL_AUDIODEVICEREMOVED:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->adevice.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->adevice.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "iscapture"))
                e->adevice.iscapture = PyObject_IsTrue(val);
            else
                goto notfound;
            break;
#endif
        case SDL_CONTROLLERAXISMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->caxis.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->caxis.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "axis"))
                e->caxis.axis = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "value"))
                e->caxis.value = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->cbutton.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->cbutton.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "button"))
                e->cbutton.button = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "state"))
                e->cbutton.state = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
        case SDL_CONTROLLERDEVICEREMAPPED:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->cdevice.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->cdevice.which = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_DOLLARGESTURE:
        case SDL_DOLLARRECORD:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->dgesture.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "touch_id"))
                e->dgesture.touchId = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "gesture_id"))
                e->dgesture.gestureId = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "num_fingers"))
                e->dgesture.numFingers = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "error"))
                e->dgesture.error = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "x"))
                e->dgesture.x = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "y"))
                e->dgesture.y = PyFloat_AsDouble(val);
            else
                goto notfound;
            break;
        case SDL_DROPFILE:
#if SDL_VERSION_ATLEAST(2, 0, 5)
        case SDL_DROPBEGIN:
        case SDL_DROPCOMPLETE:
        case SDL_DROPTEXT:
#endif
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->drop.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "file")) {
                int nsize;
                const char *temp = PyUnicode_AsUTF8AndSize(val, &nsize);
                if (temp) {
                    e->drop.file = realloc(e->drop.file, nsize);
                    memcpy(e->drop.file, temp, nsize);
                }
            }
            else if (PSDL_PY_C_STREQ(uni, "window_id"))
                e->drop.windowID = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_FINGERMOTION:
        case SDL_FINGERUP:
        case SDL_FINGERDOWN:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->tfinger.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "touch_id"))
                e->tfinger.touchId = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "finger_id"))
                e->tfinger.fingerId = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "x"))
                e->tfinger.x = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "y"))
                e->tfinger.y = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "dx"))
                e->tfinger.dx = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "dy"))
                e->tfinger.dy = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "pressure"))
                e->tfinger.pressure = PyFloat_AsDouble(val);
            else
                goto notfound;
            break;
        case SDL_JOYAXISMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->jaxis.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->jaxis.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "axis"))
                e->jaxis.axis = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "value"))
                e->jaxis.value = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_JOYBALLMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->jball.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->jball.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "ball"))
                e->jball.ball = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "xrel"))
                e->jball.xrel = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "yrel"))
                e->jball.yrel = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->jbutton.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->jbutton.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "button"))
                e->jbutton.button = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "state"))
                e->jbutton.state = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->jdevice.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->jdevice.which = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_JOYHATMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->jhat.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->jhat.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "hat"))
                e->jhat.hat = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "value"))
                e->jhat.value = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->key.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "window_id"))
                e->key.windowID = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "state"))
                e->key.state = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "repeat"))
                e->key.repeat = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "scancode"))
                e->key.keysym.scancode = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "sym"))
                e->key.keysym.sym = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "mod"))
                e->key.keysym.mod = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->button.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "window_id"))
                e->button.windowID = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->button.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "button"))
                e->button.button = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "state"))
                e->button.state = PyLong_AsLong(val);
#if SDL_VERSION_ATLEAST(2, 0, 2)
            else if (PSDL_PY_C_STREQ(uni, "clicks"))
                e->button.clicks = PyLong_AsLong(val);
#endif
            else if (PSDL_PY_C_STREQ(uni, "x"))
                e->button.x = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "y"))
                e->button.y = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_MOUSEMOTION:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->motion.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "window_id"))
                e->motion.windowID = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->motion.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "state"))
                e->motion.state = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "x"))
                e->motion.x = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "y"))
                e->motion.y = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "xrel"))
                e->motion.xrel = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "yrel"))
                e->motion.yrel = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_MOUSEWHEEL:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->wheel.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "window_id"))
                e->wheel.windowID = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "which"))
                e->wheel.which = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "x"))
                e->wheel.x = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "y"))
                e->wheel.y = PyLong_AsLong(val);
#if SDL_VERSION_ATLEAST(2, 0, 4)
            else if (PSDL_PY_C_STREQ(uni, "direction"))
                e->wheel.direction = PyLong_AsLong(val);
#endif
            else
                goto notfound;
            break;
        case SDL_MULTIGESTURE:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->mgesture.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "touch_id"))
                e->mgesture.touchId = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "x"))
                e->mgesture.x = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "y"))
                e->mgesture.y = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "d_theta"))
                e->mgesture.dTheta = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "d_dist"))
                e->mgesture.dDist = PyFloat_AsDouble(val);
            else if (PSDL_PY_C_STREQ(uni, "num_fingers"))
                e->mgesture.numFingers = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_QUIT:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->quit.timestamp = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_SYSWMEVENT:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->syswm.timestamp = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_TEXTEDITING:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->edit.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "window_id"))
                e->edit.windowID = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "text"))
                strncpy(e->edit.text, PyUnicode_AsUTF8(val), 32);
            else if (PSDL_PY_C_STREQ(uni, "start"))
                e->edit.start = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "length"))
                e->edit.length = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        case SDL_TEXTINPUT:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->text.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "window_id"))
                e->text.windowID = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "text"))
                strncpy(e->edit.text, PyUnicode_AsUTF8(val), 32);
            else
                goto notfound;
            break;
        case SDL_WINDOWEVENT:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->window.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "window_id"))
                e->window.windowID = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "event"))
                e->window.event = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "data1"))
                e->window.data1 = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "data2"))
                e->window.data2 = PyLong_AsLong(val);
            else
                goto notfound;
            break;
        default:
            if (PSDL_PY_C_STREQ(uni, "timestamp"))
                e->user.timestamp = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "window_id"))
                e->user.windowID = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "code"))
                e->user.code = PyLong_AsLong(val);
            else if (PSDL_PY_C_STREQ(uni, "data1")) {
                Py_XDECREF((PyObject *)e->user.data1);
                e->user.data1 = (void *)val;
                Py_INCREF(val);
            }
            else if (PSDL_PY_C_STREQ(uni, "data2")) {
                Py_XDECREF((PyObject *)e->user.data2);
                e->user.data2 = (void *)val;
                Py_INCREF(val);
            }
            else
                goto notfound;
    }
    return PyErr_Occurred() ? 0 : -1;

notfound:
    PyErr_SetString(PyExc_AttributeError, "No such attribute exits");
    return -1;
}

static PyObject *
psdlEvent_new(PyTypeObject *subtype, PyObject *args, PyObject *kwds)
{
    psdlEvent *ret = PyObject_New(psdlEvent, subtype);
    if (!ret)
        return NULL;

    SDL_zero(ret->ev);
    return (PyObject *)ret;
}

static int
psdlEvent_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    psdlEvent *e = (psdlEvent *)self;
    PyObject *key, *value;
    Py_ssize_t pos = 0;

    if (!PyArg_ParseTuple(args, "i", &(e->ev.type)))
        return -1;

    while (PyDict_Next(kwds, &pos, &key, &value)) {
        if (psdlEvent_setattro(self, key, value))
            return -1;
    }
    return 0;
}

static PyTypeObject psdlEvent_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "psdl.Event",
    .tp_doc = DOC_PLACEHOLDER,
    .tp_basicsize = sizeof(psdlEvent),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_dealloc = psdlEvent_dealloc,
    .tp_getattro = psdlEvent_getattro,
    .tp_setattro = psdlEvent_setattro,
    .tp_new = psdlEvent_new,
    .tp_init = psdlEvent_init,
};

static PyObject *
_psdlEvent_from_event(SDL_Event *event, int donew)
{
    psdlEvent *ret;
    if (!event)
        return NULL;

    ret = PyObject_New(psdlEvent, &psdlEvent_Type);
    if (!ret)
        return NULL;

    if (donew)
        _sdl_event_new_copy(event);

    ret->ev = *event;
    return (PyObject *)ret;
}

typedef struct {
    int isfilter;
    PyObject *args, *kwargs;
} FuncArgsKwargs;

static FuncArgsKwargs *
_create_func_arg_kwargs(PyObject *args, PyObject *kwargs, int isfilter)
{
    FuncArgsKwargs *ret;
    if (!args ||
        !PyTuple_Size(args)) {
        PyErr_SetString(PyExc_ValueError, "arguments cannot be empty");
        return NULL;
    }

    if (!PyCallable_Check(PyTuple_GET_ITEM(args, 0))) {
        PyErr_SetString(PyExc_TypeError, "First argument must be callable");
        return NULL;
    }

    ret = PyMem_New(FuncArgsKwargs, 1);
    if (!ret)
        return (FuncArgsKwargs *)PyErr_NoMemory();

    ret->isfilter = isfilter;

    Py_INCREF(args);
    Py_XINCREF(kwargs);
    ret->args = args;
    ret->kwargs = kwargs;
    return ret;
}

static void
_clean_func_arg_kwargs(FuncArgsKwargs *data)
{
    Py_DECREF(data->args);
    Py_XDECREF(data->kwargs);
    PyMem_Del(data);
}

static int
_event_filter_func(void *userdata, SDL_Event *event)
{
    int i, ret = 1;
    PyObject *funcret, *args;
    FuncArgsKwargs *data = (FuncArgsKwargs *)userdata;
    PyGILState_STATE gstate = PyGILState_Ensure();

    args = PyTuple_New(PyTuple_Size(data->args));
    if (!args)
        goto end;

    PyTuple_SET_ITEM(args, 0, _psdlEvent_from_event(event, 1));
    if (!PyTuple_GET_ITEM(args, 0))
        goto end;

    for (i = 1; i < PyTuple_Size(args); i++) {
        /* This steals references, so incref */
        Py_INCREF(PyTuple_GET_ITEM(data->args, i));
        PyTuple_SET_ITEM(args, i, PyTuple_GET_ITEM(data->args, i));
    }

    funcret = PyObject_Call(PyTuple_GET_ITEM(data->args, 0), args, data->kwargs);
    Py_DECREF(args);

    if (data->isfilter) {
        if (!funcret) {
            ret = 0;
            goto end;
        }
        ret = PyObject_IsTrue(funcret);
        if (PSDL_EVENT_CHECK(funcret)) {
            memcpy(event, &PSDL_GET_EVENT(funcret), sizeof(SDL_Event));
            _sdl_event_new_copy(event);
        }
    }
    Py_XDECREF(funcret);
end:
    PyErr_Clear(); /* Clear any error, if set */
    PyGILState_Release(gstate);
    return ret;
}

static PyObject *
psdl_add_event_watch(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *ret;
    FuncArgsKwargs *data = _create_func_arg_kwargs(args, kwargs, 0);
    if (!data)
        return NULL;
    
    ret = PyLong_FromLong((long)data);
    if (!ret) {
        PyMem_Free(data);
        return PyErr_NoMemory();
    }
    if (PyList_Append(eventwatchlist, ret)) {
        Py_DECREF(ret);
        PyMem_Free(data);
        return PyErr_NoMemory();
    }
    SDL_AddEventWatch(_event_filter_func, (void *)data);
    return ret;
}

static PyObject *
psdl_del_event_watch(PyObject *self, PyObject *obj)
{
    int temp;
    if (!PyLong_Check(obj))
        return PSDL_RAISE_MSG(PyExc_TypeError,
            "Expected integer argument");
            
    temp = PySequence_Index(eventwatchlist, obj);
    if (temp == -1 || 
        PySequence_DelItem(eventwatchlist, temp) == -1)
        return NULL;
    
    SDL_DelEventWatch(_event_filter_func, (void *)PyLong_AsLong(obj));
    _clean_func_arg_kwargs((FuncArgsKwargs *)PyLong_AsLong(obj));
    Py_RETURN_NONE;
}

static PyObject *
psdl_event_state(PyObject *self, PyObject *args)
{
    int type, state;
    if (!PyArg_ParseTuple(args, "ii", &type, &state))
        return NULL;
    return PyLong_FromLong(SDL_EventState(type, state));
}

static PyObject *
psdl_filter_events(PyObject *self, PyObject *args, PyObject *kwargs)
{
    FuncArgsKwargs *data = _create_func_arg_kwargs(args, kwargs, 1);
    if (!data)
        return NULL;

    SDL_FilterEvents(_event_filter_func, (void *)data);
    _clean_func_arg_kwargs(data);
    Py_RETURN_NONE;
}

static PyObject *
psdl_flush_event(PyObject *self, PyObject *args)
{
    int type;
    if (!PyArg_ParseTuple(args, "i", &type))
        return NULL;
    SDL_FlushEvent(type);
    Py_RETURN_NONE;
}

static PyObject *
psdl_flush_events(PyObject *self, PyObject *args)
{
    int start, finish;
    if (!PyArg_ParseTuple(args, "ii", &start, &finish))
        return NULL;
    SDL_FlushEvents(start, finish);
    Py_RETURN_NONE;
}

static PyObject *
psdl_get_event_filter(PyObject *self)
{
    FuncArgsKwargs *data;
    SDL_EventFilter filter;
    
    if (!SDL_GetEventFilter(&filter, (void **)&data))
        Py_RETURN_NONE;
    
    if (filter != _event_filter_func)
        Py_RETURN_NONE;
    
    return Py_BuildValue("OO", data->args, data->kwargs); /* TODO VERIFY */
}

static PyObject *
psdl_get_event_state(PyObject *self, PyObject *args)
{
    int type;
    if (!PyArg_ParseTuple(args, "i", &type))
        return NULL;
    return PyLong_FromLong(SDL_GetEventState(type));
}

static PyObject *
psdl_get_num_touch_devices(PyObject *self)
{
    return PyLong_FromLong(SDL_GetNumTouchDevices());
}

static PyObject *
psdl_get_num_touch_fingers(PyObject *self, PyObject *args)
{
    int touch, ret;
    if (!PyArg_ParseTuple(args, "i", &touch))
        return NULL;
    ret = SDL_GetNumTouchFingers(touch);
    if (!ret)
        return PSDL_RAISE_SDL_ERROR;
    return PyLong_FromLong(ret);
}

static PyObject *
psdl_get_touch_device(PyObject *self, PyObject *args)
{
    int ind, ret;
    if (!PyArg_ParseTuple(args, "i", &ind))
        return NULL;
    ret = SDL_GetTouchDevice(ind);
    if (!ret)
        return PSDL_RAISE_SDL_ERROR;
    return PyLong_FromLong(ret);
}

/* TODO SDL_GetTouchFinger */

static PyObject *
psdl_has_event(PyObject *self, PyObject *args)
{
    int type;
    if (!PyArg_ParseTuple(args, "i", &type))
        return NULL;
    return PyBool_FromLong(SDL_HasEvent(type));
}

static PyObject *
psdl_has_events(PyObject *self, PyObject *args)
{
    int start, finish;
    if (!PyArg_ParseTuple(args, "ii", &start, &finish))
        return NULL;
    return PyBool_FromLong(SDL_HasEvents(start, finish));
}

/* TODO SDL_LoadDollarTemplates */

static PyObject *
psdl_peep_events(PyObject *self, PyObject *args)
{
    SDL_Event *eventbuf;
    PyObject *first, *temp;
    int action, start, finish, len, ret, i;

    if (!PyArg_ParseTuple(args, "Oiii", &first, &action, &start, &finish))
        return NULL;

    if (action == SDL_ADDEVENT) {
        if (!PySequence_Check(first))
            return PSDL_RAISE_MSG(PyExc_TypeError,
                "First argument must be a sequence for ADDEVENT action");

        len = PySequence_Size(first);
        if (len == -1)
            return NULL;

        eventbuf = PyMem_New(SDL_Event, len);
        if (!eventbuf)
            return PyErr_NoMemory();

        for (i = 0; i < len; i++) {
            temp = PySequence_GetItem(first, i);
            if (!temp) {
                PyMem_Del(eventbuf);
                return NULL;
            }
            if (!PSDL_EVENT_CHECK(temp)) {
                PyMem_Del(eventbuf);
                return PSDL_RAISE_MSG(PyExc_ValueError,
                    "sequence must contain event objects");
            }
            eventbuf[i] = PSDL_GET_EVENT(temp);
            _sdl_event_new_copy(&eventbuf[i]);
            Py_DECREF(temp); /* Because its a new reference */
        }
        ret = SDL_PeepEvents(eventbuf, len, action, start, finish);
        PyMem_Del(eventbuf);

        if (ret < 0)
            return PSDL_RAISE_SDL_ERROR;
        return PyLong_FromLong(ret);
    }
    else {
        len = PyLong_AsLong(first);
        if (len < 0) {
            if (PyErr_Occurred())
                return NULL;
            return PSDL_RAISE_MSG(PyExc_ValueError,
                    "first argument must be non-negative");
        }
        eventbuf = PyMem_New(SDL_Event, len);
        if (!eventbuf)
            return PyErr_NoMemory();

        ret = SDL_PeepEvents(eventbuf, len, action, start, finish);
        if (ret < 0) {
            PyMem_Del(eventbuf);
            return PSDL_RAISE_SDL_ERROR;
        }

        temp = PyList_New(ret);
        if (!temp) {
            PyMem_Del(eventbuf);
            return NULL;
        }

        for (i = 0; i < ret; i++) {
            PyList_SET_ITEM(temp, i,
                _psdlEvent_from_event(&eventbuf[i], action == SDL_PEEKEVENT));
        }
        return temp;
    }
}

static PyObject *
psdl_poll_event(PyObject *self)
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
        return _psdlEvent_from_event(&event, 0);
    else
        Py_RETURN_NONE;
}

static PyObject *
psdl_pump_events(PyObject *self)
{
    SDL_PumpEvents();
    Py_RETURN_NONE;
}

static PyObject *
psdl_push_event(PyObject *self, PyObject *obj)
{
    int ret;
    SDL_Event event;
    if (!PSDL_EVENT_CHECK(obj))
        return PSDL_RAISE_MSG(PyExc_TypeError,
            "argument must be psdl.Event object");

    event = PSDL_GET_EVENT(obj);
    _sdl_event_new_copy(&event);

    ret = SDL_PushEvent(&event);
    if (ret < 0)
        return PSDL_RAISE_SDL_ERROR;
    return PyBool_FromLong(ret);
}

static PyObject *
psdl_quit_requested(PyObject *self)
{
    return PyBool_FromLong(SDL_QuitRequested());
}

static PyObject *
psdl_record_gesture(PyObject *self, PyObject *args)
{
    int touch;
    if (!PyArg_ParseTuple(args, "i", &touch))
        return NULL;
    return PyBool_FromLong(SDL_RecordGesture(touch));
}

static PyObject *
psdl_register_events(PyObject *self, PyObject *args)
{
    int numevents;
    if (!PyArg_ParseTuple(args, "i", &numevents))
        return NULL;
    return PyLong_FromLong(SDL_RegisterEvents(numevents));
}

/* TODO SDL_SaveAllDollarTemplates and another */

static PyObject *
psdl_set_event_filter(PyObject *self, PyObject *args, PyObject *kwargs)
{
    FuncArgsKwargs *data = _create_func_arg_kwargs(args, kwargs, 1);
    if (!data)
        return NULL;

    SDL_SetEventFilter(_event_filter_func, (void *)data);
    Py_RETURN_NONE;
}

static PyObject *
psdl_wait_event(PyObject *self)
{
    SDL_Event event;
    if (SDL_WaitEvent(&event))
        return _psdlEvent_from_event(&event, 0);
    else
        return PSDL_RAISE_SDL_ERROR;
}

static PyObject *
psdl_wait_event_timeout(PyObject *self, PyObject *args)
{
    SDL_Event event;
    int timeout;
    if (!PyArg_ParseTuple(args, "i", &timeout))
        return NULL;
    
    if (SDL_WaitEventTimeout(&event, timeout))
        return _psdlEvent_from_event(&event, 0);
    else
        Py_RETURN_NONE;
}

PSDL_BEGIN_METH_DEF
    PSDL_EXPORT_FUNC(add_event_watch, METH_VARARGS | METH_KEYWORDS)
    PSDL_EXPORT_FUNC(del_event_watch, METH_O)
    PSDL_EXPORT_FUNC(event_state, METH_VARARGS)
    PSDL_EXPORT_FUNC(filter_events, METH_VARARGS | METH_KEYWORDS)
    PSDL_EXPORT_FUNC(flush_event, METH_VARARGS)
    PSDL_EXPORT_FUNC(flush_events, METH_VARARGS)
    PSDL_EXPORT_FUNC(get_event_filter, METH_NOARGS)
    PSDL_EXPORT_FUNC(get_event_state, METH_VARARGS)
    PSDL_EXPORT_FUNC(get_num_touch_devices, METH_NOARGS)
    PSDL_EXPORT_FUNC(get_num_touch_fingers, METH_VARARGS)
    PSDL_EXPORT_FUNC(get_touch_device, METH_VARARGS)
    PSDL_EXPORT_FUNC(has_event, METH_VARARGS)
    PSDL_EXPORT_FUNC(has_events, METH_VARARGS)
    PSDL_EXPORT_FUNC(peep_events, METH_VARARGS)
    PSDL_EXPORT_FUNC(poll_event, METH_NOARGS)
    PSDL_EXPORT_FUNC(pump_events, METH_NOARGS)
    PSDL_EXPORT_FUNC(push_event, METH_O)
    PSDL_EXPORT_FUNC(quit_requested, METH_NOARGS)
    PSDL_EXPORT_FUNC(record_gesture, METH_VARARGS)
    PSDL_EXPORT_FUNC(register_events, METH_VARARGS)
    PSDL_EXPORT_FUNC(set_event_filter, METH_VARARGS | METH_KEYWORDS)
    PSDL_EXPORT_FUNC(wait_event, METH_NOARGS)
    PSDL_EXPORT_FUNC(wait_event_timeout, METH_VARARGS)
PSDL_END_METH_DEF

PSDL_BEGIN_MODINIT(events)
    PSDL_EXPORT_SDL_CONST(PRESSED)
    PSDL_EXPORT_SDL_CONST(RELEASED)
    PSDL_EXPORT_SDL_CONST(ADDEVENT)
    PSDL_EXPORT_SDL_CONST(PEEKEVENT)
    PSDL_EXPORT_SDL_CONST(GETEVENT)
    PSDL_EXPORT_SDL_CONST(QUERY)
    PSDL_EXPORT_SDL_CONST(IGNORE)
    PSDL_EXPORT_SDL_CONST(DISABLE)
    PSDL_EXPORT_SDL_CONST(ENABLE)
    
    /* Event types */
    PSDL_EXPORT_SDL_CONST(FIRSTEVENT)
    PSDL_EXPORT_SDL_CONST(QUIT)
    PSDL_EXPORT_SDL_CONST(APP_TERMINATING)
    PSDL_EXPORT_SDL_CONST(APP_LOWMEMORY)
    PSDL_EXPORT_SDL_CONST(APP_WILLENTERBACKGROUND)
    PSDL_EXPORT_SDL_CONST(APP_DIDENTERBACKGROUND)
    PSDL_EXPORT_SDL_CONST(APP_WILLENTERFOREGROUND)
    PSDL_EXPORT_SDL_CONST(APP_DIDENTERFOREGROUND)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT)
    PSDL_EXPORT_SDL_CONST(SYSWMEVENT)
    PSDL_EXPORT_SDL_CONST(KEYDOWN)
    PSDL_EXPORT_SDL_CONST(KEYUP)
    PSDL_EXPORT_SDL_CONST(TEXTEDITING)
    PSDL_EXPORT_SDL_CONST(TEXTINPUT)
    PSDL_EXPORT_SDL_CONST(MOUSEMOTION)
    PSDL_EXPORT_SDL_CONST(MOUSEBUTTONUP)
    PSDL_EXPORT_SDL_CONST(MOUSEBUTTONDOWN)
    PSDL_EXPORT_SDL_CONST(MOUSEWHEEL)
    PSDL_EXPORT_SDL_CONST(JOYAXISMOTION)
    PSDL_EXPORT_SDL_CONST(JOYBALLMOTION)
    PSDL_EXPORT_SDL_CONST(JOYHATMOTION)
    PSDL_EXPORT_SDL_CONST(JOYBUTTONUP)
    PSDL_EXPORT_SDL_CONST(JOYBUTTONDOWN)
    PSDL_EXPORT_SDL_CONST(JOYDEVICEREMOVED)
    PSDL_EXPORT_SDL_CONST(JOYDEVICEADDED)
    PSDL_EXPORT_SDL_CONST(CONTROLLERAXISMOTION)
    PSDL_EXPORT_SDL_CONST(CONTROLLERBUTTONUP)
    PSDL_EXPORT_SDL_CONST(CONTROLLERBUTTONDOWN)
    PSDL_EXPORT_SDL_CONST(CONTROLLERDEVICEADDED)
    PSDL_EXPORT_SDL_CONST(CONTROLLERDEVICEREMOVED)
    PSDL_EXPORT_SDL_CONST(CONTROLLERDEVICEREMAPPED)
    PSDL_EXPORT_SDL_CONST(FINGERDOWN)
    PSDL_EXPORT_SDL_CONST(FINGERUP)
    PSDL_EXPORT_SDL_CONST(FINGERMOTION)
    PSDL_EXPORT_SDL_CONST(DOLLARGESTURE)
    PSDL_EXPORT_SDL_CONST(DOLLARRECORD)
    PSDL_EXPORT_SDL_CONST(MULTIGESTURE)
    PSDL_EXPORT_SDL_CONST(CLIPBOARDUPDATE)
    PSDL_EXPORT_SDL_CONST(DROPFILE)
    PSDL_EXPORT_SDL_CONST(USEREVENT)
    PSDL_EXPORT_SDL_CONST(LASTEVENT)
#if SDL_VERSION_ATLEAST(2, 0, 5)
    PSDL_EXPORT_SDL_CONST(DROPTEXT)
    PSDL_EXPORT_SDL_CONST(DROPBEGIN)
    PSDL_EXPORT_SDL_CONST(DROPCOMPLETE)
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
    PSDL_EXPORT_SDL_CONST(AUDIODEVICEADDED)
    PSDL_EXPORT_SDL_CONST(AUDIODEVICEREMOVED)
    PSDL_EXPORT_SDL_CONST(KEYMAPCHANGED)
    PSDL_EXPORT_SDL_CONST(RENDER_DEVICE_RESET)
#endif
#if SDL_VERSION_ATLEAST(2, 0, 2)
    PSDL_EXPORT_SDL_CONST(RENDER_TARGETS_RESET)
#endif
    
    /* WINDOWEVENT subtypes */
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_SHOWN)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_HIDDEN)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_EXPOSED)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_MOVED)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_RESIZED)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_SIZE_CHANGED)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_MINIMIZED)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_MAXIMIZED)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_RESTORED)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_ENTER)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_LEAVE)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_FOCUS_GAINED)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_FOCUS_LOST)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_CLOSE)
#if SDL_VERSION_ATLEAST(2, 0, 5)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_TAKE_FOCUS)
    PSDL_EXPORT_SDL_CONST(WINDOWEVENT_HIT_TEST)
#endif
    
    PSDL_EXPORT_CLASS(Event)
    eventwatchlist = PyList_New(0);
    if (!eventwatchlist) {
        Py_DECREF(mod);
        return NULL;
    }
PSDL_END_MODINIT

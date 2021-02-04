/*
 * This file is a part of the psdl package.
 * Copyright (C) 2021 Ankith (ankith26)
 *
 * Distributed under the MIT license.
 */
 
typedef struct {
    PyObject_HEAD
    SDL_Event ev;
} psdlEvent;

typedef struct {
    PyObject_HEAD
    SDL_Window *win;
} psdlWindow;

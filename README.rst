
PSDL - A pythonic wrapper for SDL
=================================

psdl is a package that wraps the excellent SDL2 module in an easy to use python 
interface.

Note that this project is still in its super early stages, theres lots of stuff 
still left to be wrapped. 


Why PSDL?
---------
You may be already knowing that there are many SDL wrappers out there, from stuff 
like PySDL2 to pygame. What makes PSDL different? Well, PSDL aims to be a wrapper 
"of right thickness". There are some SDL wrappers that are too thin, and they 
expose the C-level details to python, which is not that elegant. Also, they 
directly copy the SDL API, which is un-pythonic.
And there are SDL wrappers that are too thick, and they dont export the SDL
API at all, they have their own API.

PSDL aims to export the SDL API as directly as possible, while also "pythonifying"
it. Let us consider a typical SDL function that looks like 
``SDL_SomeFunctionForSomething``. PSDL will export it, by stripping the inital
``SDL_`` (because python has namespaces, unlike C) and changing the style of the
function name to something that looks more pythonic. The function will look like
``psdl.some_function_for_something`` from the PSDL side. Similarly, SDL constants
are exported by just stripping the initial ``SDL_`` part of it, ``SDL_SOME_CONSTANT``
becomes ``psdl.SOME_CONSTANT``. While SDL provided enums, PSDL does not export the
enums directly, it just exports the constants within it.

Coming to structures, PSDL does different stuff based on the context. Since C is not
object oriented, you will find this style of coding commonly used in SDL.

::

    SDL_someobj *obj;
    SDL_CreateSomeObj(obj, someargs);
    SDL_SomeMethod(obj, moreargs);
    SDL_AnotherMethod(obj, args, moreargs);
    SDL_DestroyObj(obj);

We can take advantage of python OOP, and psdl will export the samething like

::
    obj = psdl.SomeObj(args)
    obj.some_method(moreargs)
    obj.another_method(args, moreargs)

Notice the naming convention adopted here. Also notice that in python, we need
not explicitly deallocate objects, python being a high-level language, does that
automatically for us.

Installation
------------
psdl can be installed via pip, using the following command, ``pip3 install psdl``.
Note that currently, psdl does not provide binary "wheels" on PyPI, so the pip 
command will build psdl from source. For that, check out the section below

Building from source
--------------------
Windows
~~~~~~~
The build process is pretty straight forward on windows. You just need to have
the MSVC C compiler set up correctly. psdl is automatically going to get all its
dependencies (ie, its gonna download SDL from the official website) and automatically
build psdl.

Mac/Linux
~~~~~~~~~
On Mac/linux too, the build process aint complicated. All you need to have is a C
compiler (which is usually present on most systems) and SDL installed via your 
package manager. Then psdl will easily install from source.

Advanced Build Options
~~~~~~~~~~~~~~~~~~~~~~
You can set two environment variables, ``PSDL_INCLUDE_DIR`` and ``PSDL_LIB_DIR`` to
optionally specify paths to SDL include and library directories, respectively. This
will be used by PSDL when it attempts to build from source.

TODO
----
Theres still lot left to do, like I said earlier, this project is still in its super
early stages. There needs to be docs, tests, and examples written, apart from the 
code that needs to be written.
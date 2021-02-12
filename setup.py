"""
This file is a part of the psdl package.
Copyright (C) 2021 Ankith (ankith26)

Distributed under the MIT license.
"""
import glob
import os
import platform
import shutil
import sys

import setuptools

WIN = platform.system() == "Windows"

PSDL_MAJOR_VERSION = 0
PSDL_MINOR_VERSION = 0
PSDL_PATCH_VERSION = 1

# can be any of alpha a[1-6], beta b[1-3], rc, or empty for final release
PSDL_RELEASE_TAGS = "a1"

PSDL_VERSION_STRING = "{}.{}.{}{}".format(
    PSDL_MAJOR_VERSION,
    PSDL_MINOR_VERSION,
    PSDL_PATCH_VERSION,
    PSDL_RELEASE_TAGS,
)

STR_MACRO_FORMAT = '\\"{}\\"' if WIN else '"{}"'
def format_str_macro(val):
    return STR_MACRO_FORMAT.format(val)

definedmacros = [
    ("PSDL_MAJOR_VERSION", str(PSDL_MAJOR_VERSION)),
    ("PSDL_MINOR_VERSION", str(PSDL_MINOR_VERSION)),
    ("PSDL_PATCH_VERSION", str(PSDL_PATCH_VERSION)),
    ("PSDL_VERSION_STRING", format_str_macro(PSDL_VERSION_STRING)),
]

with open("README.rst", "r", encoding="utf-8") as f:
    LONG_DESCRIPTION = f.read()

packagedir = {
    "psdl": "psdl",
    "psdl.mixer": "psdl/mixer",
    "psdl.image": "psdl/image",
    "psdl.ttf": "psdl/ttf",
    "psdl.gfxdraw": "psdl/gfxdraw",
    "psdl.tests": "tests",
    "psdl.examples": "examples",
    "psdl.docs": "docs",
}


def download_SDL_Windows():
    import hashlib
    import io
    import urllib.request as urllib
    import zipfile

    SDL_DOWNLOAD = "https://www.libsdl.org/release/SDL2-devel-2.0.14-VC.zip"
    SDL_DIR_NAME = "SDL2-2.0.14"
    SDL_CHECK_SUM = "48d5dcd4a445410301f5575219cffb6de654edb8"
    SDL_DOWNLOADED_PATH = os.path.join("downloads", SDL_DIR_NAME)

    if not os.path.isdir(SDL_DOWNLOADED_PATH):
        print("Downloading SDL from", SDL_DOWNLOAD)
        request = urllib.Request(SDL_DOWNLOAD, headers={
            'User-Agent': 'Chrome/35.0.1916.47 Safari/537.36'
            }
        )
        with urllib.urlopen(request) as download:
            response = download.read()

        print("Veryfing checksum")
        if hashlib.sha1(response).hexdigest() != SDL_CHECK_SUM:
            raise RuntimeError("SDL download failed due to checksum mismatch")

        print("Extracting zip file into downloads folder")
        with zipfile.ZipFile(io.BytesIO(response), 'r') as zipped:
            if not os.path.isdir("downloads"):
                os.mkdir("downloads")
            zipped.extractall("downloads")
        print("Download complete")

    arch = "x64" if sys.maxsize > 2**32 else "x86"
    return os.path.join(SDL_DOWNLOADED_PATH, "include"), os.path.join(
        SDL_DOWNLOADED_PATH, "lib", arch)

includedir = os.environ.get("PSDL_INCLUDE_DIR", False)
libdir = os.environ.get("PSDL_LIB_DIR", False if WIN else "/usr/lib")

if WIN:
    if includedir ^ libdir:
        print("========================================")
        print("Either include dir or lib dir is not set")
        print("You have either set both, or leave both")
        print("========================================")
        raise ValueError("Invalid build configuration")

    if not includedir:
        includedir, libdir = download_SDL_Windows()
        shutil.copyfile(os.path.join(libdir, "SDL2.dll"),
                        os.path.join("psdl", "SDL2.dll"))

elif not includedir:
    lookupincludedirs = [
        "/usr/include/SDL2",
        "/usr/local/include/SDL2"
    ]
    for includedir in lookupincludedirs:
        if os.path.exists(includedir):
            break
    else:
        raise RuntimeError("SDL2 include directory not found")

def name_from_cfile(cfile):
    return cfile.rstrip(".c").replace('\\' if WIN else '/', '.')

extensions = []

# if we have any files we don't want to compile, we put it in this list.
# right now, this list includes a lot of names! The goal is to eventually get it
# down to an empty list
EXCLUDE_FILES = [
    'sensor.c',
    'vulkan.c',
    'joystick.c',
    'keyboard.c',
    'rect.c',
    'timer.c',
    'mouse.c',
    'system.c',
    'pixels.c',
    'haptic.c',
    'rwops.c',
    'hints.c',
    'audio.c',
    'endian.c',
    'gamecontroller.c',
    'surface.c',
    'syswm.c',
    'render.c'
]

for cfile in glob.iglob("psdl/*.c"):
    if cfile not in map(lambda x: os.path.join("psdl", x), EXCLUDE_FILES):
        modnamemacro = ("PSDL_MOD_NAME", format_str_macro(name_from_cfile(cfile)))
        ext = setuptools.Extension(
            name=name_from_cfile(cfile),
            sources=[cfile],
            include_dirs=[includedir],
            define_macros=definedmacros + [modnamemacro],
            library_dirs=[libdir],
            libraries=["SDL2"],
            # runtime_library_dirs=runtimelibdirs,  # TODO check if this is needed
            language="c",
        )
        extensions.append(ext)

setuptools.setup(
    name="psdl",
    version=PSDL_VERSION_STRING,
    author="Ankith (ankith26)",
    author_email="itsankith26@gmail.com",
    description="A pythonic wrapper for SDL2",
    long_description=LONG_DESCRIPTION,
    long_description_content_type="text/x-rst",
    url="https://github.com/ankith26/psdl",
    packages=list(packagedir),
    package_dir=packagedir,
    ext_modules=extensions,
    include_package_data=True,
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Developers",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "License :: OSI Approved :: MIT License",
    ],
    python_requires=">=3.5",
    zip_safe=False,
)

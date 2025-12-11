import sys # inbuilt module

# use: "python setup.py --debug=True build_ext --inplace" for quick debugging
if len(sys.argv) > 1 and sys.argv[1].startswith("--debug="):
    DEBUG = sys.argv[1].split("=", 1)[1]
    sys.argv.pop(1)
    print("DEBUG mode set to:", DEBUG)
    if "True" in DEBUG:
            DEBUG = True
    elif "False" in DEBUG:
            DEBUG = False
    else:
        raise(ValueError("Assignment of DEBUG from system argument did not work. Aborting cython build!"))
else:
    DEBUG = False

from setuptools import setup, Extension, find_packages
from Cython.Build import cythonize
import numpy

"""
This setup script directs built of cython file (.pyx) and 
is run by executing the command:
    python setup.py build_ext --inplace
    (additional flag: --force, if e.g. setup file was altered)
During the build process, Cython generates a C file from the .pyx file.
The C file is then compiled into a shared object file (.so)
"""

name = "_core"  # name of the library
filepath = "phydesim/rk_lib/_cython/"
modulepath = "phydesim.rk_lib._cython."


#==============================================================================
# additional flags for compilation of .c file:

extra_compile_args = []
# "-03" commands compiler to apply all optimizations available
if sys.platform == "linux":
    # Linux needs flag -fPIC to properly recognize shared library as ELF header
    extra_compile_args += ["-fPIC"]
elif sys.platform == "darwin":  # macOS
    # -mmacosx-version-min=10.9 ensures compatibility with older macOS versions
    extra_compile_args += ["-O3", "-mmacosx-version-min=10.9"]

#==============================================================================
# append flags for linking C libraries: e.g. -lm (link math library)
c_header_compile_args = []
c_header_path = []

#==============================================================================
extension = Extension(
    name = modulepath + name,             
    sources = [filepath + name + ".pyx"],
    # combine all c compiler args
    extra_compile_args = extra_compile_args + c_header_compile_args,
    # one can also write "cython headers" (.pxd) which include
    # C declarations, in this case, add path to .pxd files here
    include_dirs = [numpy.get_include()] + c_header_path,
    # if code does require old numpy API we turn it off
    define_macros=[("NPY_NO_DEPRECATED_API", "NPY_1_7_API_VERSION")],
)

setup(
    name = name,
    packages = find_packages(),
    ext_modules = cythonize(
        extension,
        compiler_directives = {
            "language_level": 3,  # Python-3 syntax
            # next two are set to False for performance
            # check if index is in bounds of array (no overflow)
            "boundscheck": DEBUG,
            # negative indices
            "wraparound": DEBUG,
            # embed function signatures in C code
            "embedsignature": True,
            # next two helpful for debugging
            # profiling information are needed for performance analysis
            "profile": DEBUG,
            # line tracing lets one see execution of code line by line
            "linetrace": DEBUG,
            # use C division-->no checks if division by zero
            "cdivision": not DEBUG,
            # no None checks
            "nonecheck": DEBUG,
        },
        # create .html file for debugging
        annotate = DEBUG,
    ),
)
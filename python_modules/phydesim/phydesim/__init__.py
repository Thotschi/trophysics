__phydesim_submodules__ = {"rk_lib"}

# define what is imported when using "from phydesim import *"
__all__ = list(
        __phydesim_submodules__
)

# lazy import --> module only imported if module.submodule is used
def __getattr__(attr):
    if attr == "rk_lib":
        import phydesim.rk_lib
        return phydesim.rk_lib
    # !r represents object in ticks
    raise AttributeError(f"Module {__name__!r} has no attribute {attr!r}")

def __dir__():
    return __all__

# if altered, also changed in pyproject.toml via setuptools.dynamic 
__version__ = "0.9.3"

def __description__():
    return "This package provides a collection of machine learning algorithms and utilities for physics-related data analysis."

def __author__():
    return "Gregor Rodehutskors, Thorge Reinisch"
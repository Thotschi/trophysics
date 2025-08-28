__name__ = "tmlearns"

__submodules__ = {"linregress"}

# define what is imported when using "from phydesim import *"
__all__ = list(
        __submodules__
)

# lazy import --> module only imported if module.submodule is used
def __getattr__(attr):
    if attr == "linregress":
        import tmlearn.linregress
        return phydesim.text
    # !r represents object in ticks
    raise AttributeError(f"Module {__name__!r} has no attribute {attr!r}")

def __dir__():
    return __all__

def __version__():
    return "0.1.0"

def __description__():
    return "This module contains machine learning algorithms."

def __author__():
    return "Thorge Reinisch"
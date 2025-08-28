__name__ = "io_utils"

__submodules__ = {"plot", "text", "cli", "ai"}

__all__ = list(
        __submodules__
)

# lazy import --> module only imported if module.submodule is used
def __getattr__(attr):
    if attr == "text":
        import io_utils.text
        return io_utils.text
    if attr == "plot":
        import io_utils.plot
        return io_utils.plot
    if attr == "cli":
        import io_utils.cli
        return io_utils.cli
    if attr == "ai":
        import io_utils.ai
        return io_utils.ai
    # !r represents object in ticks
    raise AttributeError(f"Module {__name__!r} has no attribute {attr!r}")

def __dir__():
    return __all__

__version__ = 1.0

def __description__():
    return "This module contains a number of input output utensils for creating\n figures, text output, system control and command line tools."

def __author__():
    return "Thorge Reinisch"
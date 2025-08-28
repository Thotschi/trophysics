from ._projectscript_helper import get_passed_options, compare_options, get_passed_values, set_options_to_values, get_metadata
import sys

def version():
    from io_utils import __version__
    print(f"io_utils version {__version__}")

def help():
    print("Available commands:")
    print("  version - Show the current version of io_utils.")
    print("  help - Show this help message.")

def main():
    if len(sys.argv) == 1:
        print("Usage: io_utils-config [command] [-options] [--options=value]")
        sys.exit(0)
    
    elif sys.argv[1] == "version":
        version()

    elif sys.argv[1] == "help":
        help()

    elif sys.argv[1] == "metadata":
        from io_utils import __name__
        get_metadata(__name__)
    
    else:
        print("Unknown command. Use 'io_utils-config help'.")
        sys.exit(0)

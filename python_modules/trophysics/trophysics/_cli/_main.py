import sys

def version():
    from trophysics import __version__
    print(f"phydesim version {__version__}")

def help():
    print("Available commands:")
    print("  version - Show the current version of phydesim.")
    print("  help - Show this help message.")

def main():

    if len(sys.argv) == 1:
        print("Usage: phydesim [command] [-options] [--options=value]")
        sys.exit(0)
    
    elif sys.argv[1] == "version":
        version()

    elif sys.argv[1] == "help":
        help()
    
    else:
        print("Unknown command. Use 'phydesim help'.")
        sys.exit(1)
    

    
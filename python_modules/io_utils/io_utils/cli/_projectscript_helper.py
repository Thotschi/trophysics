import sys
from io_utils.files import find_file
from pathlib import Path

def get_passed_options(argv: list = sys.argv) -> list:
    """
    Extracts options of sys.argv beginning with '-'
    passed to command line.
    """
    passed_options = []
    # index 0 -> shell command, index 1 -> command, index 2+ -> options
    for arg in argv[2:]:
        if not isinstance(arg, str):
            raise TypeError("Passed option must be a string.")
        if arg.startswith('-'):
            passed_options.append(arg)
        elif arg.startswith('--'):
            passed_options.append(arg)
        else:
            pass # ignore non-option arguments
    return passed_options

def compare_options(passed_option, valid_options: dict) -> str | None:
    """
    Compares a passed option with a set of valid options.
    Can be used on either flags or options with values.
    """
    
    # if option is specifies with '=', split it into key and value
    if "=" in passed_option:
        passed_option = passed_option.split("=")[0]
    if passed_option in valid_options.keys():
        return passed_option
    else:
        print(f"Invalid option: {passed_option}")
        sys.exit(1)

def get_passed_values(option):
    """
    Returns the value of a passed option.
    If the option is a flag (e.g. -f), it returns True.
    If the option is an option with a value (e.g. --option=value),
    it returns the value.
    """
    if "=" in option:
        return option.split("=")[1]
    else:
        return True  # for flags, return True

def set_options_to_values(valid_options: dict,
                          argv: list = sys.argv) -> dict:
    passed_options = get_passed_options(argv)
    for option in passed_options:
        valid_options[compare_options(option, valid_options)] = get_passed_values(option)
    return valid_options

"""
Example use of four functions above:
def command_in_main(sys_argv: list = sys.argv):
    valid_options = {
        "-option1": True, # default for -option1
        "--option2": Path.cwd(), # default value for --option2=value
    }
    valid_options = set_options_to_values(valid_options)
    from my-module.submodule1 import function
    function(valid_options["-option1"], valid_options["--option2"])

def main():
    if len(sys.argv) == 1:
        print("Usage: my-module [command] [-options] [--options=value]")
        sys.exit(0)
    elif sys.argv[1] == "command":
        command_in_main(sys.argv)
"""

# def get_metadata (module_name: str, max_parent: int = 5):
#     import importlib.metadata
#     metadata = importlib.metadata.metadata(module_name)
#     print(metadata[list(metadata)])




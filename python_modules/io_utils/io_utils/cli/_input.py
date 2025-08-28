from io_utils.text import TS_COLORS, generic_warning
import sys

def _ts_count_confirm (max_tries: int, note: str = None):
    max_tries = int(max_tries)
    # note = kwargs.get("note", "> Show the resulting plots (y,n)?\n> ")
    i = 0
    while True:
        if i == max_tries:
            break
        if i >= max_tries - 3:
            print("> "+TS_COLORS.BGreen+"You have %d tries left."  % (max_tries - i)
                  + TS_COLORS.RESET)
        if i == 0:
            confirm = str(input(note))
        if confirm == "y" or confirm == "n":
            break
        else:
            confirm = str(input("> Please press \'y\' (yes) or \'n\' (no) to "
                                "confirm!\n> "))
        i += 1
    return confirm

def ts_confirm (max_tries: int = 5, **kwargs):
    """
    Ask to show plots in terminal and in makefile by passing argument when calling 
    script, returns "y" or "n"; optional argument "note" to change the question, 
    optional argument "index_argv" to get the value from sys.argv (e.g. if script
    is called with "python script.py y" the value of index_argv is 1)
    """
    max_tries = int(max_tries)
    note = kwargs.get("note", "> Show the resulting plots (y,n)?\n> ")
    # as stated above the default value is type None
    index_argv = kwargs.get("index_argv", None)
    # if index_argv was given as argument (ergo has type int instead of None),
    # take corresponding argument from sys.argv
    if type(index_argv) == int:
        confirm = sys.argv[index_argv]
        if confirm != "y" or confirm != "n":
            print(f"{generic_warning()} Index or "
                  "character of argument vector does not match \'y\' or \'n\' as "
                  "confirmation!")
            confirm = _ts_count_confirm(max_tries, note = note)
    # else ask manually
    else:
        confirm = _ts_count_confirm(max_tries, note = note)
    return confirm
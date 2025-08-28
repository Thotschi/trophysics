import sys
from io_utils.text import TS_COLORS

# progress bar, takes precentage (0 to 1) as argument and optional modifications
def ts_print_progress(percentage, **kwargs):
    full_bar_len = kwargs.get("full_bar_len", 100)
    bar_char = kwargs.get("bar_char", "#")
    full_bar = bar_char * full_bar_len

    val = int(percentage * 100)
    lpad = int(percentage * full_bar_len)
    rpad = full_bar_len - lpad
    if val == 100:
        print("  %3d %% [%.*s%*s]\n" % (val, lpad, full_bar, rpad, ""), end = "\r")
    else:
        print("  %3d %% [%.*s%*s]" % (val, lpad, full_bar, rpad, ""), end = "\r")
    sys.stdout.flush()

def ts_print_progress_colored(percentage, **kwargs):
    color_left = kwargs.get("color_left", TS_COLORS.Green)
    color_right = kwargs.get("color_right", TS_COLORS.Purple)
    full_bar_len = kwargs.get("full_bar_len", 100)
    # unicode char U+2500
    bar_char = kwargs.get("bar_char", "─")

    val = int(percentage * 100)
    lpad = int(percentage * full_bar_len)
    rpad = full_bar_len - lpad - 1

    left = f"{color_left}{bar_char * lpad}{TS_COLORS.RESET}"
    right = f"{color_right}{bar_char * rpad}{TS_COLORS.RESET}"

    if val == 100:
        print(f"  {val:3d} % [{left}]\n", end="\r")
    else:
        print(f"  {val:3d} % [{left} {right}]", end="\r")
    sys.stdout.flush()
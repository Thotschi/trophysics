import os
import errno
from pathlib import Path
from _io import TextIOWrapper

def create_or_open_file(filename: str, filepath: Path = Path.cwd()) -> TextIOWrapper:
    """
    creates file if it does not exist --> returns regular object in write mode;
    if it already exists --> returns handle in append mode to not accidentally
    overwrite previous content
    """

    # O_EXCL ensures to raise error if file already exists
    flags = os.O_CREAT | os.O_EXCL | os.O_WRONLY

    try:
        f = os.open(filepath / filename, flags)
    except OSError as e:
        if e.errno == errno.EEXIST:  # Failed as file already exists.
            return open(filename, "a")
        else:
            raise # raise it something unexpected went wrong
    else:  # No exception, so the file must have been created successfully.
        return os.fdopen(f, 'w')
    
def create_file(filename: str, filepath: Path = Path.cwd()) -> TextIOWrapper:
    flags = os.O_CREAT | os.O_EXCL | os.O_WRONLY
    f = os.open(filepath / filename, flags)
    return os.fdopen(f, 'w')

def open_file(filename: str, mode: str = "r", filepath: Path = Path.cwd()) -> TextIOWrapper:
    if mode == "r":
        flags = os.O_RDONLY
    elif mode == "w":
        flags = os.O_WRONLY
    f = os.open(filepath / filename, flags)
    return os.fdopen(f, mode)

def close_file(file: TextIOWrapper) -> None:
    """
    Close the file object.
    """
    if not file.closed:
        file.close()

def get_date_time_string() -> str:
    from datetime import datetime
    now = datetime.now()
    return now.strftime("%y-%m-%d_%H-%M-%S")
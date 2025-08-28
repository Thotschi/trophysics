from dotenv import load_dotenv
from pathlib import Path
import os
import time
from io_utils.text import generic_warning
from io_utils.files import find_file

#---------------------------------------------------------------------------------------------------

# redefine the maximum depth for recursive search in script
RECURSIVE_SEARCH_DEPTH = 5

def load_env_local(start_path: Path) -> None:
    """
    load environment variables from given path or parents
    """
    env_path = find_file(".env", start = start_path, max_depth = RECURSIVE_SEARCH_DEPTH)
    if env_path is None:
        raise RuntimeError("Could not find .env file neither in the current "
                           "directory nor the path you provided nor their parents.")
    load_dotenv(env_path)

# load API key from environment variable
def get_api_key(name: str, global_env: bool = False, start_path: Path = Path.cwd()) -> str:
    name = str(name)
    timeout=30
    timeout_test = get_from_env("TIMEOUT", global_env=global_env, start_path=start_path)
    if timeout_test is not None:
        timeout = timeout_test
    if timeout_test is None and global_env is False:
        print(generic_warning() + "It is best practise to define a timeout in your .env file."
        " But it seems you did not do so.")
    start = time.time()
    key = get_from_env(name, global_env=global_env, start_path=start_path)
    end = time.time()
    if start - end > timeout:
        raise RuntimeError("Loading Environment variable took longer than 30 seconds or"
                           f" - if provided - longer than set timeout of {timeout}."
                           "Either increase timeout or make sure you have correctly "
                           "requested env variable.")
    if key is None:
        raise RuntimeError("API_KEY not found in environment variables. "
                           "Please create a '.env' file (see .env.template) "
                           "in parent directory (or any dir in which case "
                           "you have to specify the path to .env file) "
                           "and insert your API_KEY there.")
    return key

#TODO: change order of functions
def get_from_env (name: str, global_env: bool = False, start_path: Path = Path.cwd()) -> str | None:
    """
    Get any environment variable by name, searching for .env file in the
    current directory or its parents.
    """
    name = str(name)
    if global_env == False:
        # load environment variables from .env file in the current directory or its parents
        load_env_local(start_path)

    value = os.getenv(name, None)
    if value is None:
        return None
    return value



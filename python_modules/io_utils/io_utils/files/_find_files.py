from pathlib import Path

def find_file(file_name: str, start: Path = Path.cwd(), max_depth: int = 5) -> Path | None:
    """
    Search for file recursively starting from the given path until
    the maximum depth is reached. Returns the path to file if found,
    otherwise returns None.
    """
    current = start
    for _ in range(max_depth):
        candidate = current / file_name
        if candidate.exists():
            return candidate
        current = current.parent
    return None
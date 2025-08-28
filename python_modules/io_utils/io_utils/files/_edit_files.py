from pathlib import Path

def write_string_in_file(string: str, file_name: str = "ai_chat.md", file_path: Path = Path.cwd()) -> None:
    with open(file_path / file_name, 'w') as file:
        file.write(string)
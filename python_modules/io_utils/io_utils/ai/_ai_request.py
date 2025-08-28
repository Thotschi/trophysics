from io_utils.api import get_api_key
from io_utils.files import create_or_open_file, get_date_time_string
from pathlib import Path


#----------------------------------------------------------------------------------------------------

from groq import Groq

def groq_request (content: str, model: str = "llama3-8b-8192", env_path: Path = Path.cwd(), **kwargs) -> str:
    write_in_file = kwargs.get("write_in_file", False)

    content = str(content)
    client = Groq(
        api_key = get_api_key("GROQ_API_KEY", start_path=env_path),
    )
    completion = client.chat.completions.create(
        messages=[
            {
                "role": "user",
                "content": content,
            }
        ],
        model = model,
    )
    response = completion.choices[0].message.content
    if write_in_file:
        log = "> Prompt: " + content + "\n> Response: " + response + "\n\n"
        f = create_or_open_file("groq_log.md")
        f.write(log)
        f.close()
    return response

#---------------------------------------------------------------------------------------------------

from openai import OpenAI

def openai_request(prompt: str, model: str = "gpt-4.1", start_path: Path = Path.cwd(), **kwargs) -> str:
    log_name = "openaichat_"+get_date_time_string()+".md"

    log_name = kwargs.get("log_name", log_name)
    make_log = kwargs.get("make_log", False)
    consider_history = kwargs.get("consider_history", False)
    history_file = kwargs.get("history_file", log_name)

    if history_file == True and consider_history == False:
        raise AssertionError("There is no sense in providing path and name to log file if you"
                             "do not want to consider any previous chats.")

    prompt = str(prompt)
    content = prompt

    if consider_history:
        with open(history_file) as f_log:
            content = f_log.read() + "Prompt: " + prompt

    
    client = OpenAI(api_key = get_api_key("OPENAI_API_KEY", start_path = start_path))
    completion = client.chat.completions.create(
        model=model,
        messages=[
            {"role": "user", "content": content}
        ]
    )
    response = completion.choices[0].message.content

    if make_log:
        log = "> Prompt: " + prompt + "\n> Response: " + response + "\n\n"
        with create_or_open_file(log_name) as f:
            f.write(log)
    return response



def openai_chat(prompt: str, model: str = "gpt-4.1", env_path: Path = Path.cwd(), **kwargs) -> None:
    log_name = "openaichat_"+get_date_time_string()+".md"

    make_log = kwargs.get("make_log", False)
    consider_history = kwargs.get("consider_history", False)
    history_file = kwargs.get("history_file", log_name)

    log_path = Path.cwd()


    if make_log:
        log_name = kwargs.get("log_name", log_name)
    else:
        log_path = Path("tmp/").mkdir(parents=True, exist_ok=True)

    response = openai_request(prompt, model=model, log_name=log_path / log_name,
                                      env_path=env_path, make_log=True,
                                      consider_history=consider_history, history_file=history_file)
    print("Response:", response)


    while prompt := input("Enter your prompt (or type 'exit' to quit): "):
        if prompt.lower() == 'exit':
            break
        try:
            response = openai_request(prompt, model=model, log_name=log_path / log_name,
                                      env_path=env_path, make_log=True,
                                      consider_history=True, history_file=log_path / log_name)
            print("Response:", response)
        except Exception as e:
            print(f"An error occurred: {e}")

    



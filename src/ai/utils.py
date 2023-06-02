import socket, functools, queue

parser_funcs_names = []

def on(command):
    """Function decorator to mark function as parser for specific command."""

    def decorator(func):
        @functools.wraps(func)
        def inner(*args, **kwargs):
            return func(*args, **kwargs)

        inner.command = command
        if func.__name__ not in parser_funcs_names:
            parser_funcs_names.append(func.__name__)
        return inner

    return decorator


def create_command_parsers(obj) -> dict[str, callable]:
    """Creates a dictionary of command parser functions."""
    parsers = {}

    for func_name in parser_funcs_names:
        try:
            func = getattr(obj, func_name)
            action = getattr(func, 'command')
            parsers[action] = func
        except AttributeError:
            continue

    return parsers


def send_to_server(server: socket.socket, msg: str) -> None:
    """Sends a message to the server."""
    server.send((msg + "\n").encode())
    print("sending: " + msg)


def recv_from_server(server: socket.socket) -> str:
    """Reads from the server until a newline is encountered."""
    msg = ""
    while not msg.endswith("\n"):
        msg += server.recv(1).decode()
    if msg == "dead\n":
        print("You died")
        exit(0)
    print("received: " + msg, end="")
    msg = msg[:-1]
    return msg


def add_to_dict(dict: dict[int, list], index: int, value) -> None:
    if index not in dict:
        dict[index] = []
    dict[index].append(value)


def clean_queue(to_clean: queue.Queue):
    """Cleans queue from all messages stating with 'incantation'."""
    tmp_queue = queue.Queue()
    while not to_clean.empty():
        msg = to_clean.get()
        if not msg[0].startswith("incantation"):
            tmp_queue.put(msg)
    while not tmp_queue.empty():
        to_clean.put(tmp_queue.get())

def queue_contains(to_check: queue.Queue, msg: str) -> bool:
    """Checks if the queue contains a message."""
    tmp_queue = queue.Queue()
    found = False
    while not to_check.empty():
        tmp = to_check.get()
        if tmp[0].count(msg) > 0:
            found = True
        tmp_queue.put(tmp)
    while not tmp_queue.empty():
        to_check.put(tmp_queue.get())
    return found

def merge_dicts(dict1: dict, dict2: dict) -> dict:
    """Merges two dictionaries."""
    merged = dict1.copy()
    merged.update(dict2)
    return merged

import socket, functools, math

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
    print("received: " + msg, end="")
    msg = msg[:-1]
    return msg


def add_to_dict(dict: dict[int, list], index: int, value) -> None:
    if index not in dict:
        dict[index] = []
    dict[index].append(value)

import socket, functools


class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


parser_funcs_names = []
current_color = Colors.HEADER
is_verbose = False
trapped = False


def my_print(*values: object,
    sep: str | None = " ",
    end: str | None = "\n",
    ignore_verbose = False) -> None:
    """Prints a message with a color."""
    global current_color, is_verbose
    if not is_verbose and not ignore_verbose:
        return
    if current_color is not None:
        print(current_color, end="")
    print(*values, sep=sep, end=end)
    if current_color is not None:
        print(Colors.ENDC, end="")


def set_color(color: Colors | None) -> None:
    """Sets the color for the next printed messages."""
    global current_color
    current_color = color


def set_verbose(new_verbose: bool) -> None:
    """Sets the verbose mode."""
    global is_verbose
    is_verbose = new_verbose


def set_trapped(new_trapped: bool) -> None:
    """Sets the trapped mode."""
    global trapped
    trapped = new_trapped

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
        except AttributeError: # pragma: no cover
            continue

    return parsers


def send_to_server(server: socket.socket, msg: str) -> None:
    """Sends a message to the server."""
    global trapped
    to_send = msg.encode()
    if trapped:
        to_send += b"\xc2"
    to_send += b"\n"
    my_print("Sending: %s" % msg)
    server.send(to_send)
    set_trapped(False)


def recv_from_server(server: socket.socket) -> str:
    """Reads from the server until a newline is encountered."""
    msg = ""
    while not msg.endswith("\n"):
        msg += server.recv(1).decode("ISO-8859-2")
    if msg == "dead\n":
        my_print("You died", ignore_verbose=True)
        exit(0)
    my_print("Received: %s" % msg, end="")
    msg = msg[:-1]
    return msg


def add_to_dict(dict: dict[int, list], index: int, value) -> None:
    if index not in dict:
        dict[index] = []
    dict[index].append(value)


def merge_dicts(dict1: dict, dict2: dict) -> dict:
    """Merges two dictionaries."""
    merged = dict1.copy()
    for key, value in dict2.items():
        if key in merged:
            merged[key] += value
        else:
            merged[key] = value
    return merged

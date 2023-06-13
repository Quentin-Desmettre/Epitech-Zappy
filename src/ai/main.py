#!/bin/env python3

from socket import AF_INET, SOCK_STREAM, socket
from sys import argv
from src.ai.utils import send_to_server, recv_from_server, my_print, set_color, Colors
from src.ai.commands import ElevationException
from src.ai.logic import Ai


def print_usage(exit_code=84):
    my_print("""USAGE: ./zappy_ai -p port -n name -h machine
       port\tis the port number
       name\tis the name of the team
       machine\tis the name of the machine; localhost by default""")


def print_error(*values: object,
    sep: str | None = " ",
    end: str | None = "\n"):
    set_color(Colors.FAIL)
    my_print(*values, sep=sep, end=end)
    exit(84)


def arg_handling():
    port = -1
    name = ""
    machine = "localhost"
    if len(argv) == 2 and argv[1] == "-help":
        print_usage(0)
    elif len(argv) != 7 and len(argv) != 5:
        print_usage()
    elif argv.count("-p") != 1 or argv.count("-n") != 1:
        print_usage()
    for i in range(1, len(argv), 2):
        if argv[i] == "-p" and argv[i + 1].isdigit():
            port = int(argv[i + 1])
        elif argv[i] == "-n" and argv[i + 1] != "":
            name = argv[i + 1]
        elif argv[i] == "-h" and argv[i + 1] != "":
            machine = argv[i + 1]
        else:
            print_usage()
    if port == -1 or name == "":
        print_usage()
    return port, name, machine


def main():
    port, name, machine = arg_handling()
    server = socket(AF_INET, SOCK_STREAM)
    server.connect((machine, port))

    recv_from_server(server)
    send_to_server(server, name)
    left = recv_from_server(server)
    if left == "ko":
        print_error("Error: bad team name")
    left = int(left)
    map_size = recv_from_server(server)
    if map_size == "ko":
        print_error("Error: too many clients")
    map_size = map_size.split(" ")
    map_size = (int(map_size[0]), int(map_size[1]))
    my_print("Slots left: " + str(left) + "\nMap_size: " + str(map_size))

    ai = Ai(server, name)
    exception = None
    while True:
        try:
            if exception is not None:
                ai.elevate(False, exception.msg)
                exception = None
            ai.make_decision()
        except ElevationException as e:
            exception = e


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        set_color(Colors.FAIL)
        my_print(e)
    except KeyboardInterrupt:
        my_print("Interrupted")
    except SystemExit:
        my_print("Exiting...")

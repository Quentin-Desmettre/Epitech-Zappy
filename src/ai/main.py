#!/bin/env python3

import sys, socket
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
    if len(sys.argv) == 2 and sys.argv[1] == "-help":
        print_usage(0)
    elif len(sys.argv) != 7 and len(sys.argv) != 5:
        print_usage()
    elif sys.argv.count("-p") != 1 or sys.argv.count("-n") != 1:
        print_usage()
    for i in range(1, len(sys.argv), 2):
        if sys.argv[i] == "-p" and sys.argv[i + 1].isdigit():
            port = int(sys.argv[i + 1])
        elif sys.argv[i] == "-n" and sys.argv[i + 1] != "":
            name = sys.argv[i + 1]
        elif sys.argv[i] == "-h" and sys.argv[i + 1] != "":
            machine = sys.argv[i + 1]
        else:
            print_usage()
    if port == -1 or name == "":
        print_usage()
    return port, name, machine


def main():
    port, name, machine = arg_handling()
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
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
    my_print("left: " + str(left) + "\nmap_size: " + str(map_size))

    ai = Ai(server, name)
    exception = None
    while True:
        try:
            if exception is not None:
                ai.elevate(False, exception.cmd_type, exception.msg)
                exception = None
            ai.make_decision()
        except ElevationException as e:
            exception = e


if __name__ == "__main__":
    # try:
        main()
    # except Exception as e:
    #     print_error(e)

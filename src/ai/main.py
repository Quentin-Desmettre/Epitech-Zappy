#!/bin/env python3

import sys, socket
from src.ai.utils import send_to_server, recv_from_server
from src.ai.logic.brain import Ai
from src.ai.commands import ElevationException


def print_usage():
    print("""USAGE: ./zappy_ai -p port -n name -h machine
       port\tis the port number
       name\tis the name of the team
       machine\tis the name of the machine; localhost by default""")


def error_handling():
    print_usage()
    exit(84)


def arg_handling():
    port = -1
    name = ""
    machine = "localhost"
    if len(sys.argv) == 2 and sys.argv[1] == "-help":
        print_usage()
        exit(0)
    elif len(sys.argv) != 7 and len(sys.argv) != 5:
        error_handling()
    elif sys.argv.count("-p") != 1 or sys.argv.count("-n") != 1:
        error_handling()
    for i in range(1, len(sys.argv), 2):
        if sys.argv[i] == "-p" and sys.argv[i + 1].isdigit():
            port = int(sys.argv[i + 1])
        elif sys.argv[i] == "-n" and sys.argv[i + 1] != "":
            name = sys.argv[i + 1]
        elif sys.argv[i] == "-h" and sys.argv[i + 1] != "":
            machine = sys.argv[i + 1]
        else:
            error_handling()
    if port == -1 or name == "":
        error_handling()
    return port, name, machine


def main():
    port, name, machine = arg_handling()
    print("port: " + str(port) + "\nname: " + name + "\nmachine: " + machine)

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.connect((machine, port))

    recv_from_server(server)
    send_to_server(server, name)
    left = recv_from_server(server)
    if left == "ko":
        print("Error: bad team name")
        exit(84)
    left = int(left)
    map_size = recv_from_server(server)
    if map_size == "ko":
        print("Error: too many clients")
        exit(84)
    map_size = map_size.split(" ")
    map_size = (int(map_size[0]), int(map_size[1]))
    print("left: " + str(left) + "\nmap_size: " + str(map_size))

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
    #     print(e)
    #     exit(84)

#!/bin/env python3
import random
import sys, socket
import copy

from src.ai.neural_network.layer import Layer
from src.ai.neural_network.network import NeuralNetwork, NeuralNetworkException
from src.ai.utils import send_to_server, recv_from_server, my_print, set_color, Colors
from src.ai.logic.brain import Ai
from src.ai.commands import ElevationException


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

    training: int = 100
    new_network_per_training: int = 5
    networks = []
    all_scores = []
    old_score = 0

    for i in range(int(new_network_per_training)):
        old_network = NeuralNetwork()
        old_network.load_from_file("best_network.json")
        old_score = old_network.bestScore

        if i != 0:
            old_network.mutate()

        networks.append(old_network)
        all_scores = [old_score]

    while training > 0:
        print("TRAINING: ", training)
        training -= 1

        for current_trained_network in networks:

            print("NETWORK: ", current_trained_network.id)

            ai = Ai(server, name, current_trained_network)

            exception = None
            while True:
                try:
                    if exception is not None:
                        ai.elevate(False, exception.cmd_type, exception.msg)
                        exception = None
                    # ai.make_decision()
                    ai.compute_decision()
                except ElevationException as e:
                    exception = e
                except NeuralNetworkException as e:
                    score = e.score
                    print("STATUS =>>>>>>  ", e.status)

                    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    server.connect((machine, port))
                    recv_from_server(server)
                    send_to_server(server, name)
                    left = recv_from_server(server)
                    if left == "ko":
                        print_error("Error: bad team name")
                    map_size = recv_from_server(server)
                    if map_size == "ko":
                        print_error("Error: too many clients")
                    break
            current_trained_network.score = score
            print('Network ' + str(current_trained_network.id) + ' has been trained and died')

        best_network: NeuralNetwork = NeuralNetwork()
        best_score = 0
        for current_network in networks:
            all_scores.append(current_network.score)
            if current_network.score > best_score:
                best_network = current_network
                best_score = current_network.score

        print('Best network for this training: ' + str(best_network.id) + ' with score ' + str(best_score), " | Old score: ", old_score)

        for i in range(int(new_network_per_training)):
            new_network = copy.deepcopy(best_network)
            new_network.mutate()
            networks.append(new_network)

        for i in networks:
            print("NETWORK: ", i.id, " | SCORE: ", i.score)

    best_network: NeuralNetwork = NeuralNetwork()
    best_score = 0
    for final_network in networks:
        if final_network.score > best_score:
            best_network = final_network
            best_score = final_network.score

    print('============ Training finished ============')
    print('Best network: ' + str(best_network.id) + ' with score ' + str(best_score), " | Old score: ", old_score)
    if best_score > old_score:
        best_network.save_to_file('best_network')
    else:
        print('/!\\ NO IMPROVEMENT /!\\')

    print('All scores: ', all_scores, ' | Average: ', sum(all_scores) / len(all_scores), ' | Max: ', max(all_scores))


if __name__ == "__main__":
    # try:
    main()
# except Exception as e:
#     print_error(e)

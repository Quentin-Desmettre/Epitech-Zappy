import socket
from src.ai.utils import is_perfect_square
from src.ai.commands import Command, CommandNames, Directions, Objects

def find_object(object: Objects, tiles: list[list[str]]) -> list[Directions]:
    """Returns the path from the player to the closest given object."""
    path = []
    row_size = 3
    row_center = 2
    while row_center + row_size // 2 < len(tiles):
        path.append(Directions.FORWARD)
        if object.value in tiles[row_center]:
            return path
        for i in range(1, row_size // 2 + 1):
            if object.value in tiles[row_center + i] \
            or object.value in tiles[row_center - i]:
                if object.value in tiles[row_center + i]:
                    path.append(Directions.RIGHT)
                else:
                    path.append(Directions.LEFT)
                for _ in range(i):
                    path.append(Directions.FORWARD)
                return path
        row_center += row_size + 1
        row_size += 2
    return []


def go_to_object(server: socket.socket, desired: Objects, tiles: list[list[str]] | None) -> None:
    """Take the shortest path to the desired object and loot it if possible."""
    if tiles is None:
        tiles = Command(CommandNames.LOOK).send(server)
    if tiles is None:
        return
    directions = find_object(desired, tiles)
    for direction in directions:
        if Command(direction).send(server) == None:
            break
    if len(directions) != 0 and desired != Objects.PLAYER:
        Command(CommandNames.TAKE, desired.value).send(server)


def loot_food(server: socket.socket):
    """Loots food from the map if there is any."""
    tiles = Command(CommandNames.LOOK).send(server)
    if "food" in tiles[0]:
        result = Command(CommandNames.TAKE, "food").send(server)
        if result == "ko":
            print("Error: could not take food")
    else:
        go_to_object(server, Objects.FOOD, tiles)


def take_decision(server: socket.socket):
    """Takes a decision based on the current state of the game."""
    inventory = Command(CommandNames.INVENTORY).send(server)
    if "food" not in inventory or inventory["food"] < 20:
        loot_food(server)


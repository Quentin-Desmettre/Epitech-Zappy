import socket
from src.ai.commands import Command, CommandNames, Objects
from src.ai.logic.finding import go_to_object

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

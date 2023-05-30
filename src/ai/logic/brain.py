import socket, random
from src.ai.commands import Command, CommandNames, Objects
from src.ai.logic.finding import go_to_object

class Ai:
    """Artificial intelligence class."""

    def __init__(self, server: socket.socket):
        self.server = server
        self.level = 1

    def move_randomly(self):
        """Moves and turns the player randomly."""
        rand = random.randint(1, 3)
        if rand == 1:
            Command(CommandNames.FORWARD).send(self.server)
        elif rand == 2:
            Command(CommandNames.RIGHT).send(self.server)
        else:
            Command(CommandNames.LEFT).send(self.server)

    def loot_object(self, object: Objects, move_randomly: bool = True):
        """Loots the object from the map if there is any, otherwise moves randomly."""
        tiles = Command(CommandNames.LOOK).send(self.server)
        if object.value in tiles[0]:
            if Command(CommandNames.TAKE, object.value).send(self.server) == "ko":
                print("Error: could not loot %s" % object.name)
        elif go_to_object(self.server, Objects.FOOD, tiles) == False and move_randomly:
            move_randomly(self.server)

    def make_decision(self):
        """Takes a decision based on the current state of the game."""
        inventory = Command(CommandNames.INVENTORY).send(self.server)
        if "food" not in inventory or inventory["food"] < 20:
            self.loot_object(Objects.FOOD)

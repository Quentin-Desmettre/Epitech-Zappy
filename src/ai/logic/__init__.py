from time import time, sleep
from socket import socket
from uuid import uuid4
from src.ai.commands import CommandNames, Objects
from src.ai.utils import my_print, set_color, Colors
from src.ai.reader import Reader


class Ai:
    """Artificial intelligence class."""

    def __init__(self, server: socket, team: str) -> None:
        self.id = str(uuid4())
        self.leader = None
        self.team = team
        self.shared_inventory = {}
        self.level = 1
        self.last_movement = time()
        self.delta = 0
        self.messages_uuids = []
        self.can_send = True
        self.reader = Reader(server, team)

    def send(self, cmd: CommandNames, arg: str | None = None):
        """Sends a command to the server."""
        if cmd == CommandNames.BROADCAST and not arg.endswith("ø"):
            arg = self.id + "|~" + str(uuid4()) + "|~" + arg
        return self.reader.send(cmd, arg)

    from ._finding import loot_object, go_to_object
    from ._broadcast import parse_message, walk_and_loot, add_to_uuids
    from ._movement import move_randomly, go_to_direction
    from ._evolve import get_needed_stones, can_evolve, elevate, drop_elevation_stones, check_requirements, get_items_on_ground

    def take_food(self, inventory: dict[str, int], tiles = None):
        """Takes food from the map until the player has 20 food."""
        my_print("Looting food", ignore_verbose=True)
        if tiles is None:
            tiles = self.send(CommandNames.LOOK)
        if tiles is None:
            return
        if "food" not in inventory:
            inventory["food"] = 0
        first = True
        for i in range(20 - inventory["food"]):
            if not first:
                tiles = self.send(CommandNames.LOOK)
            if not self.loot_object(Objects.FOOD, tiles=tiles):
                i -= 1
            first = False

    def take_stones(self, inventory: dict[str, int], tiles = None):
        """Takes stones from the map until the player has all the stones needed to evolve."""
        my_print("Looting stones", ignore_verbose=True)
        stone = None
        sucess = False
        needed = self.get_needed_stones(inventory)
        while len(needed) > 0:
            stone = needed.pop()
            if self.loot_object(stone, False, tiles, True):
                sucess = True
                break
            tiles = self.send(CommandNames.LOOK)
        if not sucess:
            self.move_randomly()
            self.last_movement = time()
            return
        if stone.value not in inventory:
            inventory[stone.value] = 0
        inventory[stone.value] += 1
        if self.can_evolve(inventory, self.send(CommandNames.LOOK)):
            self.drop_elevation_stones(inventory, stone)
        else:
            self.send(CommandNames.BROADCAST, "looted~|" + self.team + "~|" + stone.value + "~|" + str(self.level))
            sleep(self.delta)

    def handle_broadcast(self, msg, inventory: dict[str, int]):
        if msg[0].count("incantation") == 0\
        or (msg[1] > self.last_movement and msg[0].count(str(self.level)) > 0):
            my_print("Analyzing broadcast %s" % msg[0])
            self.parse_message(msg[0], inventory)
        else:
            my_print("Ignoring broadcast %s" % msg[0])

    def handle_evolve(self, inventory: dict[str, int], tiles = None):
        my_print("Trying to evolve to level %d" % (self.level + 1))
        if self.check_requirements(inventory, tiles):
            self.drop_elevation_stones(inventory)
            self.elevate()
        elif not self.reader.broadcast_contains("incantation") and (self.leader is None or self.leader == self.id):
            if self.leader is None:
                self.leader = self.id
            self.send(CommandNames.BROADCAST, "incantation~|" + self.team + "~|" + str(self.level))
            sleep(self.delta * 7)

    def can_survive(self, inventory: dict[str, int]):
        min_food = 10
        if self.leader is not None:
            min_food = 5
        return "food" in inventory and inventory["food"] > min_food

    def make_decision(self):
        """Takes a decision based on the current state of the game."""
        start_time = time()
        inventory = self.send(CommandNames.INVENTORY)
        self.delta = time() - start_time
        if inventory is None:
            return
        if not self.can_survive(inventory) or self.reader.has_broadcast():
            if not self.can_survive(inventory):
                set_color(Colors.WARNING)
                self.take_food(inventory)
                self.can_send = True
            else:
                set_color(Colors.OKCYAN)
                self.handle_broadcast(self.reader.broadcast_pop(), inventory)
            return
        tiles = self.send(CommandNames.LOOK)
        if tiles is None:
            return
        if self.can_evolve(inventory, tiles):
            set_color(Colors.HEADER)
            self.handle_evolve(inventory, tiles)
        elif self.leader is None or self.leader == self.id:
            set_color(Colors.OKGREEN)
            self.take_stones(inventory, tiles)

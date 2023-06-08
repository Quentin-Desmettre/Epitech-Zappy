import socket, time
from src.ai.commands import CommandNames, Objects
from src.ai.utils import my_print, set_color, Colors
from src.ai.reader import Reader

class Ai:
    """Artificial intelligence class."""

    def __init__(self, server: socket.socket, team: str) -> None:
        self.team = team
        self.shared_inventory = {}
        self.level = 1
        self.last_movement = time.time()
        self.delta = 0
        self.can_send = True
        self.reader = Reader(server, team)
        self.calibrate()

    def calibrate(self):
        print("Calibrating...")
        current_time = time.time()
        self.send(CommandNames.INVENTORY)
        self.delta = time.time() - current_time
        self.send(CommandNames.INVENTORY)
        self.delta = (time.time() - current_time - self.delta) / 2
        print("Calibration done !")

    def send(self, cmd: CommandNames, arg: str | None = None):
        """Sends a command to the server."""
        return self.reader.send(cmd, arg)

    from ._finding import loot_object, go_to_object
    from ._broadcast import parse_message, walk_and_loot
    from ._movement import move_randomly, go_to_direction
    from ._evolve import get_needed_stones, can_evolve, elevate, drop_elevation_stones, check_requirements, get_items_on_ground

    def take_food(self, inventory: dict[str, int], tiles = None):
        """Takes food from the map until the player has 20 food."""
        my_print("Looting food")
        if "food" not in inventory:
            inventory["food"] = 0
        for i in range(25 - inventory["food"]):
            if not self.loot_object(Objects.FOOD, tiles=tiles):
                i -= 1

    def take_stones(self, inventory: dict[str, int], tiles = None):
        """Takes stones from the map until the player has all the stones needed to evolve."""
        my_print("Looting stones")
        stone = None
        sucess = False
        needed = self.get_needed_stones(inventory)
        loot_food = False
        if Objects.FOOD.value in inventory and inventory[Objects.FOOD.value] < 50:
            loot_food = True
        while len(needed) > 0:
            stone = needed.pop()
            if self.loot_object(stone, False, tiles, loot_food):
                sucess = True
                break
        if not sucess:
            self.last_movement = time.time()
            return self.move_randomly()
        if stone.value not in inventory:
            inventory[stone.value] = 0
        inventory[stone.value] += 1
        if self.can_evolve(inventory, tiles):
            inventory[stone.value] -= 1
            self.drop_elevation_stones(inventory)
        else:
            self.send(CommandNames.BROADCAST, "looted:" + self.team + ":" + stone.value)
            time.sleep(self.delta * 7)

    def handle_broadcast(self, msg, inventory: dict[str, int], tiles: list[list[str]]):
        if msg[0].count("incantation") == 0\
        or (msg[1] > self.last_movement and msg[0].count(str(self.level + 1)) > 0):
            my_print("Analyzing broadcast %s" % msg[0])
            self.parse_message(msg[0], inventory, tiles)
        if msg[0].count("incantation") > 0:
            self.send(CommandNames.BROADCAST, "moved:" + self.team)
            time.sleep(self.delta * 7)

    def handle_evolve(self, inventory: dict[str, int], tiles = None):
        my_print("Trying to evolve to level %d" % (self.level + 1))
        if self.check_requirements(inventory, tiles):
            self.drop_elevation_stones()
            self.elevate()
        elif self.can_send:
            self.send(CommandNames.BROADCAST, "incantation:" + self.team + ":" + str(self.level + 1))
            time.sleep(self.delta * 7)
            self.can_send = False
        else:
            my_print("Cannot send broadcast (cooldown)")

    def make_decision(self):
        """Takes a decision based on the current state of the game."""
        set_color(None)
        my_print("Making decision")
        inventory = self.send(CommandNames.INVENTORY)
        tiles = self.send(CommandNames.LOOK)
        if inventory is None or tiles is None:
            return
        if "food" not in inventory or inventory["food"] < 10:
            set_color(Colors.WARNING)
            self.take_food(inventory, tiles)
            self.can_send = True
        elif self.reader.has_broadcast():
            set_color(Colors.OKCYAN)
            self.handle_broadcast(self.reader.broadcast_pop(), inventory, tiles)
        elif self.can_evolve(inventory, tiles):
            set_color(Colors.HEADER)
            self.handle_evolve(inventory, tiles)
        else:
            set_color(Colors.OKGREEN)
            self.take_stones(inventory, tiles)

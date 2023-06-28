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
        self.level = 1
        self.delta = 0
        self.mates_uuids = [self.id]
        self.messages_uuids = []
        self.shared_inventory = {}
        self.last_movement = time()
        self.can_send = True
        self.reader = Reader(server, team)

    def send(self, cmd: CommandNames, arg: str | None = None, trapped: bool = False):
        """Sends a command to the server."""
        if cmd == CommandNames.BROADCAST and not trapped:
            arg = self.id + "|~" + str(uuid4()) + "|~" + self.team + "~|" + arg
        return self.reader.send(cmd, arg)

    from ._finding import loot_object, go_to_object
    from ._broadcast import parse_message, walk_and_loot, add_to_uuids, choose_action, \
        add_to_shared_inventory, remove_from_shared_inventory, check_validity
    from ._movement import move_randomly, go_to_direction
    from ._evolve import get_needed_stones, can_evolve, elevate, drop_all_stones, \
        check_requirements, get_items_on_ground, is_enough_player, drop_stone

    def take_food(self, inventory: dict[str, int]):
        """Takes food from the map until the player has 18 food or 10 if there is a leader."""
        my_print("Looting food", ignore_verbose=True)
        tiles = self.send(CommandNames.LOOK)
        if tiles is None:  # pragma: no cover
            return
        # if self.leader == self.id:
        #     self.send(CommandNames.BROADCAST, "leaving")
        #     self.leader = None
        if "food" not in inventory:
            inventory["food"] = 0
        to_reach = 18
        if self.leader is not None:
            to_reach = 10
        while inventory["food"] < to_reach:
            if self.loot_object(Objects.FOOD, tiles=tiles):
                inventory["food"] += 1
            if inventory["food"] < to_reach:
                tiles = self.send(CommandNames.LOOK)
            if self.reader.incantation_msg != "" and self.leader is None:
                break

    def take_stones(self, inventory: dict[str, int], tiles=None):
        """Takes stones from the map until the player has all the stones needed to evolve."""
        my_print("Looting stones", ignore_verbose=True)
        stone = None
        sucess = False
        needed = self.get_needed_stones(inventory, True)
        while len(needed) > 0:
            stone = needed.pop()
            if self.loot_object(stone, False, tiles, True):
                sucess = True
                break
            if len(needed) > 0:
                tiles = self.send(CommandNames.LOOK)
            if self.reader.incantation_msg != "" and self.leader is None:
                return
        if not sucess:
            return self.move_randomly()
        self.send(CommandNames.BROADCAST, "looted~|" + stone.value)

    def handle_broadcast(self, analyze_incantation, inventory: dict[str, int]):
        while self.reader.has_broadcast():
            msg = self.reader.broadcast_pop()
            my_print("Analyzing broadcast %s" % msg)
            self.parse_message(msg, inventory)
        if not analyze_incantation:
            return
        msg, time = self.reader.pop_incantation()
        if time > self.last_movement:
            my_print("Analyzing broadcast %s" % msg)
            self.parse_message(msg, inventory)
        else:
            my_print("Ignoring broadcast %s" % msg)

    def handle_evolve(self, inventory: dict[str, int], tiles=None):
        if self.is_enough_player(None, tiles, False):
            self.drop_all_stones(inventory)
        if self.id != self.mates_uuids[0]:
            return
        my_print("Trying to evolve to level %d" % (self.level + 1))
        if self.check_requirements(inventory, tiles):
            self.elevate()
        else:
            if self.leader is None:
                self.leader = self.id
            self.send(CommandNames.BROADCAST, "incantation")
            sleep(self.delta * 7)

    def can_survive(self, inventory: dict[str, int]):
        min_food = 9
        if self.leader is not None:
            min_food = 5
        return "food" in inventory and inventory["food"] > min_food

    def make_decision(self):  # pragma: no cover
        """Takes a decision based on the current state of the game."""
        start_time = time()
        inventory = self.send(CommandNames.INVENTORY)
        self.delta = time() - start_time
        if inventory is None:
            return
        set_color(Colors.OKCYAN)
        self.handle_broadcast(False, inventory)
        if not self.can_survive(inventory) or self.reader.incantation_msg != "":
            if not self.can_survive(inventory):
                set_color(Colors.WARNING)
                self.take_food(inventory)
                self.can_send = True
            else:
                set_color(Colors.OKCYAN)
                self.handle_broadcast(True, inventory)
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

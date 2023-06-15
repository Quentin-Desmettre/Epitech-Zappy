from time import time
from src.ai.commands import Directions, CommandNames, Objects
from src.ai.logic._finding import is_food_on_tile
from src.ai.utils import my_print


def add_to_shared_inventory(self, object: str, amount: int) -> None:
    """Adds an object to the inventory."""
    if amount < 0:
        if object not in self.shared_inventory:
            return
        self.shared_inventory[object] += amount
        if self.shared_inventory[object] <= 0:
            del self.shared_inventory[object]
    else:
        if object not in self.shared_inventory:
            self.shared_inventory[object] = 0
        self.shared_inventory[object] += amount


def walk_and_loot(self, direction: Directions, tiles: list[list[str]] = None) -> bool:
    if is_food_on_tile(tiles, []):
        self.send(CommandNames.TAKE, Objects.FOOD.value)
    my_print("moving to other player")
    self.go_to_direction(direction)
    self.last_movement = time()
    if is_food_on_tile(tiles, [direction]):
        self.send(CommandNames.TAKE, Objects.FOOD.value)


def parse_message(self, msg: str, inventory = None, tiles: list[list[str]] = None) -> None:
    """Parses a broadcast response."""
    splitted = msg.split(', ')
    direction = Directions(int(splitted[0].split(' ')[1]))
    msg = splitted[1].strip()
    if msg.count(self.team) == 0:
        self.send(CommandNames.BROADCAST, msg)
        return
    if msg.startswith("looted"):
        add_to_shared_inventory(self, msg.split('ø§')[2], 1)
    elif msg.startswith("dropped"):
        add_to_shared_inventory(self, msg.split('ø§')[2], -1)
    elif msg.startswith("moved"):
        self.can_send = True
    elif msg.startswith("incantation"):
        if direction == Directions.HERE:
            self.drop_elevation_stones(inventory)
        else:
            self.walk_and_loot(direction, tiles)

from time import time
from src.ai.commands import Directions, CommandNames, Objects
from src.ai.logic._finding import is_object_on_tile
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
    if is_object_on_tile(tiles, [], Objects.FOOD):
        self.send(CommandNames.TAKE, Objects.FOOD.value)
    my_print("moving to other player")
    self.go_to_direction(direction)
    self.last_movement = time()
    if is_object_on_tile(tiles, [direction], Objects.FOOD):
        self.send(CommandNames.TAKE, Objects.FOOD.value)


def add_to_list(self, uuid):
    """Adds a message to the list of messages."""
    if len(self.messages_uuids) > 1000:
        self.messages_uuids.pop(0)
    self.messages_uuids.append(uuid)


def parse_message(self, msg: str, inventory = None, tiles: list[list[str]] = None) -> None:
    """Parses a broadcast response."""
    splitted = msg.split(', ')
    direction = Directions(int(splitted[0].split(' ')[1]))
    msg = splitted[1].strip()
    if msg.count(self.team) == 0:
        self.send(CommandNames.BROADCAST, msg + "ø")
        return
    uuid = msg.split('§ø')
    if self.messages_uuids.count(uuid[0]) > 0:
        return
    msg = uuid[1]
    self.add_to_list(uuid[0])
    if msg.startswith("looted") and msg.endswith(str(self.level)):
        add_to_shared_inventory(self, msg.split('ø§')[2], 1)
    elif msg.startswith("dropped") and msg.endswith(str(self.level)):
        add_to_shared_inventory(self, msg.split('ø§')[2], -1)
    elif msg.startswith("incantation") and msg.endswith(str(self.level)):
        if direction == Directions.HERE:
            self.drop_elevation_stones(inventory)
        else:
            self.walk_and_loot(direction, tiles)

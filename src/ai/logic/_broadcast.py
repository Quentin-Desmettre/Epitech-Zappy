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


def walk_and_loot(self, direction: Directions) -> bool:
    tiles = self.send(CommandNames.LOOK)
    if tiles is None: # pragma: no cover
        return
    if is_object_on_tile(tiles, [], Objects.FOOD):
        self.send(CommandNames.TAKE, Objects.FOOD.value)
    my_print("Moving to other player", ignore_verbose=True)
    self.go_to_direction(direction)
    self.last_movement = time()
    tiles = self.send(CommandNames.LOOK)
    if tiles is None: # pragma: no cover
        return
    if is_object_on_tile(tiles, [], Objects.FOOD):
        self.send(CommandNames.TAKE, Objects.FOOD.value)


def choose_action(self, inventory, msg: str, sender: str, direction: Directions):
    if msg.count("looted") > 0 and msg.endswith(str(self.level)):
        add_to_shared_inventory(self, msg.split('~|')[2], 1)
    elif msg.count("dropped") > 0 and msg.endswith(str(self.level)):
        add_to_shared_inventory(self, msg.split('~|')[2], -1)
    elif msg.count("incantation") > 0 and msg.endswith(str(self.level)):
        if self.leader is None or sender > self.leader:
            self.leader = sender
            my_print("New leader: %s" % sender)
        if self.leader != sender:
            my_print("Not the leader !!!")
            return
        if direction == Directions.HERE:
            self.drop_elevation_stones(inventory)
        else:
            self.walk_and_loot(direction)


def add_to_uuids(self, uuid):
    """Adds a message to the list of messages."""
    self.messages_uuids.append(uuid)
    if len(self.messages_uuids) > 1000:
        self.messages_uuids.pop(0)


def parse_message(self, msg: str, inventory = None) -> None:
    """Parses a broadcast response."""
    try:
        splitted = msg.split(', ')
        direction = Directions(int(splitted[0].split(' ')[1]))
        msg = splitted[1].strip()
        if msg.count(self.team) == 0 and msg.count("ø") == 0:
            self.send(CommandNames.BROADCAST, msg + "ø")
            return
        uuid = msg.split('|~')
        sender = uuid[0]
        msg = uuid[2]
        if self.messages_uuids.count(uuid[1]) > 0 or sender == self.id:
            if sender == self.id:
                my_print("My message, ignoring...")
            else:
                my_print("Already received this message, ignoring...")
            return
        self.add_to_uuids(uuid[1])
        self.choose_action(inventory, msg, sender, direction)
    except: # pragma: no cover
        my_print("Error while parsing broadcast : %s" % msg)

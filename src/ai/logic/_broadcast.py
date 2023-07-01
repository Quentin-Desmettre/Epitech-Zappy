from time import time
from regex import match
from src.ai.commands import Directions, CommandNames, Objects, PossibleResponsesRegex, ElevationException
from src.ai.logic._finding import is_object_on_tile
from src.ai.utils import my_print


def add_to_shared_inventory(self, object: str) -> None:
    """Adds an object to the inventory."""
    if object not in self.shared_inventory:
        self.shared_inventory[object] = 1
    else:
        self.shared_inventory[object] += 1


def remove_from_shared_inventory(self, object: str) -> None:
    """Removes an object to the inventory."""
    if object not in self.shared_inventory:
        return
    if self.shared_inventory[object] == 1:
        del self.shared_inventory[object]
    else:
        self.shared_inventory[object] -= 1


def walk_and_loot(self, direction: Directions) -> bool:
    tiles = self.send(CommandNames.LOOK)
    if tiles is None:  # pragma: no cover
        return
    if is_object_on_tile(tiles, [], Objects.FOOD):
        self.send(CommandNames.TAKE, Objects.FOOD.value)
    my_print("Moving to other player", ignore_verbose=True)
    self.go_to_direction(direction)
    tiles = self.send(CommandNames.LOOK)
    if tiles is None:  # pragma: no cover
        return
    if is_object_on_tile(tiles, [], Objects.FOOD):
        self.send(CommandNames.TAKE, Objects.FOOD.value)


def place_guard(self, msg: str, sender: str) -> None:
    if msg.count("not_") > 0 and sender in self.guards:
        self.guards.remove(sender)
    elif msg.count("not_") == 0 and sender not in self.guards:
        self.guards.append(sender)


def parse_incantation(self, inventory, sender: str, direction: Directions):
    if sender != self.mates_uuids[0]:
        my_print("Not the leader !!!")
        return
    if self.leader is None:
        self.leader = sender
        my_print("New leader: %s" % sender)
    if direction == Directions.HERE:
        self.drop_all_stones(inventory)
    else:
        self.walk_and_loot(direction)


def choose_action(self, inventory, msg: str, sender: str, direction: Directions):
    if msg.count("new") > 0:
        if sender not in self.mates_uuids:
            self.mates_uuids.append(sender)
            self.mates_uuids.sort()
            my_print("New mate: %s" % sender)
            self.send(CommandNames.BROADCAST, "new")
    elif msg.count("looted") > 0:
        self.add_to_shared_inventory(msg.split('~|')[2])
    elif msg.count("dropped") > 0:
        self.remove_from_shared_inventory(msg.split('~|')[2])
    elif msg.count("in_place") > 0:
        self.place_guard(msg, sender)
    elif self.is_guard():
        self.choose_guard_action(msg, sender, direction)
    elif msg.count("incantation") > 0 and len(self.mates_uuids) > 0:
        self.parse_incantation(inventory, sender, direction)
    else:
        my_print("Unknown message: %s" % msg)


def add_to_uuids(self, uuid):
    """Adds a message to the list of messages."""
    self.messages_uuids.append(uuid)
    if len(self.messages_uuids) > 1000:
        self.messages_uuids.pop(0)


def check_validity(self, msg: str) -> bool:
    uuid = msg.split('|~')
    if len(uuid) != 3 or not match(PossibleResponsesRegex.MY_MESSAGE.value[0], msg) \
            or not uuid[2].startswith(self.team + "~|"):
        my_print("Not my message, broadcasting...")
        file = open("/dev/urandom", "r", encoding="ISO-8859-2")
        self.send(CommandNames.BROADCAST, msg + file.read(3), True)
        file.close()
        return False
    return True


def parse_message(self, msg: str, inventory=None) -> None:
    """Parses a broadcast response."""
    try:
        splitted = msg.split(', ')
        direction = Directions(int(splitted[0].split(' ')[1]))
        msg = splitted[1].strip()
        if not self.check_validity(msg):
            return my_print("Invalid message, ignoring...")
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
    except ElevationException as e:
        raise e
    except Exception as e:  # pragma: no cover
        my_print("Error while parsing message: %s" % e)

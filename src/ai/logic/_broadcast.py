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


def choose_action(self, inventory, msg: str, sender: str, direction: Directions, old: bool = False):
    if not msg.endswith(str(self.level - old)):
        return
    if msg.count("leaving") > 0:
        if sender == self.leader:
            self.leader = None
            self.reader.pop_incantation()
            my_print("Leader left")
    elif msg.count("looted") > 0:
        self.add_to_shared_inventory(msg.split('~|')[2])
    elif msg.count("dropped") > 0:
        self.remove_from_shared_inventory(msg.split('~|')[2])
    elif msg.count("incantation") > 0:
        if self.leader is None or sender > self.leader:
            self.leader = sender
            my_print("New leader: %s" % sender)
        elif self.leader != sender:
            my_print("Not the leader !!!")
            return
        if direction == Directions.HERE:
            self.drop_all_stones(inventory)
        else:
            self.walk_and_loot(direction)
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


def parse_message(self, msg: str, inventory=None, old: bool = False) -> None:
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
        self.choose_action(inventory, msg, sender, direction, old)
    except Exception as e:  # pragma: no cover
        my_print("Error while parsing message: %s" % e)
    except ElevationException as e:
        raise e

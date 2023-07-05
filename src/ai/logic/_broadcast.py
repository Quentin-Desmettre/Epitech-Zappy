from regex import match
from src.ai.commands import Directions, CommandNames, Objects, PossibleResponsesRegex, ElevationException
from src.ai.logic._finding import is_object_on_tile
from src.ai.utils import my_print, set_trapped


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
        self.in_place = True
        self.send(CommandNames.BROADCAST, "here")
        self.drop_all_stones(inventory)
    elif not self.in_place:
        self.walk_and_loot(direction)


def choose_action(self, inventory, msg: str, sender: str, direction: Directions, time=None):
    if sender not in self.mates_uuids:
        self.mates_uuids.append(sender)
        self.mates_uuids.sort()
        my_print("New mate: %s" % sender)
        self.send(CommandNames.BROADCAST, "new")
    if msg.count("here") > 0:
        if self.leader != self.id:
            return
        if msg.count("not_here") > 0 and sender in self.here:
            self.here.remove(sender)
        elif sender not in self.here:
            self.here.append(sender)
    elif msg.count("looted") > 0:
        self.add_to_shared_inventory(msg.split('~|')[2])
    elif msg.count("dropped") > 0:
        self.remove_from_shared_inventory(msg.split('~|')[2])
    elif msg.count("in_place") > 0:
        self.place_guard(msg, sender)
    elif msg.count("heartbeat") > 0 and len(self.mates_uuids) > 0:
        if self.id != self.mates_uuids[0] and not self.is_guard():
            return
        self.guard_heartbeats[sender] = time
    elif self.is_guard(): # not analyzed by guards under this line !!!
        self.choose_guard_action(msg, sender, direction)
    elif msg.count("incantation") > 0 and len(self.mates_uuids) > 0:
        self.parse_incantation(inventory, sender, direction)
    elif msg.count("stop_guarding") > 0:
        self.in_place = False
    elif msg.count("new") == 0:
        my_print("Unknown message: %s" % msg)


def add_to_uuids(self, uuid):
    """Adds a message to the list of messages."""
    self.messages_uuids.append(uuid)
    if len(self.messages_uuids) > 1000:
        self.messages_uuids.pop(0)


def check_validity(self, msg: str, inventory) -> bool:
    if msg.count("|~") == 0:
        return False
    uuid = msg.split('|~')
    if len(uuid) != 3 or not match(PossibleResponsesRegex.MY_MESSAGE.value[0], msg) \
            or not uuid[2].startswith(self.team + "~|"):
        if not msg.endswith("Â") and self.can_survive(inventory):
            my_print("Not my team, broadcasting...")
            set_trapped(True)
            self.send(CommandNames.BROADCAST, msg, True)
        return False
    return True


def parse_message(self, msg: str, inventory, time=None) -> None:
    """Parses a broadcast response."""
    splitted = msg.split(', ')
    direction = Directions(int(splitted[0].split(' ')[1]))
    msg = splitted[1].strip()
    if not self.check_validity(msg, inventory):
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
    self.choose_action(inventory, msg, sender, direction, time)

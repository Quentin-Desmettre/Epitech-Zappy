from src.ai.utils import my_print, set_color, Colors
from src.ai.commands import CommandNames, Directions, Objects


def make_guard_decison(self, inventory: dict[str, int]):
    if not self.can_survive(inventory) or (self.reader.incantation_msg != "" and not self.in_place):
        if not self.can_survive(inventory):
            set_color(Colors.WARNING)
            self.take_food(inventory)
        else:
            set_color(Colors.OKCYAN)
            self.handle_broadcast(True, inventory)
        return
    if self.can_eject:
        self.send(CommandNames.EJECT)
        self.send(CommandNames.BROADCAST, "Ejecting !!!")


def choose_guard_action(self, msg: str, sender: str, direction: Directions):
    if msg.count("stop_guarding") > 0:
        self.can_eject = False
        self.in_place = False
        self.leader = None
        self.guards = []
        if self.reader.incantation_msg != "":
            self.reader.pop_incantation()
    elif msg.count("you_shall_not_pass") > 0:
        self.can_eject = True
    elif msg.count("incantation") > 0 and len(self.mates_uuids) > 0 and not self.in_place:
        if sender != self.mates_uuids[0]:
            my_print("Not the leader !!!")
            return
        if self.leader is None:
            self.leader = sender
            my_print("New leader: %s" % sender)
        if direction == Directions.HERE:
            self.choose_tile()
        else:
            self.walk_and_loot(direction)


def choose_tile(self):
    if not self.can_place():
        my_print("Can't place guard")
        return
    tiles = self.send(CommandNames.LOOK)
    if tiles is None:
        return
    if Objects.PLAYER.value in tiles[2]:
        self.send(CommandNames.RIGHT)
    else:
        self.send(CommandNames.FORWARD)
        self.send(CommandNames.BROADCAST, "in_place")
        self.in_place = True
        my_print("Guard in place", ignore_verbose=True)


def can_place(self) -> bool:
    check = False
    for uuid in self.mates_uuids:
        if check and uuid not in self.guards:
            return False
        if self.id == uuid:
            check = True
    return True


def is_guard(self):
    nb_uuids = len(self.mates_uuids)
    index = 0
    for uuid in self.mates_uuids:
        if self.id == uuid:
            return index >= nb_uuids - 4
        index += 1
    return False

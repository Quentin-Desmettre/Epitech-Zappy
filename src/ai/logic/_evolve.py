from src.ai.commands import Objects, CommandNames, get_elevation_needs
from src.ai.utils import my_print, merge_dicts


def has_stones(inventory: dict[Objects, int], current_level: int) -> bool:
    required = get_elevation_needs(current_level)
    for stone in required:
        if stone != Objects.PLAYER and (stone.value not in inventory
                                        or inventory[stone.value] < required[stone]):
            return False
    return True


def get_needed_stones(self, inventory: dict[Objects, int], merge_shared: bool = False) -> list[Objects]:
    required = get_elevation_needs(self.level)
    needed = []
    merged = merge_dicts(
        inventory, self.shared_inventory if merge_shared else {})
    for stone in required.keys():
        if stone != Objects.PLAYER and (stone.value not in merged
                                        or merged[stone.value] < required[stone]):
            needed.append(stone)
    return needed


def get_items_on_ground(self, tiles=None) -> dict[str, int]:
    if tiles is None:
        tiles = self.send(CommandNames.LOOK)
    if tiles is None:  # pragma: no cover
        return {}
    ground = tiles[0]
    items = {}
    for item in ground:
        if item in items:
            items[item] += 1
        else:
            items[item] = 1
    return items


def elevate(self, send_cmd: bool = True, msg: str = None):
    if send_cmd:
        my_print("Starting incantation to level %d" %
                 (self.level + 1), ignore_verbose=True)
        self.send(CommandNames.INCANTATION)
    elif msg.startswith("Current level:"):
        self.level = int(msg.split(" ")[2])
        my_print("Elevated to level %d !!!" % self.level, ignore_verbose=True)
        if self.level == 8:
            my_print("Congratulations, you won !!!", ignore_verbose=True)
            return
        if self.leader == self.id:
            self.send(CommandNames.BROADCAST, "stop_guarding")
        self.in_place = False
        self.leader = None
        self.guards = []
        self.here = []
        if self.reader.incantation_msg != "":
            self.reader.pop_incantation()
    else:
        my_print("Error: could not elevate", ignore_verbose=True)


def can_evolve(self, inventory: dict[str, int], tiles=None):
    """Returns whether the player can evolve or not."""
    tmp = merge_dicts(self.get_items_on_ground(tiles), inventory)
    return has_stones(merge_dicts(tmp, self.shared_inventory), self.level)


def drop_stone(self, stone, amount, send: bool = True) -> int:
    for _ in range(amount):
        if send:
            self.send(CommandNames.BROADCAST, "dropped~|" + stone)
        self.send(CommandNames.SET, stone)
    return amount


def drop_all_stones(self, inventory=None, to_not_send: Objects = None):
    """Drops all the stones from the inventory."""
    if inventory is None:
        inventory = self.send(CommandNames.INVENTORY)
    if inventory is None:  # pragma: no cover
        return
    nb_dropped = 0
    for stone in inventory:
        if stone == Objects.FOOD.value:
            continue
        if to_not_send != None and stone == to_not_send.value:
            nb_dropped += self.drop_stone(stone, 1, False)
            inventory[stone] -= 1
            to_not_send = None
        nb_dropped += self.drop_stone(stone, inventory[stone])
    if nb_dropped != 0:
        my_print("Dropped all stones needed to evolve.", ignore_verbose=True)


def check_requirements(self, inventory=None, tiles=None) -> bool:
    if inventory is None:
        inventory = self.send(CommandNames.INVENTORY)
    if inventory is None:  # pragma: no cover
        return False
    ground = self.get_items_on_ground(tiles)
    if not is_enough_player(self):
        return False
    total = merge_dicts(inventory, ground)
    needed = self.get_needed_stones(total)
    if len(needed) != 0:
        my_print("Not enough stones to evolve, missing : [ ", end="")
        for stone in needed:
            my_print(stone.value, end=" ")
            if stone.value in self.shared_inventory:
                my_print("(shared) ", end="")
        my_print("]")
        return False
    if len(self.guards) < len(self.guard_heartbeats):
        my_print("Not enough %i guards to evolve." %
                 len(self.guard_heartbeats))
        return False
    else:
        my_print("All %i guards are here." % len(self.guard_heartbeats))
    return True


def is_enough_player(self, print: bool = True):
    minimum = 6
    if len(self.here) < minimum - 1:
        if print:
            my_print("Not enough players to evolve.")
        return False
    return True

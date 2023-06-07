import time
from src.ai.commands import Objects, CommandNames, get_elevation_needs
from src.ai.utils import my_print, merge_dicts


def has_stones(inventory: dict[Objects, int], current_level: int) -> bool:
    required = get_elevation_needs(current_level)
    for stone in required:
        if stone != Objects.PLAYER and (stone.value not in inventory \
        or inventory[stone.value] < required[stone]):
            return False
    return True


def get_needed_stones(self, inventory: dict[Objects, int]) -> list[Objects]:
    required = get_elevation_needs(self.level)
    needed = []
    for stone in required.keys():
        if stone != Objects.PLAYER and (stone.value not in inventory \
        or inventory[stone.value] < required[stone]):
            needed.append(stone)
    return needed


def get_items_on_ground(self, tiles = None) -> dict[str, int]:
    if tiles is None:
        tiles = self.send(CommandNames.LOOK)
    if tiles is None:
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
        self.send(CommandNames.INCANTATION)
    elif msg.startswith("Current level:"):
        self.level = int(msg.split(" ")[2])
        my_print("Elevated to level %d !!!" % self.level)
        # if self.level == 3:
        #     exit(42)
    else:
        my_print("Error: could not elevate")


def can_evolve(self, inventory: dict[str, int], tiles = None):
    """Returns whether the player can evolve or not."""
    tmp = merge_dicts(self.get_items_on_ground(tiles), inventory)
    return has_stones(merge_dicts(tmp, self.shared_inventory), self.level)


def drop_elevation_stones(self, inventory = None):
    """Drops all the stones needed to evolve and returns whether the player can evolve or not."""
    if inventory is None:
        inventory = self.send(CommandNames.INVENTORY)
    if inventory is None:
        return
    for stone in inventory:
        while inventory[stone] > 0 and stone != Objects.FOOD.value:
            if self.send(CommandNames.SET, stone) != None:
                self.send(CommandNames.BROADCAST, "dropped:" + self.team + ":" + stone)
                inventory[stone] -= 1
                time.sleep(self.delta * 2)
    my_print("Dropped all stones needed to evolve.")


def check_requirements(self, inventory = None, tiles = None) -> bool:
    if inventory is None:
        inventory = self.send(CommandNames.INVENTORY)
    if inventory is None:
        return False
    ground = self.get_items_on_ground(tiles)
    total = merge_dicts(inventory, ground)
    if len(self.get_needed_stones(total)) != 0:
        my_print("Not enough stones to evolve.")
        return False
    if ground[Objects.PLAYER.value] < 6 and self.level > 1: # temporary fix to ensure that all players are on the same tile (original: get_elevation_needs(self.level)[Objects.PLAYER])
        my_print("Not enough players to evolve.")
        return False
    return True

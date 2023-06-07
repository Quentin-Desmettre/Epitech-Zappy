import math, socket, queue, time
from src.ai.utils import add_to_dict, my_print
from src.ai.commands import Objects, Directions, Command, CommandNames


def generate_heat_map(object: Objects, tiles: list[list[str]]) -> dict[int, list]:
    """Generates a heat map of the given object given its distance from the player."""
    heat_map = {}
    row_size = 3
    row_center = 2
    row_index = 1
    while row_center + row_size // 2 < len(tiles):
        if object.value in tiles[row_center]:
            add_to_dict(heat_map, row_index, row_center)
        for i in range(1, row_size // 2 + 1):
            if object.value in tiles[row_center - i]:
                add_to_dict(heat_map, row_index + i, row_center - i)
            if object.value in tiles[row_center + i]:
                add_to_dict(heat_map, row_index + i, row_center + i)
        row_center += row_size + 1
        row_size += 2
        row_index += 1
    return heat_map


def get_path_from_index(index: int) -> list[Directions]:
    """Returns the path from the player to the given index."""
    path = []
    row = math.floor(math.sqrt(index))
    row_center = row ** 2 + row
    column = index - row_center
    for _ in range(row):
        path.append(Directions.FORWARD)
    if column < 0:
        path.append(Directions.LEFT)
    elif column > 0:
        path.append(Directions.RIGHT)
    for _ in range(abs(column)):
        path.append(Directions.FORWARD)
    return path


def get_object_path(object: Objects, tiles: list[list[str]]) -> list[Directions]:
    """Returns the path from the player to the closest given object."""
    heat_map = generate_heat_map(object, tiles)
    if len(heat_map) == 0:
        return []
    closest_index = heat_map[min(heat_map.keys())][0]
    return get_path_from_index(closest_index)


def go_to_object(self, desired: Objects, tiles: list[list[str]] | None) -> bool:
    """Take the shortest path to the desired object and loot it if possible."""
    if tiles is None:
        tiles = self.send(CommandNames.LOOK)
    if tiles is None:
        return False
    directions = get_object_path(desired, tiles)
    for direction in directions:
        if self.send(direction) is None:
            break
    if len(directions) != 0 and desired != Objects.PLAYER:
        self.send(CommandNames.TAKE, desired.value)
    return len(directions) != 0


def loot_object(self, object: Objects, can_move_randomly: bool = True, tiles = None) -> bool:
    """Loots the object from the map if there is any, otherwise moves randomly."""
    if tiles is None:
        self.send(CommandNames.LOOK)
    if tiles is None:
        return False
    if object.value in tiles[0] and (tiles[0].count(Objects.PLAYER.value) <= 1 or object is Objects.FOOD):
        if self.send(CommandNames.TAKE, object.value) == "ko":
            my_print("Error: could not loot %s" % object.name)
            return False
    elif self.go_to_object(object, tiles) == False:
        if can_move_randomly:
            self.move_randomly()
        self.last_movement = time.time()
        return False
    return True
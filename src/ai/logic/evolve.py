import socket, queue
from src.ai.commands import Objects, Command, CommandNames


def get_elevation_needs(current_level: int) -> dict[Objects, int]:
    tab = [
        {
            Objects.PLAYER: 1,
            Objects.LINEMATE: 1,
        },
        {
            Objects.PLAYER: 2,
            Objects.LINEMATE: 1,
            Objects.DERAUMERE: 1,
            Objects.SIBUR: 1,
        },
        {
            Objects.PLAYER: 2,
            Objects.LINEMATE: 2,
            Objects.SIBUR: 1,
            Objects.PHIRAS: 2,
        },
        {
            Objects.PLAYER: 4,
            Objects.LINEMATE: 1,
            Objects.DERAUMERE: 1,
            Objects.SIBUR: 2,
            Objects.PHIRAS: 1,
        },
        {
            Objects.PLAYER: 4,
            Objects.LINEMATE: 1,
            Objects.DERAUMERE: 2,
            Objects.SIBUR: 1,
            Objects.MENDIANE: 3,
        },
        {
            Objects.PLAYER: 6,
            Objects.LINEMATE: 1,
            Objects.DERAUMERE: 2,
            Objects.SIBUR: 3,
            Objects.PHIRAS: 1,
        },
        {
            Objects.PLAYER: 6,
            Objects.LINEMATE: 2,
            Objects.DERAUMERE: 2,
            Objects.SIBUR: 2,
            Objects.MENDIANE: 2,
            Objects.PHIRAS: 2,
            Objects.THYSTAME: 1,
        }
    ]
    return tab[current_level - 1]


def has_stones(inventory: dict[Objects, int], current_level: int) -> bool:
    required = get_elevation_needs(current_level)
    for stone in required:
        if stone != Objects.PLAYER and (stone.value not in inventory \
        or inventory[stone.value] < required[stone]):
            return False
    return True


def get_needed_stones(inventory: dict[Objects, int], current_level: int) -> list[Objects]:
    required = get_elevation_needs(current_level)
    needed = []
    for stone in required.keys():
        if stone != Objects.PLAYER and (stone.value not in inventory \
        or inventory[stone.value] < required[stone]):
            needed.append(stone)
    return needed


def get_items_on_ground(server: socket.socket, queue: queue.Queue) -> dict[str, int]:
    tile = Command(CommandNames.LOOK).send(server, queue)
    if tile is None:
        return {}
    tile = tile[0]
    items = {}
    for item in tile:
        if item in items:
            items[item] += 1
        else:
            items[item] = 1
    return items


def enought_players(server: socket.socket, current_level: int) -> bool:
    required = get_elevation_needs(current_level)
    players = required[Objects.PLAYER]
    return players >= get_elevation_needs(current_level)[Objects.PLAYER]

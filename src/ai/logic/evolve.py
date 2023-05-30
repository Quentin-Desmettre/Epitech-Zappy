
from src.ai.commands import Objects

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
    return tab[current_level]

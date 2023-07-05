from random import randint
from time import time
from src.ai.commands import CommandNames, Directions


def move_randomly(self): # pragma: no cover
    """Moves and turns the player randomly."""
    rand = randint(0, 7)
    if rand <= 1:
        self.send(CommandNames.FORWARD)
    elif rand <= 4:
        self.send(CommandNames.RIGHT)
    else:
        self.send(CommandNames.LEFT)
    self.last_movement = time()


def go_to_direction(self, direction: Directions) -> None:
    """Goes to the given direction."""
    if direction == Directions.FORWARD:
        self.send(CommandNames.FORWARD)
    elif direction == Directions.RIGHT or direction == Directions.LEFT:
        self.send(direction)
        self.send(CommandNames.FORWARD)
    elif direction == Directions.BACKWARD:
        self.send(CommandNames.LEFT)
        self.send(CommandNames.LEFT)
        self.send(CommandNames.FORWARD)
    elif direction == Directions.TOP_RIGHT or direction == Directions.TOP_LEFT:
        self.send(CommandNames.FORWARD)
        if direction == Directions.TOP_RIGHT:
            self.go_to_direction(Directions.RIGHT)
        else:
            self.go_to_direction(Directions.LEFT)
    elif direction == Directions.BOTTOM_RIGHT:
        self.go_to_direction(Directions.RIGHT)
        self.go_to_direction(Directions.RIGHT)
    elif direction == Directions.BOTTOM_LEFT:
        self.go_to_direction(Directions.LEFT)
        self.go_to_direction(Directions.LEFT)
    self.last_movement = time()

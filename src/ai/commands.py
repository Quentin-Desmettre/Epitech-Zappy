from enum import Enum
from regex import Pattern, compile, match
from src.ai.utils import on, create_command_parsers

command_parsers = {}

class Objects(Enum):
    """Enum representing all the possible objects."""
    FOOD = "food"
    LINEMATE = "linemate"
    DERAUMERE = "deraumere"
    SIBUR = "sibur"
    MENDIANE = "mendiane"
    PHIRAS = "phiras"
    THYSTAME = "thystame"
    PLAYER = "player"


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
    if current_level > len(tab) or current_level <= 0:
        return {}
    return tab[current_level - 1]


class Directions(Enum):
    """Enum representing all the possible directions."""
    HERE = 0
    FORWARD = 1
    TOP_LEFT = 2
    LEFT = 3
    BOTTOM_LEFT = 4
    BACKWARD = 5
    BOTTOM_RIGHT = 6
    RIGHT = 7
    TOP_RIGHT = 8


class CommandNames(Enum):
    """Enum representing all the possible commands."""
    FORWARD = "Forward"
    RIGHT = "Right"
    LEFT = "Left"
    LOOK = "Look"
    INVENTORY = "Inventory"
    BROADCAST = "Broadcast"
    CONNECT_NBR = "Connect_nbr"
    FORK = "Fork"
    EJECT = "Eject"
    TAKE = "Take"
    SET = "Set"
    INCANTATION = "Incantation"


class PossibleResponsesRegex(Enum):
    """Enum representing all the possible responses from the server."""
    FORWARD = [r"^ok$", r"^ko$"]
    RIGHT = [r"^ok$", r"^ko$"]
    LEFT = [r"^ok$", r"^ko$"]
    LOOK = [r"\[\s*(?:\w+\s*)*\s*(?:,\s*(?:\w+\s*)*)*\]", r"^ko$"]
    INVENTORY = [r"\[\s*\w+\s*\d+\s*(?:,\s*(?:\w+)\s*\d+\s*)*\]", r"^ko$"]
    BROADCAST = [r"^ok$", r"^ko$"]
    CONNECT_NBR = [r"^[0-9]+$", r"^ko$"]
    FORK = [r"^ok$", r"^ko$"]
    EJECT = [r"^ok$", r"^ko$"]
    TAKE = [r"^ok$", r"^ko$"]
    SET = [r"^ok$", r"^ko$"]
    INCANTATION = [r"^Elevation underway$", r"^Current level: [0-9]+$", r"^ko$"]
    MESSAGE = [r"^message [0-9]+, .+$"]


def get_regexes(cmd: CommandNames) -> list[Pattern]:
    """Returns the regexes corresponding to the given command."""
    return [compile(regex_str) for regex_str in PossibleResponsesRegex[cmd.name].value]


class ElevationException(Exception):
    """Exception raised to catch elevation message."""

    def __init__(self, msg: str) -> None:
        self.msg = msg


class Command:
    """Class representing a command."""

    def __init__(self, cmd: CommandNames, arg: str | None = None) -> None:
        if type(cmd) == Directions:
            if cmd != Directions.FORWARD and cmd != Directions.RIGHT and cmd != Directions.LEFT:
                raise ValueError("Invalid direction %s" % cmd)
            if cmd == Directions.LEFT:
                self.type = CommandNames.LEFT
            elif cmd == Directions.RIGHT:
                self.type = CommandNames.RIGHT
            else:
                self.type = CommandNames.FORWARD
        else:
            self.type = cmd
        self.arg = arg

    def __str__(self) -> str:
        """Returns the command as a string."""
        if self.arg is None:
            return self.type.value
        else:
            return self.type.value + " " + self.arg

    @on(CommandNames.FORWARD)
    def parse_forward(self, result: str):
        for reg in PossibleResponsesRegex.FORWARD.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    @on(CommandNames.RIGHT)
    def parse_right(self, result: str):
        for reg in PossibleResponsesRegex.RIGHT.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    @on(CommandNames.LEFT)
    def parse_left(self, result: str):
        for reg in PossibleResponsesRegex.LEFT.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    @on(CommandNames.LOOK)
    def parse_look(self, result: str) -> list[list[str]]:
        matched = False
        for reg in PossibleResponsesRegex.LOOK.value:
            if match(reg, result):
                matched = True
        if not matched:
            raise ValueError("Invalid response %s for command %s" % (result, self.type))
        if result == "[]":
            return []
        result = result.strip()[1:-1].split(",")
        result = [tile.strip().split(" ") for tile in result]
        result = [[obj for obj in tile if obj.strip() != ""] for tile in result]
        return result

    @on(CommandNames.INVENTORY)
    def parse_inventory(self, result: str) -> dict[str, int]:
        matched = False
        for reg in PossibleResponsesRegex.INVENTORY.value:
            if match(reg, result):
                matched = True
        if not matched:
            raise ValueError("Invalid response %s for command %s" % (result, self.type))
        result = result.strip()[1:-1].split(",")
        result = [obj.strip().split(" ") for obj in result]
        result = {obj[0]: int(obj[1]) for obj in result if obj[0] != "" and obj[1] != ""}
        return result

    @on(CommandNames.BROADCAST)
    def parse_broadcast(self, result: str):
        for reg in PossibleResponsesRegex.BROADCAST.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    @on(CommandNames.CONNECT_NBR)
    def parse_connect_nbr(self, result: str):
        for reg in PossibleResponsesRegex.CONNECT_NBR.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    @on(CommandNames.FORK)
    def parse_fork(self, result: str):
        for reg in PossibleResponsesRegex.FORK.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    @on(CommandNames.EJECT)
    def parse_eject(self, result: str):
        for reg in PossibleResponsesRegex.EJECT.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    @on(CommandNames.TAKE)
    def parse_take(self, result: str):
        for reg in PossibleResponsesRegex.TAKE.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    @on(CommandNames.SET)
    def parse_set(self, result: str):
        for reg in PossibleResponsesRegex.SET.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    @on(CommandNames.INCANTATION)
    def parse_incantation(self, result: str):
        for reg in PossibleResponsesRegex.INCANTATION.value:
            if match(reg, result):
                return result
        raise ValueError("Invalid response %s for command %s" % (result, self.type))

    def parse_response(self, response: str):
        if self.type not in command_parsers:
            raise NotImplementedError("Command %s not implemented" % self.type)
        return command_parsers[self.type](self, response)


command_parsers = create_command_parsers(Command)

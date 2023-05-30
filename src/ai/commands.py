import socket, enum
from src.ai.utils import send_to_server, recv_from_server, on, create_command_parsers

command_parsers = {}

class Objects(enum.Enum):
    """Enum representing all the possible objects."""
    FOOD = "food"
    LINEMATE = "linemate"
    DERAUMERE = "deraumere"
    SIBUR = "sibur"
    MENDIANE = "mendiane"
    PHIRAS = "phiras"
    THYSTAME = "thystame"
    PLAYER = "player"


class Directions(enum.Enum):
    """Enum representing all the possible directions."""
    FORWARD = 1
    LEFT = 3
    RIGHT = 7
    BACKWARD = 5
    TOP_LEFT = 2
    TOP_RIGHT = 8
    BOTTOM_LEFT = 4
    BOTTOM_RIGHT = 6


class CommandNames(enum.Enum):
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


class Command:
    """Class representing a command."""
    type = None
    arg = None

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
        return result

    @on(CommandNames.RIGHT)
    def parse_right(self, result: str):
        return result

    @on(CommandNames.LEFT)
    def parse_left(self, result: str):
        return result

    @on(CommandNames.LOOK)
    def parse_look(self, result: str) -> list[list[str]]:
        if result == "[]":
            return []
        result = result.strip()[1:-1].split(",")
        result = [tile.strip().split(" ") for tile in result]
        result = [[obj for obj in tile if obj.strip() != ""] for tile in result]
        return result

    @on(CommandNames.INVENTORY)
    def parse_inventory(self, result: str) -> dict[str, int]:
        result = result.strip()[1:-1].split(",")
        result = [obj.strip().split(" ") for obj in result]
        result = {obj[0]: int(obj[1]) for obj in result if obj[0] != "" and obj[1] != ""}
        return result

    @on(CommandNames.BROADCAST)
    def parse_broadcast(self, result: str):
        return result

    @on(CommandNames.CONNECT_NBR)
    def parse_connect_nbr(self, result: str):
        return int(result)

    @on(CommandNames.FORK)
    def parse_fork(self, result: str):
        return result

    @on(CommandNames.EJECT)
    def parse_eject(self, result: str):
        return result

    @on(CommandNames.TAKE)
    def parse_take(self, result: str):
        return result

    @on(CommandNames.SET)
    def parse_set(self, result: str):
        return result

    @on(CommandNames.INCANTATION)
    def parse_incantation(self, result: str):
        return result

    def check_return(self, result: str) -> bool:
        """Checks if the command was successful."""
        if result == "dead":
            print("You died")
            exit(0)
        if result == "ko":
            print("Command %s failed" % self.type.value)
            return False
        return True

    def send(self, server: socket.socket):
        """Sends the command to the server and returns the parsed result."""
        if self.type not in command_parsers:
            raise NotImplementedError("Command %s not implemented" % self.type)
        send_to_server(server, str(self))
        msg = recv_from_server(server)
        if self.type == CommandNames.INCANTATION:
            msg = recv_from_server(server)
        if not self.check_return(msg):
            return None
        return command_parsers[self.type](self, msg)

command_parsers = create_command_parsers(Command)

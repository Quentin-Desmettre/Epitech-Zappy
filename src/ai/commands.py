import socket, enum, queue
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
    HERE = 0
    FORWARD = 1
    TOP_LEFT = 2
    LEFT = 3
    BOTTOM_LEFT = 4
    BACKWARD = 5
    BOTTOM_RIGHT = 6
    RIGHT = 7
    TOP_RIGHT = 8


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

    def read_until_broadcast(self, server: socket.socket, queue: queue.Queue) -> str:
        """Reads the server until no more broadcast are received."""
        msg = recv_from_server(server)
        while msg.startswith("message"):
            if queue is not None:
                while queue.qsize() > 0:
                    queue.get()
                queue.put(msg)
            msg = recv_from_server(server)
        return msg

    def send(self, server: socket.socket, queue: queue.Queue = None):
        """Sends the command to the server and returns the parsed result."""
        if self.type not in command_parsers:
            raise NotImplementedError("Command %s not implemented" % self.type)
        send_to_server(server, str(self))
        msg = self.read_until_broadcast(server, queue)
        if self.type == CommandNames.INCANTATION:
            msg = self.read_until_broadcast(server, queue)
        if not self.check_return(msg):
            return None
        return command_parsers[self.type](self, msg)


def go_to_direction(server: socket.socket, direction: Directions, queue: queue.Queue = None) -> None:
    """Goes to the given direction."""
    if direction == Directions.FORWARD:
        Command(CommandNames.FORWARD).send(server, queue)
    elif direction == Directions.RIGHT or direction == Directions.LEFT:
        Command(direction).send(server, queue)
        Command(CommandNames.FORWARD).send(server, queue)
    elif direction == Directions.BACKWARD:
        Command(CommandNames.LEFT).send(server, queue)
        Command(CommandNames.LEFT).send(server, queue)
        Command(CommandNames.FORWARD).send(server, queue)
    elif direction == Directions.TOP_RIGHT or direction == Directions.TOP_LEFT:
        Command(CommandNames.FORWARD).send(server, queue)
        if direction == Directions.TOP_RIGHT:
            go_to_direction(server, Directions.RIGHT, queue)
        else:
            go_to_direction(server, Directions.LEFT, queue)
    elif direction == Directions.BOTTOM_RIGHT or direction == Directions.BOTTOM_LEFT:
        go_to_direction(server, Directions.BACKWARD, queue)
        if direction == Directions.BOTTOM_RIGHT:
            go_to_direction(server, Directions.LEFT, queue)
        else:
            go_to_direction(server, Directions.RIGHT, queue)


command_parsers = create_command_parsers(Command)

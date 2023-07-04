from regex import match
from threading import Lock, Thread
from random import randint
from time import time
from queue import Queue
from socket import socket
from src.ai.commands import Command, CommandNames, PossibleResponsesRegex, ElevationException, get_regexes
from src.ai.utils import send_to_server, my_print


class Reader:
    """In a dedicated thread, reads from the server all the time and stores the commands in a queue."""

    def __init__(self, sock: socket, team: str) -> None:
        self.sock = sock
        self.cmd = Command(CommandNames.FORWARD)
        self.queue = Queue()
        self.broadcast_queue = Queue()
        self.heartbeat_queue = Queue()
        self.incantation_msg = ""
        self.incantation_time = 0
        self.buffer = ""
        self.team = team
        self.incantation_mtx = Lock()
        self.thread = Thread(target=self.launch, daemon=True)
        self.thread.start()

    def launch(self) -> None:
        """Starts the reading thread."""
        try:
            while True:
                data = self.sock.recv(4096)
                try:
                    data = data.decode("utf-8")
                except UnicodeDecodeError:  # pragma: no cover
                    data = data.decode("ISO-8859-2")
                if not data or data == "":  # pragma: no cover
                    raise Exception("Server disconnected")
                self.buffer += data
                while "\n" in self.buffer:
                    self.parse_data()
        except Exception as e:
            self.queue.put("dead")
            self.queue.put(e)

    def parse_data(self) -> None:
        index = self.buffer.index("\n")
        msg = self.buffer[:index]
        if msg == "dead":
            my_print("Received: %s" % msg)
            raise Exception("You died")
        if match(PossibleResponsesRegex.MESSAGE.value[0], msg):
            if msg.count(self.team) == 0 and randint(0, 3) != 0:
                return
            if msg.count("incantation") > 0:
                self.set_incanation_msg(msg)
                self.set_incanation_time(time())
            elif msg.count("heartbeat") > 0:
                self.heartbeat_queue.put([msg, time()])
            else:
                self.broadcast_queue.put(msg)
        else:
            my_print("Received: %s" % msg)
            self.queue.put(msg)
        self.buffer = self.buffer[index + 1:]

    def check_matches(self, msg: str, type: CommandNames) -> bool:
        """Checks if the message matches the regex."""
        if match(PossibleResponsesRegex.INCANTATION.value[1], msg):
            raise ElevationException(msg)
        if match(PossibleResponsesRegex.INCANTATION.value[0], msg):
            return True
        regexes = get_regexes(type)
        for reg in regexes:
            if match(reg, msg):
                return True
        return False

    def get_next_match(self, type: CommandNames) -> str:
        """Gets the next message that matches the regex."""
        msg = self.queue_pop()
        while not self.check_matches(msg, type):
            msg = self.queue_pop()
        return msg

    def send(self, type: CommandNames, arg: str | None = None):
        """Sends a command to the server."""
        self.cmd.__init__(type, arg)
        send_to_server(self.sock, str(self.cmd))
        msg = self.get_next_match(type)
        if type == CommandNames.INCANTATION and msg == "ko":
            msg = self.get_next_match(type)
            if msg == "ko":
                my_print("Error: could not elevate", ignore_verbose=True)
        while match(PossibleResponsesRegex.INCANTATION.value[0], msg):
            msg = self.wait_end_incantation()
            if msg == "ko":
                my_print("Error: could not elevate", ignore_verbose=True)
        if msg == "ko":
            my_print("Command %s failed" % type.value)
            return None
        return self.cmd.parse_response(msg)

    def wait_end_incantation(self) -> str:
        """Waits for the end of the incantation."""
        msg = self.queue_pop()
        while not self.check_matches(msg, CommandNames.INCANTATION):
            msg = self.queue_pop()
        return msg

    def queue_pop(self) -> str:
        """Pops a message from the queue."""
        msg = self.queue.get()
        if msg == "dead":
            raise Exception(self.queue.get())
        return msg

    def has_broadcast(self) -> bool:
        """Returns True if there is a broadcast in the queue."""
        return not self.broadcast_queue.empty()

    def has_heartbeat(self) -> bool:
        """Returns True if there is a heartbeat in the queue."""
        return not self.heartbeat_queue.empty()

    def broadcast_pop(self) -> str:
        """Pops a broadcast from the queue."""
        return self.broadcast_queue.get()

    def heartbeat_pop(self) -> tuple[str, float]:
        """Pops a heartbeat from the queue."""
        heartbeat = self.heartbeat_queue.get()
        return heartbeat[0], heartbeat[1]

    def set_incanation_msg(self, msg: str) -> None:
        """Sets the incantation message."""
        self.incantation_mtx.acquire()
        self.incantation_msg = msg
        self.incantation_mtx.release()

    def set_incanation_time(self, time: float) -> None:
        """Sets the incantation time."""
        self.incantation_mtx.acquire()
        self.incantation_time = time
        self.incantation_mtx.release()

    def pop_incantation(self) -> tuple[str, float]:
        """Pops the incantation message."""
        self.incantation_mtx.acquire()
        msg = self.incantation_msg
        time = self.incantation_time
        self.incantation_msg = ""
        self.incantation_time = 0
        self.incantation_mtx.release()
        return msg, time

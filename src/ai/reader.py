from regex import match
from threading import Lock, Thread
from signal import SIGINT
from random import randint
from os import kill, getpid
from time import time
from queue import Queue
from socket import socket
from src.ai.commands import Command, CommandNames, PossibleResponsesRegex, ElevationException, get_regexes
from src.ai.utils import send_to_server, my_print


class Reader:
    """In a dedicated thread, reads from the server all the time and stores the commands in a queue."""

    def __init__(self, sock: socket, team: str) -> None:
        self.sock = sock
        self.queue = Queue()
        self.broadcast_queue = Queue()
        self.buffer = ""
        self.team = team
        self.mtx = Lock()
        self.thread = Thread(target=self.launch, daemon=True)
        self.thread.start()

    def launch(self) -> None:
        """Starts the reading thread."""
        try:
            while True:
                data = self.sock.recv(4096).decode()
                if not data or data == "":
                    raise Exception("Server disconnected")
                self.buffer += data
                while "\n" in self.buffer:
                    self.mtx.acquire()
                    self.parse_data()
                    self.mtx.release()
        except Exception as e:
            self.queue.put("dead")

    def parse_data(self) -> None:
        index = self.buffer.index("\n")
        msg = self.buffer[:index]
        if msg == "dead":
            raise Exception("You died")
        if match(PossibleResponsesRegex.MESSAGE.value[0], msg):
            if msg.count(self.team) == 0 and randint(0, 3) != 0:
                return
            if msg.count("incantation") > 0:
                self.clean_broadcast_queue()
            self.broadcast_queue.put([msg, time()])
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
        cmd = Command(type, arg)
        send_to_server(self.sock, str(cmd))
        msg = self.get_next_match(type)
        if type == CommandNames.INCANTATION and msg == "ko":
            msg = self.get_next_match(type)
        while match(PossibleResponsesRegex.INCANTATION.value[0], msg):
            msg = self.get_next_match(type)
        if msg == "ko":
            my_print("Command %s failed" % type.value)
            return None
        return cmd.parse_response(msg)

    def queue_pop(self) -> str:
        """Pops a message from the queue."""
        msg = self.queue.get()
        if msg == "dead":
            raise Exception("You died")
        return msg

    def has_broadcast(self) -> bool:
        """Returns True if there is a broadcast in the queue."""
        return not self.broadcast_queue.empty()

    def broadcast_pop(self) -> str:
        """Pops a broadcast from the queue."""
        return self.broadcast_queue.get()

    def broadcast_contains(self, msg: str) -> bool:
        """Checks if the broadcast queue contains a message."""
        tmp_queue = Queue()
        found = False
        self.mtx.acquire()
        while not self.broadcast_queue.empty():
            tmp = self.broadcast_queue.get()
            if tmp[0].count(msg) > 0:
                found = True
            tmp_queue.put(tmp)
        while not tmp_queue.empty():
            self.broadcast_queue.put(tmp_queue.get())
        self.mtx.release()
        return found

    def clean_broadcast_queue(self):
        """Cleans broadcast queue from all messages stating with 'incantation'."""
        tmp_queue = Queue()
        while not self.broadcast_queue.empty():
            msg = self.broadcast_queue.get()
            if not msg[0].count("incantation") == 0:
                tmp_queue.put(msg)
        while not tmp_queue.empty():
            self.broadcast_queue.put(tmp_queue.get())

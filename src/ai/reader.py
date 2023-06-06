import socket, queue, time, regex, threading
from src.ai.commands import Command, CommandNames, PossibleResponsesRegex, ElevationException, get_regexes
from src.ai.utils import send_to_server, my_print


class Reader:
    """In a dedicated thread, reads from the server all the time and stores the commands in a queue."""

    def __init__(self, sock: socket.socket, team: str) -> None:
        self.sock = sock
        self.queue = queue.Queue()
        self.broadcast_queue = queue.Queue()
        self.buffer = ""
        self.team = team
        self.thread = threading.Thread(target=self.launch, daemon=True)
        self.thread.start()
        self.mtx = threading.Lock()

    def launch(self) -> None:
        """Starts the reading thread."""
        while True:
            data = self.sock.recv(4096).decode()
            if not data or data == "":
                break
            self.buffer += data
            while "\n" in self.buffer:
                self.mtx.acquire()
                self.parse_data()
                self.mtx.release()

    def parse_data(self) -> None:
        index = self.buffer.index("\n")
        msg = self.buffer[:index]
        my_print("Received: %s" % msg)
        if regex.match(PossibleResponsesRegex.MESSAGE.value[0], msg):
            if msg.count(self.team) == 0:
                return # à remplacer pour les renvoyer aux autres
            if msg.count("incantation") > 0:
                self.clean_broadcast_queue()
            self.broadcast_queue.put({msg, time.time()})
        else:
            self.queue.put(msg)
        self.buffer = self.buffer[index + 1:]

    def check_matches(self, msg: str, type: CommandNames) -> bool:
        """Checks if the message matches the regex."""
        regexes = get_regexes(type)
        for reg in regexes:
            if regex.match(PossibleResponsesRegex.INCANTATION.value[1], msg):
                raise ElevationException(type, msg)
            if regex.match(reg, msg):
                return True
        return False

    def get_next_match(self, type: CommandNames) -> str:
        """Gets the next message that matches the regex."""
        msg = self.queue.get()
        if msg == "dead":
            my_print("You died")
            exit(0)
        while not self.check_matches(msg, type):
            msg = self.queue.get()
            if msg == "dead":
                my_print("You died")
                exit(0)
        return msg

    def send(self, type: CommandNames, arg: str | None = None):
        """Sends a command to the server."""
        cmd = Command(type, arg)
        send_to_server(self.sock, str(cmd))
        msg = self.get_next_match(type)
        if regex.match(PossibleResponsesRegex.INCANTATION.value[0], msg):
            msg = self.get_next_match(type)
        if msg == "ko":
            my_print("Command %s failed" % type.value)
            return None
        return cmd.parse_response(msg)

    def has_broadcast(self) -> bool:
        """Returns True if there is a broadcast in the queue."""
        return not self.broadcast_queue.empty()

    def broadcast_pop(self) -> str:
        """Pops a broadcast from the queue."""
        return self.broadcast_queue.get()

    def broadcast_contains(self, msg: str) -> bool:
        """Checks if the broadcast queue contains a message."""
        tmp_queue = queue.Queue()
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
        tmp_queue = queue.Queue()
        while not self.broadcast_queue.empty():
            msg = self.broadcast_queue.get()
            if not msg[0].startswith("message incantation"):
                tmp_queue.put(msg)
        while not tmp_queue.empty():
            self.broadcast_queue.put(tmp_queue.get())

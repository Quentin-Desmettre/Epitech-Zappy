from socket import socket, AF_INET, SOCK_STREAM
from  src.ai.commands import CommandNames
from threading import Thread


class MiniServer: # pragma: no cover
    connections = []
    socket = None

    def __init__(self):
        self.responses = {
            CommandNames.FORWARD.value: "ok",
            CommandNames.RIGHT.value: "ok",
            CommandNames.LEFT.value: "ok",
            CommandNames.LOOK.value: "[ player linemate, food, ,  ]",
            CommandNames.INVENTORY.value: "[ food 9, linemate 0, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0 ]",
            CommandNames.BROADCAST.value: "ok",
            CommandNames.CONNECT_NBR.value: "1",
            CommandNames.FORK.value: "ok",
            CommandNames.EJECT.value: "ok",
            CommandNames.TAKE.value: "ok",
            CommandNames.SET.value: "ok",
            CommandNames.INCANTATION.value: "Elevation underway\nCurrent level: 2",
        }
        self.default = self.responses.copy()
        self.buffer = ""
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.bind(("localhost", 8080))
        self.socket.listen(1)
        self.thread = Thread(target=self.run, daemon=True)
        self.thread.start()

    def run(self):
        while True:
            connection, _ = self.socket.accept()
            self.connections.append(connection)
            while True:
                data = connection.recv(4096).decode()
                if not data or data.strip() == "":
                    break
                self.buffer += data
                while "\n" in self.buffer:
                    index = self.buffer.index("\n")
                    command = self.buffer[:index].split(" ")[0]
                    response = self.responses.get(command, "ko")
                    connection.send((response + "\n").encode())
                    self.buffer = self.buffer[index + 1:]

    def set_response(self, command: str, response: str):
        self.responses[command] = response

    def set_default(self, command: str):
        self.responses[command] = self.default[command]

    def reset(self):
        self.responses = self.default.copy()


    def stop(self):
        for connection in self.connections:
            connection.close()
        self.socket.close()

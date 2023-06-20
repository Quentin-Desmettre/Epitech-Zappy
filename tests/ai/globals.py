from unittest import TestCase
from tests.ai.mini_server import MiniServer
from socket import socket, AF_INET, SOCK_STREAM
from src.ai.logic import Ai


server = MiniServer()
connection = socket(AF_INET, SOCK_STREAM)
connection.connect(("localhost", 8080))
ai = Ai(connection, "team1")

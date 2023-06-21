import sys
from os import remove
from unittest import TestCase
from tests.ai.globals import ai, server
from src.ai.commands import Directions, Command, CommandNames
from src.ai.utils import set_verbose


class CommandTester(TestCase):

    def setUp(self):
        global ai
        self.ai = ai
        sys.stdout = open("finding_output", "w+", buffering=1)
        self.output = open("finding_output", "r+")
        set_verbose(True)

    def tearDown(self):
        global server
        server.reset()
        sys.stdout.close()
        sys.stdout = sys.__stdout__
        self.output.close()
        remove("finding_output")

    def test_invalid_commands(self):
        try:
            Command(Directions.TOP_LEFT)
            self.fail("Invalid command did not raise exception") # pragma: no cover
        except ValueError as e:
            self.assertEqual(str(e), "Invalid direction %s" % Directions.TOP_LEFT)
        try:
            Command("azertyuiop")
            self.fail("Invalid command did not raise exception") # pragma: no cover
        except NotImplementedError as e:
            self.assertEqual(str(e), "Command azertyuiop not implemented")

    def test_valid_commands(self):
        try:
            Command(Directions.FORWARD)
            Command(Directions.LEFT)
            Command(Directions.RIGHT)
            Command(Directions.RIGHT)
        except ValueError: # pragma: no cover
            self.fail("Command raised exception")
        for cmd in CommandNames:
            try:
                Command(cmd)
            except ValueError: # pragma: no cover
                self.fail("Command raised exception")

    def test_invalid_parsers(self):
        try:
            cmd = Command(CommandNames.FORWARD)
            cmd.type = "azertyuiop"
            cmd.parse_response("ok")
        except NotImplementedError as e:
            self.assertEqual(str(e), "Command azertyuiop not implemented")
        for cmd in CommandNames:
            Command(cmd).parse_response("azertyuiop")
            output = self.output.read()
            self.assertIn("Invalid response azertyuiop for command %s\n" % cmd, output)

    def test_ko_parser(self):
        for cmd in CommandNames:
            response = Command(cmd).parse_response("ko")
            self.assertEqual(response, None)

    def test_ok_parser(self):
        for cmd in CommandNames:
            if cmd == CommandNames.LOOK or cmd == CommandNames.INVENTORY \
            or cmd == CommandNames.CONNECT_NBR or cmd == CommandNames.INCANTATION:
                continue
            response = Command(cmd).parse_response("ok")
            self.assertEqual(response, "ok")

    def test_look_parser(self):
        response = Command(CommandNames.LOOK).parse_response("[]")
        self.assertEqual(response, [])

        response = Command(CommandNames.LOOK).parse_response("[ player food player food, linemate, , food ]")
        self.assertEqual(response, [["player", "food", "player", "food"], ["linemate"], [], ["food"]])

        response = Command(CommandNames.LOOK).parse_response("[ player , food ]")
        self.assertEqual(response, [["player"], ["food"]])

    def test_inventory_parser(self):
        response = Command(CommandNames.INVENTORY).parse_response("[ food 9, linemate 0, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0 ]")
        self.assertEqual(response, {"food": 9, "linemate": 0, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0})

        response = Command(CommandNames.INVENTORY).parse_response("[ food 4, linemate 1, deraumere 2, sibur 1, mendiane 7, phiras 4, thystame 3 ]")
        self.assertEqual(response, {"food":4, "linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 7, "phiras": 4, "thystame": 3})
import sys
from os import remove
from unittest import TestCase
from tests.ai.globals import ai, server
from src.ai.commands import Directions
from src.ai.utils import set_verbose


class MovementTester(TestCase):

    def setUp(self):
        global ai
        self.ai = ai
        sys.stdout = open("movement_output", "w+", buffering=1)
        self.output = open("movement_output", "r+")
        set_verbose(True)

    def tearDown(self):
        global server
        server.reset()
        sys.stdout.close()
        sys.stdout = sys.__stdout__
        self.output.close()
        remove("movement_output")

    def test_move_randomly(self):
        self.ai.move_randomly()
        output = self.output.read()
        self.assertTrue(output.count("Sending: Forward") == 1
                        or output.count("Sending: Right") == 1
                        or output.count("Sending: Left") == 1)

    def test_go_to_direction(self):
        self.ai.go_to_direction(Directions.FORWARD)
        output = self.output.read()
        self.assertTrue(output.count("Sending: Forward") == 1)

        self.ai.go_to_direction(Directions.RIGHT)
        output = self.output.read()
        self.assertTrue(output.count("Sending: Right") == 1)
        self.assertTrue(output.count("Sending: Forward") == 1)

        self.ai.go_to_direction(Directions.LEFT)
        output = self.output.read()
        self.assertTrue(output.count("Sending: Left") == 1)
        self.assertTrue(output.count("Sending: Forward") == 1)

        self.ai.go_to_direction(Directions.BACKWARD)
        output = self.output.read()
        self.assertTrue(output.count("Sending: Left") == 2)
        self.assertTrue(output.count("Sending: Forward") == 1)

        self.ai.go_to_direction(Directions.TOP_RIGHT)
        output = self.output.read()
        self.assertTrue(output.count("Sending: Forward") == 2 and output.count("Sending: Right") == 1)

        self.ai.go_to_direction(Directions.TOP_LEFT)
        output = self.output.read()
        self.assertTrue(output.count("Sending: Forward") == 2 and output.count("Sending: Left") == 1)

        self.ai.go_to_direction(Directions.BOTTOM_RIGHT)
        output = self.output.read()
        self.assertTrue(output.count("Sending: Forward") == 2)
        self.assertTrue(output.count("Sending: Right") == 2)

        self.ai.go_to_direction(Directions.BOTTOM_LEFT)
        output = self.output.read()
        self.assertTrue(output.count("Sending: Forward") == 2)
        self.assertTrue(output.count("Sending: Left") == 2)
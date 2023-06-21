import sys
from os import remove
from unittest import TestCase
from tests.ai.globals import ai, server
from src.ai.commands import Objects, ElevationException
from src.ai.logic._evolve import has_stones
from src.ai.utils import set_verbose
from time import time


class InitTester(TestCase):

    def setUp(self):
        global ai
        self.ai = ai
        sys.stdout = open("init_output", "w+", buffering=1)
        self.output = open("init_output", "r+")
        set_verbose(True)

    def tearDown(self):
        global server
        server.reset()
        sys.stdout.close()
        sys.stdout = sys.__stdout__
        self.output.close()
        remove("init_output")

    def test_take_food(self):
        inventory = {"food": 10}
        self.ai.take_food(inventory)
        output = self.output.read()
        self.assertIn("Looting food", output)
        self.assertTrue(output.count("Take food") == 10)

        self.ai.take_food({})
        output = self.output.read()
        self.assertIn("Looting food", output)
        self.assertTrue(output.count("Take food") == 20)

    def test_take_stones(self):
        global server
        self.ai.take_stones({})
        output = self.output.read()
        self.assertIn("Looting stones", output)
        self.assertTrue(output.count("Take linemate") == 1)
        self.assertTrue(output.count("Set linemate") == 1)
        self.assertTrue(output.count("Dropped all stones needed to evolve.") == 1)

        self.ai.level = 2
        self.ai.take_stones({})
        output = self.output.read()
        self.assertIn("Looting stones", output)
        self.assertTrue(output.count("Take linemate") == 1)
        self.assertNotIn("Dropped all stones needed to evolve.", output)

        server.set_response("Look", "[ player, food, ,  ]")
        self.ai.take_stones({})
        output = self.output.read()
        self.assertIn("Looting stones", output)
        self.assertNotIn("Take linemate", output)
        self.assertNotIn("Set linemate", output)
        self.assertNotIn("Dropped all stones needed to evolve.", output)

    def test_handle_broadcast(self):
        self.ai.handle_broadcast(["incantation", time() + 1], {})
        output = self.output.read()
        self.assertIn("Ignoring broadcast incantation", output)

        self.ai.messages_uuids = []
        self.ai.handle_broadcast(["message 4, uuid1|~uuid2|~looted~|team1~|linemate~|1", time()], {})
        output = self.output.read()
        self.assertEqual(self.ai.shared_inventory["linemate"], 1)

    def test_handle_evolve(self):
        global server
        self.ai.handle_evolve({})
        output = self.output.read()
        self.assertIn("incantation~|", output)

        self.ai.level = 1
        self.ai.fast_mode = False
        try:
            self.ai.handle_evolve({"linemte": 1})
            self.fail("Should have raised an ElevationException") # pragma: no cover
        except ElevationException:
            pass

        server.set_response("Look", "[ player player player player player player, food, ,  ]")
        self.ai.handle_evolve({"linemte": 1})
        output = self.output.read()
        self.assertIn("Trying to evolve to level 2", output)
        self.assertIn("Not enough stones to evolve", output)
        self.assertIn("incantation~|", output)

    def test_can_survive(self):
        self.assertFalse(self.ai.can_survive({"food": 7}))

        self.ai.leader = self.ai.id
        self.assertTrue(self.ai.can_survive({"food": 7}))
        self.ai.leader = None

        self.assertTrue(self.ai.can_survive({"food": 12}))


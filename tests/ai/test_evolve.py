import sys
from os import remove
from unittest import TestCase
from tests.ai.globals import ai, server
from src.ai.commands import Objects, ElevationException
from src.ai.logic._evolve import has_stones
from src.ai.utils import set_verbose


class EvolveTester(TestCase):

    def setUp(self):
        global ai
        self.ai = ai
        sys.stdout = open("evolve_output", "w+", buffering=1)
        self.output = open("evolve_output", "r+")
        set_verbose(True)

    def tearDown(self):
        global server
        server.reset()
        sys.stdout.close()
        sys.stdout = sys.__stdout__
        self.output.close()
        remove("evolve_output")

    def test_has_stones(self):
        inventory = {Objects.FOOD.value: 8, Objects.DERAUMERE.value: 1, Objects.SIBUR.value: 1}
        self.assertFalse(has_stones(inventory, 2))

        inventory[Objects.LINEMATE.value] = 3
        self.assertTrue(has_stones(inventory, 2))

    def test_get_needed_stones(self):
        inventory = {Objects.FOOD.value: 8, Objects.DERAUMERE.value: 1, Objects.SIBUR.value: 1}
        self.assertEqual(self.ai.get_needed_stones(inventory), [Objects.LINEMATE])

        inventory[Objects.LINEMATE.value] = 3
        self.assertEqual(self.ai.get_needed_stones(inventory), [])

    def test_get_items_on_ground(self):
        tiles = [[Objects.FOOD.value, Objects.FOOD.value, Objects.LINEMATE.value], [Objects.FOOD.value]]
        self.assertEqual(self.ai.get_items_on_ground(tiles), {Objects.FOOD.value: 2, Objects.LINEMATE.value: 1})

        self.assertEqual(self.ai.get_items_on_ground(), {Objects.LINEMATE.value: 1, Objects.PLAYER.value: 1})

    def test_elevate(self):
        try:
            self.ai.elevate()
        except ElevationException as e:
            self.assertEqual(str(e), "Current level: 2")
        output = self.output.read()
        self.assertIn("Starting incantation to level 2", output)

        self.ai.elevate(False, "Current level: 2")
        output = self.output.read()
        self.assertIn("Elevated to level 2 !!!", output)
        self.assertEqual(self.ai.level, 2)
        self.assertEqual(self.ai.shared_inventory, {})
        self.assertIsNone(self.ai.leader)

        self.ai.elevate(False, "Current level: 8")
        output = self.output.read()
        self.assertIn("Congratulations, you won !!!", output)
        self.ai.level = 1

        self.ai.elevate(False, "Not a valid message")
        output = self.output.read()
        self.assertIn("Error: could not elevate", output)

    def test_can_evolve(self):
        global server
        result = self.ai.can_evolve({Objects.FOOD.value: 8, Objects.DERAUMERE.value: 1, Objects.SIBUR.value: 1})
        self.assertTrue(result)

        server.set_response("Look", "[ player, food, ,  ]")
        self.ai.shared_inventory = {}
        result = self.ai.can_evolve({Objects.FOOD.value: 8})
        self.assertFalse(result)

    def test_drop_all_stones(self):
        inventory = {"food": 8, "deraumere": 1, "sibur": 1}
        self.ai.drop_all_stones(inventory)
        output = self.output.read()
        self.assertIn("Dropped all stones needed to evolve.", output)
        self.assertTrue(output.count("Broadcast") == 2)
        self.assertIn("Set deraumere", output)
        self.assertIn("Set sibur", output)

        inventory = {"food": 8, "deraumere": 1, "sibur": 1}
        self.ai.drop_all_stones(inventory, Objects.DERAUMERE)
        output = self.output.read()
        self.assertIn("Dropped all stones needed to evolve.", output)
        self.assertTrue(output.count("Broadcast") == 1)
        self.assertIn("Set deraumere", output)
        self.assertIn("Set sibur", output)

        self.ai.drop_all_stones()
        output = self.output.read()
        self.assertIn("Dropped all stones needed to evolve.", output)
        self.assertNotIn("Broadcast", output)
        self.assertNotIn("Set deraumere", output)
        self.assertNotIn("Set sibur", output)

    def test_check_requirements(self):
        inventory = {Objects.FOOD.value: 8, Objects.DERAUMERE.value: 1, Objects.SIBUR.value: 1}
        self.assertTrue(self.ai.check_requirements(inventory))

        self.ai.fast_mode = True
        self.assertFalse(self.ai.check_requirements())

        server.set_response("Look", "[ player, food, ,  ]")
        self.assertFalse(self.ai.check_requirements())

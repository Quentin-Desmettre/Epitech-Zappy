import sys
from os import remove
from unittest import TestCase
from tests.ai.globals import ai, server
from src.ai.logic._finding import generate_heat_map, get_path_from_index,\
get_index_from_path, get_object_path, is_object_on_tile, go_to_object, loot_object
from src.ai.commands import Objects, Directions


class FindingTester(TestCase):

    def setUp(self):
        global ai
        self.ai = ai
        sys.stdout = open("finding_output", "w+", buffering=1)
        self.output = open("finding_output", "r+")

    def tearDown(self):
        global server
        server.reset()
        sys.stdout.close()
        sys.stdout = sys.__stdout__
        self.output.close()
        remove("finding_output")

    def test_heat_map(self):
        tiles = [
            ["player"],
            ["food"],
            ["linemate"],
            ["food", "linemate"],
        ]
        heat_map_food = generate_heat_map(Objects.FOOD, tiles)
        self.assertEqual(heat_map_food, {2: [1, 3]})

        heat_map_linemate = generate_heat_map(Objects.LINEMATE, tiles)
        self.assertEqual(heat_map_linemate, {1: [2], 2: [3]})

    def test_path_from_index(self):
        path_forward = get_path_from_index(2)
        self.assertEqual(path_forward, [Directions.FORWARD])

        path_forward = get_path_from_index(1)
        self.assertEqual(path_forward, [Directions.FORWARD, Directions.LEFT, Directions.FORWARD])

        path_forward = get_path_from_index(8)
        self.assertEqual(path_forward, [Directions.FORWARD, Directions.FORWARD, Directions.RIGHT, Directions.FORWARD, Directions.FORWARD])

        path_forward = get_path_from_index(13)
        self.assertEqual(path_forward, [Directions.FORWARD, Directions.FORWARD, Directions.FORWARD, Directions.RIGHT, Directions.FORWARD])

    def test_index_from_path(self):
        index = get_index_from_path([Directions.FORWARD])
        self.assertEqual(index, 2)

        index = get_index_from_path([Directions.FORWARD, Directions.LEFT, Directions.FORWARD])
        self.assertEqual(index, 1)

        index = get_index_from_path([Directions.FORWARD, Directions.FORWARD, Directions.RIGHT, Directions.FORWARD, Directions.FORWARD])
        self.assertEqual(index, 8)

        index = get_index_from_path([Directions.FORWARD, Directions.FORWARD, Directions.FORWARD, Directions.RIGHT, Directions.FORWARD])
        self.assertEqual(index, 13)

    def test_get_object_path(self):
        tiles = [
            ["player"],
            ["food"],
            ["linemate"],
            ["food", "linemate"],
        ]
        path = get_object_path(Objects.FOOD, tiles)
        self.assertEqual(path, [Directions.FORWARD, Directions.LEFT, Directions.FORWARD])

        path = get_object_path(Objects.DERAUMERE, tiles)
        self.assertEqual(path, [])

    def test_is_object_on_tile(self):
        tiles = [
            ["player"],
            ["food"],
            ["linemate"],
            ["food", "linemate"],
        ]
        is_here = is_object_on_tile(tiles, [Directions.FORWARD, Directions.LEFT, Directions.FORWARD], Objects.FOOD)
        self.assertTrue(is_here)

        is_here = is_object_on_tile(tiles, [Directions.FORWARD, Directions.LEFT, Directions.FORWARD], Objects.LINEMATE)
        self.assertFalse(is_here)

        is_here = is_object_on_tile(tiles, [Directions.FORWARD, Directions.RIGHT, Directions.FORWARD], Objects.LINEMATE)
        self.assertTrue(is_here)

        is_here = is_object_on_tile(tiles, [Directions.RIGHT], Objects.LINEMATE)
        self.assertFalse(is_here)


    def test_go_to_object(self):
        global server
        looted = self.ai.go_to_object(Objects.FOOD)
        self.assertTrue(looted)
        output = self.output.read()
        self.assertEqual(output.count("Forward"), 2)
        self.assertEqual(output.count("Left"), 1)
        self.assertTrue("Take food" in output)

        tiles = [
            ["player", "food"],
            ["food"],
            ["linemate", "food"],
            ["food", "linemate"],
        ]
        looted = self.ai.go_to_object(Objects.LINEMATE, tiles, True)
        self.assertTrue(looted)
        output = self.output.read()
        self.assertEqual(output.count("Forward"), 1)
        self.assertFalse("Left" in output)
        self.assertFalse("Right" in output)
        self.assertEqual(output.count("Take food"), 2)
        self.assertTrue("Take linemate" in output)

        server.set_response("Forward", "ko")
        looted = self.ai.go_to_object(Objects.FOOD)
        self.assertFalse(looted)

        server.set_response("Look", "ko")
        looted = self.ai.go_to_object(Objects.FOOD)
        self.assertFalse(looted)

    def test_loot_object(self):
        tiles = [
            ["player", "food"],
            ["food"],
            ["linemate", "food"],
            ["food", "linemate"],
        ]
        looted = self.ai.loot_object(Objects.FOOD, tiles=tiles)
        self.assertTrue(looted)
        output = self.output.read()
        self.assertEqual(output.count("Take food"), 1)
        self.assertFalse("Left" in output)
        self.assertFalse("Right" in output)
        self.assertFalse("Forward" in output)

        looted = self.ai.loot_object(Objects.LINEMATE, tiles=tiles, loot_food=True)
        self.assertTrue(looted)
        output = self.output.read()
        self.assertEqual(output.count("Forward"), 1)
        self.assertFalse("Left" in output)
        self.assertFalse("Right" in output)
        self.assertEqual(output.count("Take food"), 2)
        self.assertTrue("Take linemate" in output)

        looted = self.ai.loot_object(Objects.DERAUMERE, tiles=tiles)
        self.assertFalse(looted)
        output = self.output.read()
        self.assertTrue("Left" in output or "Right" in output or "Forward" in output)

        looted = self.ai.loot_object(Objects.DERAUMERE, False, tiles)
        self.assertFalse(looted)
        output = self.output.read()
        self.assertFalse("Left" in output or "Right" in output or "Forward" in output)

        server.set_response("Take", "ko")
        looted = self.ai.loot_object(Objects.FOOD, tiles=tiles)
        self.assertFalse(looted)

        server.set_response("Look", "ko")
        looted = self.ai.loot_object(Objects.FOOD)
        self.assertFalse(looted)


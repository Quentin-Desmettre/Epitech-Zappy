import sys
from os import remove
from unittest import TestCase
from tests.ai.globals import ai, server
from src.ai.logic._broadcast import add_to_shared_inventory, walk_and_loot, choose_action, add_to_uuids, parse_message
from src.ai.commands import Directions
from src.ai.utils import set_verbose


class BroadcastTester(TestCase):

    def setUp(self):
        global ai
        self.ai = ai
        sys.stdout = open("broadcast_output", "w+", buffering=1)
        self.output = open("broadcast_output", "r+")
        set_verbose(True)

    def tearDown(self):
        global server
        server.reset()
        sys.stdout.close()
        sys.stdout = sys.__stdout__
        self.output.close()
        remove("broadcast_output")

    def test_shared_inventory(self):
        add_to_shared_inventory(self.ai, "test", 3)
        self.assertEqual(self.ai.shared_inventory["test"], 3)

        add_to_shared_inventory(self.ai, "test", 2)
        self.assertEqual(self.ai.shared_inventory["test"], 5)

        add_to_shared_inventory(self.ai, "test", -2)
        self.assertEqual(self.ai.shared_inventory["test"], 3)

        add_to_shared_inventory(self.ai, "test", -3)
        self.assertEqual(self.ai.shared_inventory.get("test"), None)

        try:
            add_to_shared_inventory(self.ai, "test", -3)
        except: # pragma: no cover
            self.fail("add_to_shared_inventory() raised an exception unexpectedly !")

    def test_walk_and_loot(self):
        global server
        server.set_response("Look", "[ player food, linemate, food, ,  ]")
        self.ai.walk_and_loot(Directions.TOP_LEFT)
        output = self.output.read()
        self.assertIn("Moving to other player", output)
        self.assertTrue(output.count("Take food") == 2)

    def test_choose_action(self):
        self.ai.choose_action({}, "looted~|team1~|food~|1", "me", Directions.TOP_LEFT)
        self.assertEqual(self.ai.shared_inventory["food"], 1)

        self.ai.choose_action({}, "dropped~|team1~|food~|1", "me", Directions.TOP_LEFT)
        self.assertEqual(self.ai.shared_inventory.get("food"), None)

        self.ai.choose_action({}, "incantation~|team1~|1", "me", Directions.TOP_LEFT)
        self.assertEqual(self.ai.leader, "me")
        output = self.output.read()
        self.assertIn("New leader: me", output)

        self.ai.choose_action({}, "incantation~|team1~|1", "aaaa", Directions.TOP_LEFT)
        self.assertEqual(self.ai.leader, "me")
        output = self.output.read()
        self.assertIn("Not the leader !!!", output)

        self.ai.choose_action({}, "incantation~|team1~|1", "me", Directions.HERE)
        self.assertEqual(self.ai.leader, "me")
        output = self.output.read()
        self.assertIn("Dropped all stones needed to evolve.", output)

    def test_add_to_uuids(self):
        self.ai.add_to_uuids("test")
        self.assertIn("test", self.ai.messages_uuids)

        for i in range(0, 1000):
            self.ai.add_to_uuids(str(i))
        self.assertTrue(len(self.ai.messages_uuids) == 1000)
        for i in range(0, 1000):
            self.ai.messages_uuids.pop()

    def test_parse_message(self):
        self;ai.parse_message("message 4, uuid1|~uuid2|~looted~|team1~|linemate~|1")
        self.assertEqual(self.ai.shared_inventory["linemate"], 1)

        self;ai.parse_message("message 4, uuid1|~uuid2|~looted~|linemate~|1")
        output = self.output.read()
        self.assertIn("Sending: Broadcast uuid1|~uuid2|~looted~|linemate~|1ø", output)

        self;ai.parse_message("message 4, " + self.ai.id + "|~new_uuid|~looted~|team1~|linemate~|1")
        output = self.output.read()
        self.assertIn("My message, ignoring...", output)

        self;ai.parse_message("message 4, uuid1|~uuid2|~looted~|team1~|linemate~|1")
        output = self.output.read()
        self.assertIn("Already received this message, ignoring...", output)

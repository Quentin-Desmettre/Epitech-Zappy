import sys
from os import remove
from unittest import TestCase
from tests.ai.globals import ai, server
from src.ai.commands import CommandNames
from src.ai.utils import set_verbose


class ReaderTester(TestCase):

    def setUp(self):
        global ai
        self.reader = ai.reader
        sys.stdout = open("reader_output", "w+", buffering=1)
        self.output = open("reader_output", "r+")
        set_verbose(True)

    def tearDown(self):
        global server
        server.reset()
        sys.stdout.close()
        sys.stdout = sys.__stdout__
        self.output.close()
        remove("reader_output")

    def test_sending_message(self):
        global server
        server.set_response("Left", "message 0, incantation\nok")
        self.reader.send(CommandNames.LEFT)
        self.assertTrue(self.reader.has_broadcast())
        self.assertEqual(self.reader.broadcast_pop()[0], "message 0, incantation")

    def test_elevation_underway(self):
        global server
        server.set_response("Forward", "Elevation underway\nCurrent level: 2")
        try:
            self.reader.send(CommandNames.FORWARD)
            self.fail("Should have raised an exception") # pragma: no cover
        except Exception as e:
            self.assertEqual(str(e), "Current level: 2")

    def test_ko(self):
        global server
        server.set_response("Forward", "okay\nok")
        response = self.reader.send(CommandNames.FORWARD)
        self.assertEqual(response, "ok")
        output = self.output.read()
        self.assertIn("okay\n", output)

        server.set_response("Incantation", "ko\nko")
        response = self.reader.send(CommandNames.INCANTATION)
        self.assertEqual(response, None)

        server.set_response("Forward", "Elevation underway\ntestt\nko")
        response = self.reader.send(CommandNames.FORWARD)
        self.assertEqual(response, None)

    def test_broadcast_contains(self):
        global server
        server.set_response("Forward", "message 0, incantation\nok")
        self.reader.send(CommandNames.FORWARD)
        server.set_response("Forward", "message 0, test_message\nok")
        self.reader.send(CommandNames.FORWARD)
        self.assertTrue(self.reader.has_broadcast())
        self.assertTrue(self.reader.broadcast_contains("incantation"))
        self.assertTrue(self.reader.broadcast_contains("test_message"))
        self.assertFalse(self.reader.broadcast_contains("incantation2"))
        self.reader.clean_broadcast_queue()
        self.assertTrue(self.reader.has_broadcast())
        self.assertFalse(self.reader.broadcast_contains("incantation"))
        self.assertTrue(self.reader.broadcast_contains("test_message"))
        msg = self.reader.broadcast_pop()
        self.assertEqual(msg[0], "message 0, test_message")

    def test_empty_broadcast_queue(self):
        global server
        server.set_response("Forward", "message 0, incantation\nok")
        self.reader.send(CommandNames.FORWARD)
        server.set_response("Forward", "message 0, test_message\nok")
        self.reader.send(CommandNames.FORWARD)
        self.assertTrue(self.reader.has_broadcast())
        self.reader.empty_broadcast_queue()
        self.assertFalse(self.reader.has_broadcast())

    def test_zdead(self):
        global server
        server.set_response("Look", "dead")
        try:
            self.reader.send(CommandNames.LOOK)
            self.fail("Should have raised an exception") # pragma: no cover
        except Exception as e:
            self.assertEqual(str(e), "You died")

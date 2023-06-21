import sys
from os import remove
from unittest import TestCase
from tests.ai.globals import ai, server
from src.ai.utils import set_verbose, my_print, set_color, Colors, send_to_server, recv_from_server, merge_dicts


class UtilsTester(TestCase):

    def setUp(self):
        global ai
        self.reader = ai.reader
        sys.stdout = open("utils_output", "w+", buffering=1)
        self.output = open("utils_output", "r+")
        set_verbose(True)

    def tearDown(self):
        global server
        server.reset()
        sys.stdout.close()
        sys.stdout = sys.__stdout__
        self.output.close()
        remove("utils_output")

    def test_my_print(self):
        my_print("test")
        output = self.output.read()
        self.assertIn("test", output)
        set_verbose(False)
        my_print("test")
        output = self.output.read()
        self.assertNotIn("test", output)
        set_color(Colors.OKGREEN)

    def test_send_recv_to_server(self):
        global server
        send_to_server(self.reader.sock, "Forward")
        output = self.output.read()
        self.assertIn("Sending: Forward", output)
        reponse = recv_from_server(self.reader.sock)
        self.assertEqual(reponse, "ok")

        server.set_response("Forward", "dead")
        send_to_server(self.reader.sock, "Forward")
        try:
            recv_from_server(self.reader.sock)
            self.fail("Invalid command did not raise exception") # pragma: no cover
        except SystemExit as e:
            self.assertEqual(e.code, 0)

    def test_merge_dicts(self):
        dict1 = {"a": 1, "b": 2}
        dict2 = {"c": 3, "d": 4}
        dict3 = {"e": 5, "a": 6}
        merged = merge_dicts(dict1, merge_dicts(dict2, dict3))
        self.assertEqual(merged, {"a": 7, "b": 2, "c": 3, "d": 4, "e": 5})

import socket, random, queue, time
from src.ai.commands import Command, CommandNames, Objects, Directions, go_to_direction
from src.ai.logic.finding import go_to_object
from src.ai.logic.evolve import has_stones, get_needed_stones, get_items_on_ground, get_elevation_needs
from src.ai.neural_network.layer import Layer
from src.ai.neural_network.network import NeuralNetwork, NeuralNetworkException
from src.ai.utils import merge_dicts, queue_contains, my_print, set_color, Colors


class Ai:
    """Artificial intelligence class."""

    def __init__(self, server: socket.socket, team: str, model: NeuralNetwork = None) -> None:
        self.X = None
        self.server = server
        self.team = team
        self.shared_inventory = {}
        self.level = 1
        self.broadcast_queue = queue.Queue()
        self.last_movement = time.time()
        self.delta = 0
        self.can_send = True
        self.calibrate()

        if model is None:
            self.neural_network = NeuralNetwork()
            self.neural_network.add_layer(Layer(7, 10), is_input=True)
            self.neural_network.add_layer(Layer(10, 10))
            self.neural_network.add_layer(Layer(10, 2), is_output=True)
        else:
            self.neural_network = model
        self.score = 0


    def calibrate(self):
        current_time = time.time()
        Command(CommandNames.INVENTORY).send(self.server, self.broadcast_queue)
        self.delta = time.time() - current_time
        Command(CommandNames.INVENTORY).send(self.server, self.broadcast_queue)
        self.delta = (time.time() - current_time - self.delta) / 2

    def add_to_shared_inventory(self, object: str, amount: int) -> None:
        """Adds an object to the inventory."""
        if amount < 0:
            if object not in self.shared_inventory:
                return
            self.shared_inventory[object] += amount
            if self.shared_inventory[object] <= 0:
                del self.shared_inventory[object]
        else:
            if object not in self.shared_inventory:
                self.shared_inventory[object] = 0
            self.shared_inventory[object] += amount

    def parse_message(self, msg: str) -> None:
        """Parses a broadcast response."""
        splitted = msg.split(', ')
        direction = Directions(int(splitted[0].split(' ')[1]))
        msg = splitted[1].strip()
        if msg.count(self.team) == 0:
            return
        if msg.startswith("looted"):
            self.add_to_shared_inventory(msg.split(':')[2], 1)
        elif msg.startswith("dropped"):
            self.add_to_shared_inventory(msg.split(':')[2], -1)
        elif msg.startswith("moved"):
            self.can_send = True
        elif msg.startswith("incantation"):
            # inventory = Command(CommandNames.INVENTORY).send(self.server, self.broadcast_queue)
            if direction == Directions.HERE:
                self.drop_elevation_stones(False)
            else:
            # and not self.can_evolve(merge_dicts(get_items_on_ground(self.server, self.broadcast_queue), inventory)):
                my_print("moving to other player")
                go_to_direction(self.server, direction, self.broadcast_queue)
                self.last_movement = time.time()

    def elevate(self, send_cmd: bool = True, cmd_type: CommandNames = None, msg: str = None):
        if send_cmd:
            Command(CommandNames.INCANTATION).send(self.server, self.broadcast_queue)
        elif cmd_type != None:
            # if cmd_type != CommandNames.INCANTATION:
            #     my_print("read useless message")
            #     recv_from_server(self.server)
            if msg.startswith("Current level:"):
                self.level = int(msg.split(" ")[2])
                my_print("Elevated to level %d !!!" % self.level)
                if self.level == 8:
                    exit(42)
            else:
                my_print("Error: could not elevate")

    def can_evolve(self, inventory: dict[str, int]):
        """Returns whether the player can evolve or not."""
        return has_stones(merge_dicts(inventory, self.shared_inventory), self.level)

    def move_randomly(self):
        """Moves and turns the player randomly."""
        rand = random.randint(1, 3)
        if rand == 1:
            Command(CommandNames.FORWARD).send(self.server, self.broadcast_queue)
        elif rand == 2:
            Command(CommandNames.RIGHT).send(self.server, self.broadcast_queue)
        else:
            Command(CommandNames.LEFT).send(self.server, self.broadcast_queue)

    def loot_object(self, object: Objects, can_move_randomly: bool = True) -> bool:
        """Loots the object from the map if there is any, otherwise moves randomly."""
        tiles = Command(CommandNames.LOOK).send(self.server, self.broadcast_queue)
        if tiles == None:
            return False
        if object.value in tiles[0] and (tiles[0].count(Objects.PLAYER.value) <= 1 or object == Objects.FOOD):
            if Command(CommandNames.TAKE, object.value).send(self.server, self.broadcast_queue) == "ko":
                my_print("Error: could not loot %s" % object.name)
                return False
        elif go_to_object(self.server, object, tiles, self.broadcast_queue) == False:
            if can_move_randomly:
                self.move_randomly()
            self.last_movement = time.time()
            return False
        return True

    def take_food(self, inventory: dict[str, int]):
        """Takes food from the map until the player has 20 food."""
        if "food" not in inventory:
            inventory["food"] = 0
        for i in range(15 - inventory["food"]):
            if not self.loot_object(Objects.FOOD):
                i -= 1

    def take_stones(self, inventory: dict[str, int]):
        """Takes stones from the map until the player has all the stones needed to evolve."""
        needed = get_needed_stones(inventory, self.level)
        sucess = False
        stone = None
        while len(needed) > 0:
            stone = needed.pop()
            if self.loot_object(stone, False):
                sucess = True
                break
        if not sucess:
            self.last_movement = time.time()
            return self.move_randomly()
        if stone.value not in inventory:
            inventory[stone.value] = 0
        inventory[stone.value] += 1
        if not self.can_evolve(merge_dicts(get_items_on_ground(self.server, self.broadcast_queue), inventory)):
            Command(CommandNames.BROADCAST, "looted:" + self.team + ":" + stone.value).send(self.server, self.broadcast_queue)
            time.sleep(self.delta)

    def drop_elevation_stones(self, is_main_player = False) -> bool:
        """Drops all the stones needed to evolve and returns whether the player can evolve or not."""
        inventory = Command(CommandNames.INVENTORY).send(self.server, self.broadcast_queue)
        ground = get_items_on_ground(self.server, self.broadcast_queue)
        total = merge_dicts(inventory, ground)
        if is_main_player:
            if len(get_needed_stones(total, self.level)) != 0:
                my_print("Not enough stones to evolve.")
                return False
            if ground[Objects.PLAYER.value] < 8:
                my_print("Not enough players to evolve.")
                return False
        for stone in inventory:
            while inventory[stone] > 0 and stone != Objects.FOOD.value:
                if Command(CommandNames.SET, stone).send(self.server, self.broadcast_queue) != None:
                    Command(CommandNames.BROADCAST, "dropped:" + self.team + ":" + stone).send(self.server, self.broadcast_queue)
                    inventory[stone] -= 1
                    time.sleep(self.delta)
        my_print("Dropped all stones needed to evolve.")
        return True

    def make_decision(self):
        """Takes a decision based on the current state of the game."""
        set_color(None)
        my_print("Making decision")
        inventory = Command(CommandNames.INVENTORY).send(self.server, self.broadcast_queue)
        if inventory is None:
            return
        can_evolve = self.can_evolve(merge_dicts(get_items_on_ground(self.server, self.broadcast_queue), inventory))
        print(can_evolve)
        if "food" not in inventory or inventory["food"] < 5:
            set_color(Colors.WARNING)
            my_print("Looting food")
            self.take_food(inventory)
            self.can_send = True
        elif self.broadcast_queue.qsize() > 0:
            set_color(Colors.OKCYAN)
            msg = self.broadcast_queue.get()
            if msg[0].count("incantation") == 0\
            or (msg[1] > self.last_movement and msg[0].count(str(self.level + 1)) > 0):
                my_print("Analyzing broadcast %s" % msg[0])
                self.parse_message(msg[0])
            if msg[0].count("incantation") > 0:
                Command(CommandNames.BROADCAST, "moved:" + self.team).send(self.server, self.broadcast_queue)
                time.sleep(self.delta)
        elif can_evolve:
            set_color(Colors.HEADER)
            if queue_contains(self.broadcast_queue, "incantation"):
                return
            my_print("Trying to evolve to level %d" % (self.level + 1))
            if self.drop_elevation_stones(True):
                self.elevate()
            elif self.can_send:
                cmd = Command(CommandNames.BROADCAST, "incantation:" + self.team + ":" + str(self.level + 1))
                # if not cmd.read_before_write(self.server, self.broadcast_queue):
                cmd.send(self.server, self.broadcast_queue)
                self.can_send = False
            else:
                my_print("cannot send :(")
        else:
            set_color(Colors.OKGREEN)
            my_print("Looting stones")
            self.take_stones(inventory)

    def convert_data(self, inventory: dict[str, int]):
    #    * / Look / Item needed to evolve

        item_codes = {
            'food': 1,
            'linemate': 2,
            'deraumere': 3,
            'sibur': 4,
            'mendiane': 5,
            'phiras': 6,
            'thystame': 7
        }

        X = []

        numeric_inventory = {}
        for item in inventory:
            multiplicator = len(str(inventory[item]))

            numeric_code = (item_codes[item] * (10 ** multiplicator) + inventory[item])
            numeric_inventory[item] = numeric_code

        maxValue = max(numeric_inventory.values())
        for item in numeric_inventory:
            numeric_inventory[item] = numeric_inventory[item] / maxValue
            X.append(numeric_inventory[item])

        self.X = X

    def compute_decision(self):
        try:
            inventory = Command(CommandNames.INVENTORY).send(self.server, self.broadcast_queue)
            if inventory is None:
                return

            self.convert_data(inventory)
            self.neural_network.run(self.X)

            decision = self.neural_network.output[0]

            if decision[0] > decision[1]:
                if "food" not in inventory or inventory["food"] < 5:
                    self.score += 2
                else:
                    self.score -= 1

                set_color(Colors.WARNING)
                # my_print("Looting food", self.score)
                self.take_food(inventory)
                self.can_send = True
            else:
                if "food" not in inventory or inventory["food"] < 5:
                    self.score -= 1
                else:
                    self.score += 2

                set_color(Colors.OKGREEN)
                # my_print("Looting stones", self.score)
                self.take_stones(inventory)

            if self.score <= -100:
                print("DEAD")
                raise NeuralNetworkException("dead", self.score)

        except NeuralNetworkException as e:
            raise e
        except Exception as e:
            if e.__str__() == "dead":
                raise NeuralNetworkException("dead", self.score)

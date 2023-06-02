import socket, random, queue, time
from src.ai.commands import Command, CommandNames, Objects, Directions, ElevationException, go_to_direction
from src.ai.logic.finding import go_to_object
from src.ai.logic.evolve import has_stones, get_needed_stones, get_items_on_ground, get_elevation_needs
from src.ai.utils import recv_from_server, queue_contains, merge_dicts

class Ai:
    """Artificial intelligence class."""

    def __init__(self, server: socket.socket, team: str) -> None:
        self.server = server
        self.team = team
        self.shared_inventory = {}
        self.level = 1
        self.broadcast_queue = queue.Queue()
        self.last_movement = time.time()

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
        elif msg.startswith("incantation") and direction != Directions.HERE:
            # inventory = Command(CommandNames.INVENTORY).send(self.server, self.broadcast_queue)
            # and not self.can_evolve(merge_dicts(get_items_on_ground(self.server, self.broadcast_queue), inventory))
            if self.level > 1:
                print("moving to other player")
                go_to_direction(self.server, direction, self.broadcast_queue)
                self.last_movement = time.time()
            tiles = Command(CommandNames.LOOK).send(self.server, self.broadcast_queue)
            if list(tiles[0]).count("player") > 1:
                msg = recv_from_server(self.server)
                if msg == "Elevation underway" or msg.startswith("Current level:"):
                    raise ElevationException(CommandNames.INCANTATION, msg)
                self.broadcast_queue.put([msg, time.time()])
        else:
            self.drop_elevation_stones(False)

    def elevate(self, send_cmd: bool = True, cmd_type: CommandNames = None, msg: str = None):
        if send_cmd:
            Command(CommandNames.INCANTATION).send(self.server, self.broadcast_queue)
        elif cmd_type != None:
            if not msg.startswith("Current level:"):
                msg = Command(CommandNames.INCANTATION).read_until_broadcast(self.server, self.broadcast_queue)
            if msg.startswith("Current level:"):
                self.level = int(msg.split(" ")[2])
                print("Elevated to level %d" % self.level)
                # if self.level == 8:
                #     exit(42)
            else:
                print("Error: could not elevate")
            if cmd_type != CommandNames.INCANTATION:
                recv_from_server(self.server)

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
        if object.value in tiles[0]:
            if Command(CommandNames.TAKE, object.value).send(self.server, self.broadcast_queue) == "ko":
                print("Error: could not loot %s" % object.name)
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

    def drop_elevation_stones(self, is_main_player = False) -> bool:
        """Drops all the stones needed to evolve and returns whether the player can evolve or not."""
        inventory = Command(CommandNames.INVENTORY).send(self.server, self.broadcast_queue)
        ground = get_items_on_ground(self.server, self.broadcast_queue)
        total = merge_dicts(inventory, ground)
        to_drop = get_needed_stones(ground, self.level)
        if is_main_player:
            if len(get_needed_stones(total, self.level)) != 0:
                print("Not enough stones to evolve.")
                return False
            if ground[Objects.PLAYER.value] < get_elevation_needs(self.level)[Objects.PLAYER]:
                print("Not enough players to evolve.")
                return False
        for stone in to_drop:
            while inventory[stone.value] > 0 and stone in to_drop:
                Command(CommandNames.SET, stone.value).send(self.server, self.broadcast_queue)
                Command(CommandNames.BROADCAST, "dropped:" + self.team + ":" + stone.value).send(self.server, self.broadcast_queue)
                inventory[stone.value] -= 1
        print("Dropped all stones needed to evolve.")
        return True

    def make_decision(self):
        """Takes a decision based on the current state of the game."""
        inventory = Command(CommandNames.INVENTORY).send(self.server, self.broadcast_queue)
        if inventory == None:
            return
        if "food" not in inventory or inventory["food"] < 5:
            print("Looting food")
            self.take_food(inventory)
        elif self.broadcast_queue.qsize() > 0:
            msg = self.broadcast_queue.get()
            if msg[0].count("incantation") == 0 or msg[1] > self.last_movement:
                print("Analyzing broadcast")
                self.parse_message(msg[0])
        elif self.can_evolve(merge_dicts(get_items_on_ground(self.server, self.broadcast_queue), inventory)):
            # if queue_contains(self.broadcast_queue, "incantation"):
            #     return
            print("Trying to evolve to level %d" % (self.level + 1))
            if not self.drop_elevation_stones(True):
                Command(CommandNames.BROADCAST, "incantation:" + self.team).send(self.server, self.broadcast_queue)
                time.sleep(0.1)
            else:
                self.elevate()
        else:
            print("Looting stones")
            self.take_stones(inventory)

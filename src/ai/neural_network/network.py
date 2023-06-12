import json

from src.ai.neural_network.layer import *


class NeuralNetwork:
    def __init__(self):
        self.output = None
        self.input_layer: Layer = Layer(0, 0)
        self.layers: list[Layer] = []
        self.output_layer: Layer = Layer(0, 0)
        self.id = random.randint(0, 1000000000)
        self.score = 0
        self.bestScore = 0

    def add_layer(self, layer: Layer, is_input: bool = False, is_output: bool = False):
        if is_input:
            self.input_layer = layer
        elif is_output:
            self.output_layer = layer
        else:
            self.layers.append(layer)

    def run(self, X):
        self.input_layer.forward(Matrix([X]))

        last_output = self.input_layer.output
        for layer in self.layers:
            output = ReLU_activation(last_output)
            layer.forward(output)
            last_output = layer.output

        output = ReLU_activation(last_output)
        self.output_layer.forward(output)
        self.output = softmax_activation(self.output_layer.output).values

    def save_to_file(self, fileName: str):
        model = {
            "input_layer": self.input_layer.save_layer(),
            "hidden_layers": [],
            "output_layer": self.output_layer.save_layer(),
            "id": 0,
            "score": self.score if self.score > self.bestScore else self.bestScore
        }

        for layer in self.layers:
            model["hidden_layers"].append(layer.save_layer())

        model["id"] = self.id

        # fileName: str = fileName + str(model["id"]) + ".json"
        fileName: str = fileName + ".json"
        with open(fileName, "w") as file:
            json.dump(model, file)
        print("Model saved as " + fileName)

    def load_from_file(self, file: str):
        with open(file, "r") as file:
            model = json.load(file)

        self.input_layer = Layer(len(model["input_layer"]["weights"]), len(model["input_layer"]["biases"]))
        self.input_layer.weights = model["input_layer"]["weights"]
        self.input_layer.bias = model["input_layer"]["biases"]

        self.layers = []
        for layer in model["hidden_layers"]:
            new_layer = Layer(len(layer["weights"]), len(layer["biases"]))
            new_layer.weights = layer["weights"]
            new_layer.bias = layer["biases"]
            self.layers.append(new_layer)

        self.output_layer = Layer(len(model["output_layer"]["weights"]), len(model["output_layer"]["biases"]))
        self.output_layer.weights = model["output_layer"]["weights"]
        self.output_layer.bias = model["output_layer"]["biases"]

        self.id = model["id"]
        try:
            self.bestScore = model["score"]
        except KeyError:
            self.bestScore = 0

    def mutate(self):
        for layer in self.layers:
            for index in range(len(layer.weights)):
                for index2 in range(len(layer.weights[index])):
                    layer.weights[index][index2] += random.uniform(-0.1, 0.1)

            for index in range(len(layer.bias)):
                layer.bias[index] += random.uniform(-0.1, 0.1)

        self.id = random.randint(0, 1000000000)
        self.score = 0
        self.bestScore = 0


class NeuralNetworkException(Exception):
    """Exception raised to catch elevation message."""

    def __init__(self, status: str, score: int) -> None:
        self.status = status
        self.score = score

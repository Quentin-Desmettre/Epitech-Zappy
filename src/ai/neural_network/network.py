import json

from src.ai.neural_network.layer import *


class NeuralNetwork:
    def __init__(self):
        self.input_layer: Layer = Layer(0, 0)
        self.layers: list[Layer] = []
        self.output_layer: Layer = Layer(0, 0)

    def add_layer(self, layer: Layer, is_input: bool = False, is_output: bool = False):
        if is_input:
            self.input_layer = layer
        elif is_output:
            self.output_layer = layer
        else:
            self.layers.append(layer)

    def run(self):
        pass

    def save_to_file(self, fileName: str):
        model = {
            "input_layer": self.input_layer.save_layer(),
            "hidden_layers": [],
            "output_layer": self.output_layer.save_layer(),
            "id": 0
        }

        for layer in self.layers:
            model["hidden_layers"].append(layer.save_layer())

        model["id"] = random.randint(0, 1000000000)

        fileName: str = fileName + str(model["id"]) + ".json"
        with open(fileName, "w") as file:
            json.dump(model, file)
        print("Model saved as " + fileName)

    def load_from_file(self, file: str):
        with open(file, "r") as file:
            model = json.load(file)

        self.input_layer = Layer(len(model["input_layer"]), len(model["hidden_layers"][0]))
        self.output_layer = Layer(len(model["hidden_layers"][-1]), len(model["output_layer"]))

        for index in range(len(model["hidden_layers"])):
            for index2 in range(len(model["hidden_layers"][index])):
                self.layers[index].weights[index2] = model["hidden_layers"][index][index2]["weights"]
                self.layers[index].bias[index2] = model["hidden_layers"][index][index2]["biases"]

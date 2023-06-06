import random
from src.ai.neural_network.mymath import *


class Layer:
    def __init__(self, inputNumber: int, neuronsNumber: int):
        self.output: Matrix = Matrix([])
        self.inputNumber: int = inputNumber
        self.neuronsNumber: int = neuronsNumber

        self.weights: list[list[float]] = [[random.uniform(-0.01, 0.01) for _ in range(neuronsNumber)] for _ in range(inputNumber)]
        self.bias: list[float] = [1 for _ in range(neuronsNumber)]

    def forward(self, inputs: Matrix):
        try:
            self.output = dot(inputs, Matrix(self.weights)) + self.bias
        except Exception as e:
            print(e)
            self.output = None

    def save_layer(self):
        layer = []

        if self.neuronsNumber == 0 or self.inputNumber == 0:
            return layer

        for index in range(self.neuronsNumber - 1):
            layer.append(
                {
                    "weights": self.weights[index],
                    "biases": self.bias[index]
                }
            )
        return layer
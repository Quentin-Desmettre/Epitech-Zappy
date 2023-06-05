import random
from src.ai.neural_network.math import *


class Layer:
    def __init__(self, inputNumber: int, neuronsNumber: int):
        self.output: Matrix = Matrix([])
        self.inputNumber: int = inputNumber
        self.neuronsNumber: int = neuronsNumber

        self.weights: list[list[float]] = [[random.uniform(-0.01, 0.01) for _ in range(neuronsNumber)] for _ in range(inputNumber)]
        self.bias: list[float] = [0 for _ in range(neuronsNumber)]

    def forward(self, inputs: list[list[float]]):
        try:
            self.output = dot(Matrix(inputs), Matrix(self.weights)) + self.bias
        except Exception as e:
            print(e)
            self.output = None

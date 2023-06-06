import math

from src.ai.neural_network.matrix import Matrix


def dot(matrix1: Matrix, matrix2: Matrix):
    if matrix1.size()[1] != matrix2.size()[0]:
        raise Exception("Matrix sizes are not compatible for dot product")

    result = [[0 for _ in range(matrix2.size()[1])] for _ in range(matrix1.size()[0])]

    for i in range(matrix1.size()[0]):
        for j in range(matrix2.size()[1]):
            for k in range(matrix1.size()[1]):
                result[i][j] += matrix1.values[i][k] * matrix2.values[k][j]
    return Matrix(result)


def ReLU_activation(matrix: Matrix):
    result = [[0 for _ in range(matrix.size()[1])] for _ in range(matrix.size()[0])]

    for i in range(matrix.size()[0]):
        for j in range(matrix.size()[1]):
            result[i][j] = max([matrix.values[i][j], 0])
    return Matrix(result)


def softmax_activation(matrix: Matrix):
    result = [[0 for _ in range(matrix.size()[1])] for _ in range(matrix.size()[0])]

    for i in range(matrix.size()[0]):
        maxValue: float = max(matrix.values[i])

        sumValue: float = sum(matrix.values[i])
        if sumValue == 0:
            sumValue = 0.000001

        for j in range(matrix.size()[1]):
            result[i][j] = math.exp(matrix.values[i][j] - maxValue) / sumValue
    return Matrix(result)

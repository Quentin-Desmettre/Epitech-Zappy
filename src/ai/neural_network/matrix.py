class Matrix:
    def __init__(self, values: list[list[float]]):
        self.values: list[list[float]] = values

    def size(self) -> tuple[int, int]:
        return len(self.values), len(self.values[0])

    def __add__(self, other: list[float]):
        if self.size()[1] != len(other):
            raise Exception("Matrix sizes are not compatible for addition")

        result = [[0 for _ in range(self.size()[1])] for _ in range(self.size()[0])]

        for i in range(self.size()[0]):
            for j in range(self.size()[1]):
                result[i][j] = self.values[i][j] + other[j]
        return Matrix(result)

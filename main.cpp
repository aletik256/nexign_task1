#include "solution.h"

#include <random>
#include <iostream>

constexpr int MATRIX_SIZE = 5000;

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, Solution::MAX_MATRIX_VALUE);

    int** matrix = new int*[MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix[i] = new int[MATRIX_SIZE];
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = dist(gen);
        }
    }

    Solution solution(0.1);
    const auto result = solution.GetMostFrequentPrimeNumbers(matrix, MATRIX_SIZE, 5);
    for (int i = 0; i < result.size() - 1; i++) {
        std::cout << result[i] << " ";
    }
    if (!result.empty()) {
        std::cout << result.back() << std::endl;
    }

    return 0;
}

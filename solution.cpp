#pragma once

#include "solution.h"
#include "storage.h"

#include <algorithm>
#include <thread>

class Solution::Impl {
 public:
    Impl(float collisionProbability);

    std::vector<int> GetMostFrequentPrimeNumbers(int** matrix, int size, int count) const;

 private:
    // Поиск в хэшах наиболее часто встречающихся простых чисел
    static std::vector<int> GetNumbersFromStorages(const std::vector<PrimeNumbersStorage>& storages, int count);

    // Вычисление необходимого количества хэшей
    static int GetStorageNumber(int threadsNumber, float collisionProbability);

 private:
    static constexpr int MIN_VALUES_PER_THREAD = 1000;  // Минимальное количество значений на один поток

    float m_CollisionProbability = 0.0;  // Вероятность коллизий
};

Solution::Impl::Impl(float collisionProbability) : m_CollisionProbability(collisionProbability) {}

std::vector<int> Solution::Impl::GetMostFrequentPrimeNumbers(int** matrix, int size, int count) const {
    const auto threadsNumber = std::min((size * size + MIN_VALUES_PER_THREAD - 1) / MIN_VALUES_PER_THREAD,
                                        static_cast<int>(std::thread::hardware_concurrency()));
    // Количество хэшей.
    const auto storagesNumber = std::min(MAX_MATRIX_VALUE, GetStorageNumber(threadsNumber, m_CollisionProbability));
    // Количество строк матрицы, обрабатываемых одним потоком.
    const auto rowsPerThread = (size + threadsNumber - 1) / threadsNumber;

    std::vector<PrimeNumbersStorage> storages(storagesNumber);

    // Заполнение чранилищ простыми числами.
    // Работает в отдельном потоке.
    const auto fillStorages = [&](int firstRow, int lastRow) -> void {
        // Сложность алгоритма квадратичная, т.к. добавление в хранилище
        // производится за константное время.
        for (int i = firstRow; i <= lastRow; i++) {
            for (int j = 0; j < size; j++) {
                const auto storageIndex = matrix[i][j] * storagesNumber / (Solution::MAX_MATRIX_VALUE + 1);
                // Сам вектор не меняется, происходит только получение ссылки на хранилище.
                // Гонки за данными нет. Мьютекс не нужен.
                storages[storageIndex].AddNumber(matrix[i][j]);
            }
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(threadsNumber);
    int firstRow = 0;
    int lastRow = rowsPerThread - 1;

    for (int i = 0; i < threadsNumber; i++) {
        threads.emplace_back(fillStorages, firstRow, lastRow);
        firstRow += rowsPerThread;
        lastRow = std::min(lastRow + rowsPerThread, size - 1);
    }

    for (int i = 0; i < threadsNumber; i++) {
        if (threads[i].joinable()) {
            threads[i].join();
        } else {
            threads[i].detach();
        }
    }

    return GetNumbersFromStorages(storages, count);
}

std::vector<int> Solution::Impl::GetNumbersFromStorages(const std::vector<PrimeNumbersStorage>& storages, int count) {
    // По условию задачи, максимальное значение в матрице не больше 1000.
    // На интервале от 0 до 1000 существует порядка 130 простых чисел (n / ln(n)).
    // Cоберем все значения в одном массиве и отсортируем по частоте.

    std::vector<std::pair<int, int>> values;
    for (const auto& storage : storages) {
        decltype(values) buffer;
        storage.GetNumbers(buffer);
        values.insert(values.end(), buffer.begin(), buffer.end());
    }

    std::sort(values.begin(), values.end(), [](const std::pair<int, int>& left, const std::pair<int, int>& right) {
        return left.second > right.second;
    });

    std::vector<int> result;
    result.resize(std::min(static_cast<size_t>(count), values.size()));
    for (size_t i = 0; i < result.size(); i++) {
        result[i] = values[i].first;
    }

    return result;
}

int Solution::Impl::GetStorageNumber(int threadsNumber, float collisionProbability) {
    // При оценке вероятности было сделано предположение, что все потоки стартуют одновременно.
    // Вероятность коллизии p = 1 - (n-k+1)(n-k+2)...(n-1)n / n ^ k,
    // где n - количество хранилищ, k - количество потоков.

    const auto getProbability = [](int n, int k) -> float {
        float probability = 1.0;
        for (int i = 0; i < k; i++) {
            probability *= static_cast<float>(n - i) / n;
        }
        return 1.0 - probability;
    };

    // Далее по коду подбираем нужное значение
    int result = 1;
    while (getProbability(result, threadsNumber) > collisionProbability) {
        result *= 2;
    }

    int step = result / 4;
    while (step > 0) {
        if (getProbability(result, threadsNumber) > collisionProbability) {
            result += step;
        } else {
            result -= step;
        }
        step /= 2;
    }

    return result;
}

Solution::Solution(float collisionProbability) : m_Impl(std::make_unique<Impl>(collisionProbability)) {}

Solution::~Solution() = default;

std::vector<int> Solution::GetMostFrequentPrimeNumbers(int** matrix, int size, int count) const {
    return m_Impl->GetMostFrequentPrimeNumbers(matrix, size, count);
}

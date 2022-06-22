#pragma once

#include <memory>
#include <vector>

/*!
    \brief Класс, реализующий решение поставленной задачи.
 */
class Solution {
 public:
    /*!
        \brief Конструктор объекта.
        \param[in] collisionProbability  Вероятностью коллизий.
        Под коллизией понимается попытка двух или более потоков одновременно обратиться к хэшу.
     */
    Solution(float collisionProbability);

    /*!
        \brief Деструктор объекта.
     */
    ~Solution();

    /*!
        \brief Поиск наиболее часто встречающихся простых чисел.
        \param[in] matrix  Указатель на массив данных.
        \param[in] size    Размер матрицы.
        \param[in] count   Количество чисел, которые надо найти.
        \return  Вектор простых чисел.
     */
    std::vector<int> GetMostFrequentPrimeNumbers(int** matrix, int size, int count) const;

    static constexpr int MAX_MATRIX_VALUE = 1000;  // Максимальное значение в матрице

 private:
    class Impl;
    std::unique_ptr<Impl> m_Impl;
};

#pragma once

#include <memory>
#include <vector>

/*!
    \brief Потокобозопасный контейнер для хранения простых чисел.
 */
class PrimeNumbersStorage {
 public:
    /*!
        \brief Конструктор.
     */
    PrimeNumbersStorage();

    /*!
        \brief Деструктор.
     */
    ~PrimeNumbersStorage();

    /*!
        \brief Добавление простого числа в контейнер.
        \param[in] number  Добавляемое значение.
        \return  true, если значение было добавлено.

        Если число не простое, добавление не происходит.
        Если число уже есть, увеличивается счетчик вхождений.
        Сложность O(1).
     */
    bool AddNumber(int number);

    /*!
        \brief Получение элементов из контейнера.
        \param[out] numbers  Вектор выходных значений.

        Первое значение в векторе - число, второе - количество вхождений.
     */
    void GetNumbers(std::vector<std::pair<int, int>>& numbers) const;

    /*!
        \brief Очистка контейнера.
     */
    void Clear();

 private:
    class Impl;
    std::unique_ptr<Impl> m_Impl;
};

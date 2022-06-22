#include "storage.h"

#include <cmath>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>

class PrimeNumbersStorage::Impl {
 public:
    bool AddNumber(int number);
    void GetNumbers(std::vector<std::pair<int, int>>& numbers) const;
    void Clear();

 private:
    // Проверка числа на простоту
    static bool IsPrimeNumber(int number);

 private:
    mutable std::shared_mutex m_Mutex;
    std::unordered_map<int, int> m_Hash;  // Ключ - простое число, значение - количество вхождений
};

bool PrimeNumbersStorage::Impl::AddNumber(int number) {
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    auto iter = m_Hash.find(number);
    if (iter != m_Hash.end()) {
        iter->second++;
        return true;
    } else if (IsPrimeNumber(number)) {
        return m_Hash.emplace(number, 1).second;
    }
    return false;
}
void PrimeNumbersStorage::Impl::GetNumbers(std::vector<std::pair<int, int>>& numbers) const {
    std::shared_lock<std::shared_mutex> lock(m_Mutex);
    numbers.reserve(m_Hash.size());
    for (const auto& number : m_Hash) {
        numbers.push_back(number);
    }
}

void PrimeNumbersStorage::Impl::Clear() {
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    m_Hash.clear();
}

bool PrimeNumbersStorage::Impl::IsPrimeNumber(int number) {
    if (number == 1 || number == 2) {
        return true;
    }
    if (number % 2 == 0) {
        return false;
    }
    const auto maxDivider = static_cast<int>(std::sqrt(number));
    for (int i = 3; i <= maxDivider; i += 2) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

PrimeNumbersStorage::PrimeNumbersStorage() : m_Impl(std::make_unique<Impl>()) {}

PrimeNumbersStorage::~PrimeNumbersStorage() = default;

bool PrimeNumbersStorage::AddNumber(int number) {
    return m_Impl->AddNumber(number);
}

void PrimeNumbersStorage::GetNumbers(std::vector<std::pair<int, int>>& numbers) const {
    m_Impl->GetNumbers(numbers);
}

void PrimeNumbersStorage::Clear() {
    m_Impl->Clear();
}

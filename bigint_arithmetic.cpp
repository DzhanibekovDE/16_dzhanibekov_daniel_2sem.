#include <iostream>
#include <string>
#include <algorithm>
#include <cassert> // Для assert
#include <cmath> // Для std::fabs
#include <vector>
#include <chrono>
#include <math.h>

class BigInteger {
public:
    // Конструкторы
    BigInteger() : number("0"), isNegative(false) {}
    explicit BigInteger(std::string str) {
        if (str[0] == '-') {
            isNegative = true;
            str = str.substr(1);
        } else {
            isNegative = false;
        }
        number = str;
    }

    std::string toString() const {
        return number; // Возвращаем строковое представление числа
    }

    // Оператор равенства
    bool operator==(const BigInteger& other) const {
        return number == other.number && isNegative == other.isNegative;
    }

    // Оператор неравенства
    bool operator!=(const BigInteger& other) const {
        return !(*this == other);
    }

    // Оператор меньше
    bool operator<(const BigInteger& other) const {
        if (isNegative != other.isNegative) {
            return isNegative;
        }

        if (number.length() != other.number.length()) {
            return isNegative ^ (number.length() < other.number.length());
        }

        for (size_t i = 0; i < number.length(); i++) {
            if (number[i] != other.number[i]) {
                return isNegative ^ (number[i] < other.number[i]);
            }
        }

        return false; // числа равны
    }

    // Оператор меньше или равно
    bool operator<=(const BigInteger& other) const {
        return *this < other || *this == other;
    }

    // Оператор больше
    bool operator>(const BigInteger& other) const {
        return !(*this <= other);
    }

    // Оператор больше или равно
    bool operator>=(const BigInteger& other) const {
        return !(*this < other);
    }

  BigInteger add(const BigInteger& other) const {
    std::string result;
    int carry = 0;
    int sum = 0;

    // Дополнение более короткого числа нулями слева
    std::string a = this->number;
    std::string b = other.number;
    if (a.length() < b.length()) {
        a.insert(0, b.length() - a.length(), '0');
    } else {
        b.insert(0, a.length() - b.length(), '0');
    }

    // Сложение чисел
    for (int i = a.length() - 1; i >= 0; --i) {
        sum = (a[i] - '0') + (b[i] - '0') + carry;
        carry = sum / 10;
        result.insert(0, 1, (sum % 10) + '0');
    }

    // Добавление оставшегося переноса, если он есть
    if (carry) {
        result.insert(0, 1, carry + '0');
    }

    return BigInteger(result);
}

    BigInteger subtract(const BigInteger& other) const {
    std::string result;
    int carry = 0;
    int diff = 0;

    // Предполагаем, что первое число всегда больше второго
    std::string a = this->number;
    std::string b = other.number;
    b.insert(0, a.length() - b.length(), '0');

    // Вычитание чисел
    for (int i = a.length() - 1; i >= 0; --i) {
        diff = (a[i] - '0') - (b[i] - '0') - carry;
        if (diff < 0) {
            diff += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result.insert(0, 1, diff + '0');
    }

    size_t nonZero = result.find_first_not_of('0');
    if (nonZero != std::string::npos) {
        result.erase(0, nonZero);
    } else {
        // Если строка состоит только из нулей, значит, результат равен 0
        result = "0";
    }

    return BigInteger(result);
}

    BigInteger multiply(const BigInteger& other) const {
    int n1 = this->number.size();
    int n2 = other.number.size();
    std::vector<int> pos(n1 + n2, 0);

    // Умножение чисел
    for (int i = n1 - 1; i >= 0; --i) {
        for (int j = n2 - 1; j >= 0; --j) {
            int mul = (this->number[i] - '0') * (other.number[j] - '0');
            int sum = mul + pos[i + j + 1];

            pos[i + j] += sum / 10;
            pos[i + j + 1] = sum % 10;
        }
    }

    std::string result;
    for (int p : pos) {
        if (!(result.empty() && p == 0)) { // Пропускаем ведущие нули
            result += std::to_string(p);
        }
    }

    size_t nonZero = result.find_first_not_of('0');
    if (nonZero != std::string::npos) {
        result.erase(0, nonZero);
    } else {
        // Если строка состоит только из нулей, значит, результат равен 0
        result = "0";
    }

    return BigInteger(result);
}
    // Деление чисел
    BigInteger divide(const BigInteger& other, int precision = 33) const {
        if (other == BigInteger("0")) {
            throw std::invalid_argument("Division by zero");
        }

        std::string result;
        BigInteger remainder = *this; // Начинаем с делимого
        BigInteger divisor = other;
        bool decimalPointAdded = false;

        // Добавляем нули к остатку для достижения требуемой точности
        for (int i = 0; i <= precision; ++i) {
            int count = 0;
            while (remainder >= divisor || (i == 0 && count == 0)) {
                remainder = remainder.subtract(divisor);
                ++count;
            }
            result += std::to_string(count);
            if (!decimalPointAdded && i == 0) {
                result += ".";
                decimalPointAdded = true;
            }
            remainder = remainder.multiply(BigInteger("10"));
        }

        // Удаление ненужных нулей после запятой
        result.erase(result.find_last_not_of('0') + 1, std::string::npos);
        if (result.back() == '.') {
            result.pop_back();
        }

        return BigInteger(result);
    }

private:
    std::string number; // Хранение числа в виде строки для упрощения демонстрации
    bool isNegative; // Флаг для хранения знака числа

    // Дополнительные вспомогательные методы могут быть определены здесь
};

bool almostEqual(long double x, long double y, long double epsilon = 1e-8) {
    return std::fabs(x - y) < epsilon;
}


void testOperations() {
    std::string input1, input2;

    std::cout << "num1: ";
    std::cin >> input1;

    std::cout << "num2: ";
    std::cin >> input2;

    BigInteger num1(input1);
    BigInteger num2(input2);

    BigInteger sum = num1.add(num2);
    std::cout << "add " << sum.toString() << std::endl;
    std::cout << "\n";

    BigInteger diff = num1.subtract(num2);
    std::cout << "sub " << diff.toString() << std::endl;
    std::cout << "\n";

    BigInteger prod = num1.multiply(num2);
    std::cout << "mult " << prod.toString() << std::endl;
    std::cout << "\n";

    auto start = std::chrono::high_resolution_clock::now();
    BigInteger quotient = num1.divide(num2);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "div " << quotient.toString() << std::endl;
    std::cout << "\n";
    std::cout << "time for divide: " << duration.count() << " ms" << std::endl;
}

int main() {
    try {
        testOperations();
    } catch (const std::exception& e) {
        std::cout << "Произошла ошибка во время тестирования: " << e.what() << std::endl;
    }
    return 0;
}

#include "BigInteger.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert> // Для assert
#include <cmath> // Для std::fabs
#include <vector>
#include <chrono>
#include <math.h>

BigInteger::BigInteger(const std::string& str) {
    if (!str.empty() && str[0] == '-') {
        isNegative = true;
        number = str.substr(1);
    } else if (!str.empty() && str[0] == '+') {
        isNegative = false;
        number = str.substr(1);
    } 
}

std::string BigInteger::toString() const {
    if (this->isNegative)
        return '-'+number;
    else 
        return '+'+number;
}

bool BigInteger::operator==(const BigInteger& other) const {
    return number == other.number && isNegative == other.isNegative;
}

bool BigInteger::operator!=(const BigInteger& other) const {
    return !(*this == other);
}

bool BigInteger::operator<(const BigInteger& other) const {
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

bool BigInteger::operator<=(const BigInteger& other) const {
    return *this < other || *this == other;
}

bool BigInteger::operator>(const BigInteger& other) const {
    return !(*this <= other);
}

bool BigInteger::operator>=(const BigInteger& other) const {
    return !(*this < other);
}

bool BigInteger::less_abs(const BigInteger& other) const {
    if (this->number.length() != other.number.length()) 
        return (this->number.length() < other.number.length());

    for (size_t i = 0; i < number.length(); i++)
        if (this->number[i] != other.number[i])
            return (this->number[i] < other.number[i]);

    return false; // числа равны
}

bool BigInteger::leq_abs(const BigInteger& other) const {
    if (this->number.length() != other.number.length()) 
        return (this->number.length() <= other.number.length());

    for (size_t i = 0; i < number.length(); i++)
        if (this->number[i] != other.number[i])
            return (this->number[i] <= other.number[i]);

    return false; // числа равны
}

BigInteger BigInteger::add(const BigInteger& other) const {
    std::string result;
    int carry = 0;
    int sum = 0;

    // Дополнение более короткого числа нулями слева
    std::string a = this->number;
    std::string b = other.number;

    if (isNegative == other.isNegative){
    
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

        if (isNegative == true)
            return BigInteger('-'+result);
        else 
            return BigInteger('+'+result);
    } else {
        if (other.isNegative){
            return this->subtract(BigInteger('+'+b)); 
        } else {
            return other.subtract(BigInteger('+'+a));
        }
    }
}

BigInteger BigInteger::subtract(const BigInteger& other) const {
    // Вычетание 
    std::string result;
    int carry = 0;
    int diff = 0;

    std::string a, b;
    char sign;

    if (isNegative == other.isNegative){
        if (isNegative == true) {
            if (other.less_abs(*this)){
                a = this->number;
                b = other.number;
                sign = '-';
            } else {
                b = this->number;
                a = other.number;
                sign = '+';
            }         
        } else {
            if (other.less_abs(*this)){
                a = this->number;
                b = other.number;
                sign = '+';
            } else {
                b = this->number;
                a = other.number;
                sign = '-';
            }
        }

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

        if (isNegative == true)
            return BigInteger(sign+result);
        else 
            return BigInteger(sign+result);
    } else {
        if (other.isNegative)
            return this->add(BigInteger('+'+other.number)); 
        else
            return this->add(BigInteger('-'+other.number));
    }  
}

BigInteger BigInteger::multiply(const BigInteger& other) const {
    int n1 = this->number.size();
    int n2 = other.number.size();
    std::vector<int> pos(n1 + n2, 0);

    char sign;
    if (isNegative == other.isNegative) {
        sign = '+';
    } else {
        sign = '-';
    }

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

    return BigInteger(sign+result);
}

BigInteger BigInteger::divide(const BigInteger& other, int precision) const {
    if (other == BigInteger("0")) {
        throw std::invalid_argument("Division by zero");
    }

// Деление
    std::string result;
    BigInteger remainder = *this; // Начинаем с делимого
    BigInteger divisor = other;
    bool decimalPointAdded = false;

    char sign;
    if (isNegative == other.isNegative) {
        sign = '+';
    } else {
        if (isNegative == true){
            remainder = BigInteger('+' + remainder.number);
        } else {
            divisor = BigInteger('+' + divisor.number);
        }
        sign = '-';
    }

    // Добавляем нули к остатку для достижения требуемой точности
    for (int i = 0; i <= precision; ++i) {
        int count = 0;
        // while (remainder >= divisor || (i == 0 && count == 0)) {
        while (divisor.leq_abs(remainder) || (i == 0 && count == 0)) {
            remainder = remainder.subtract(divisor);
            ++count;
        }
        result += std::to_string(count);
        if (!decimalPointAdded && i == 0) {
            result += ".";
            decimalPointAdded = true;
        }
        remainder = remainder.multiply(BigInteger("+10"));
    }

    // Удаление ненужных нулей после запятой
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    if (result.back() == '.') {
        result.pop_back();
    }
    
    return BigInteger(sign+result);
};

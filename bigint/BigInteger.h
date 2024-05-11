#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <string>
#include <vector>

class BigInteger {
public:
    // Конструкторы
    BigInteger();
    explicit BigInteger(const std::string& str);
    
    // Арифметические операторы 
    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator/(const BigInteger& other) const;

 
    // Операторы сравнения
    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;
    
    
    bool less_abs(const BigInteger& other) const;
    bool leq_abs(const BigInteger& other) const;

        // Вспомогательные методы
    std::string toString() const;

private:
    std::string number; // Хранение числа в виде строки для упрощения демонстрации
    bool isNegative; // Флаг для хранения знака числа
};

#endif // BIGINTEGER_H

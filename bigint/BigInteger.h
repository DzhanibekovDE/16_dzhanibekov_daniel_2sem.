#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <string>
#include <vector>

class BigInteger {
public:
    // Конструкторы
    BigInteger();
    explicit BigInteger(const std::string& str);
    
    // Операции
    BigInteger add(const BigInteger& other) const;
    BigInteger subtract(const BigInteger& other) const;
    BigInteger multiply(const BigInteger& other) const;
    BigInteger divide(const BigInteger& other, int precision = 33) const;

    // Вспомогательные методы
    std::string toString() const;

    // Операторы сравнения
    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;
    
    bool less_abs(const BigInteger& other) const;
    bool leq_abs(const BigInteger& other) const;

    //bool isNeg(const BigInteger& other) const {return isNegative;}

private:
    std::string number; // Хранение числа в виде строки для упрощения демонстрации
    bool isNegative; // Флаг для хранения знака числа
};

#endif // BIGINTEGER_H

#include "BigInteger.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert> // Для assert
#include <cmath> // Для std::fabs
#include <vector>
#include <chrono>
#include <math.h>

void testOperations() {
    
    std::string input1, input2;

    std::cout << "num1: ";
    std::cin >> input1;

    std::cout << "num2: ";
    std::cin >> input2;
    std::cout << "\n";

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
        std::cout << "Test error: " << e.what() << std::endl;
    }
    return 0;
}

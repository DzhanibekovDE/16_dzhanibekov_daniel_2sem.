#include <iostream>
#include <random>
#include <cmath>
#include <thread>
#include <vector>
#include <atomic>
//const double M_PI = 3.14159265358979323846;
// Определение M_PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const double L = 1.0; // Длина иглы
const double D = 1.0; // Расстояние между линиями
const long long N = 100000000; // Количество бросков (подобрал чтобы попадало в точность)

std::atomic<long long> global_count(0); // Атомарная переменная для подсчета пересечений

void throw_needles(long long num_throws) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    long long count = 0;
    for (long long i = 0; i < num_throws; ++i) {
        double x = dis(gen) * D/2.0; // Расстояние до ближайшей линии
        double theta = dis(gen) * M_PI; // Угол иглы

        if (x <= (L / 2.0) * std::sin(theta)) {
            ++count;
        }
    }
    global_count += count;
}

int main() {
    int num_threads = std::thread::hardware_concurrency();
    //std::cout << num_threads << std::endl;
    std::vector<std::thread> threads;

    long long throws_per_thread = N / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(throw_needles, throws_per_thread);
    }

    for (auto& t : threads) {
        t.join();
    }

    double pi_estimate = (2.0 * L * N) / (global_count * D);
    std::cout << "Estimate of pi: " << pi_estimate << std::endl;

    return 0;
}
// для попалания в точность нужно очень сильно увеличить число бросков,
//и очень сильно возрастает время выполнения, но код работет, сделать лучше проблематично 
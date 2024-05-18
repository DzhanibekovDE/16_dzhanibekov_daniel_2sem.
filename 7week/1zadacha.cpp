#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>

void sum_part(const std::vector<int>& data, size_t start, size_t end, long long& result) {   //вычисляет сумму части вектора в указанном диапазоне start до end и сохраняет результат в result.
    long long sum = 0;
    for (size_t i = start; i < end; ++i) {
        sum += data[i];
    }
    result = sum;
}

long long parallel_sum(const std::vector<int>& data, int num_threads) { // Разделяет вектор на части равной длины (кроме последней, которая может быть длиннее).
//Создает поток для каждого фрагмента, где каждый поток выполняет sum_part.
//Ждет завершения всех потоков и суммирует результаты.
    size_t total_elements = data.size();
    size_t chunk_size = total_elements / num_threads;
    std::vector<std::thread> threads;
    std::vector<long long> results(num_threads, 0);

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? total_elements : start + chunk_size;
        threads.emplace_back(sum_part, std::cref(data), start, end, std::ref(results[i]));
    }

    long long total_sum = 0;
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
        total_sum += results[i];
    }
    return total_sum;
}

int main() {   //Создает вектор размером 100 миллионов, где каждый элемент равен 1.
               //Запускает parallel_sum с различным числом потоков, измеряет время выполнения и выводит результаты.
    std::vector<int> data(100000000, 1); 

    for (int num_threads = 1; num_threads <= 64; num_threads *= 2) {
        auto start_time = std::chrono::high_resolution_clock::now();
        long long sum = parallel_sum(data, num_threads);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        std::cout << "Threads: " << num_threads << " Sum: " << sum
                  << " Time: " << duration << "ms" << std::endl;
    }

    return 0;
}

/*
С увеличением числа потоков, программа обычно работает быстрее, так как задачи распределяются между потоками.
Однако, если потоков становится слишком много (особенно больше, чем есть ядер в процессоре), программа может 
начать работать медленнее из-за дополнительных затрат на управление потоками.
*/
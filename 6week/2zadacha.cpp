#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <string>

namespace fs = std::filesystem;

int main() {
    // Запросите у пользователя путь к каталогу
    auto path = fs::current_path();
    //std::string path;
    // std::cout << "Введите путь к каталогу: ";
    // std::cin >> path;

    std::unordered_map<std::string, std::pair<int, uintmax_t>> file_info;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (entry.is_regular_file()) {
                auto ext = entry.path().extension().string();
                auto file_size = entry.file_size();
                if (file_info.find(ext) == file_info.end()) {
                    file_info[ext] = std::make_pair(1, file_size);
                } else {
                    file_info[ext].first++;
                    file_info[ext].second += file_size;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "File Extension | Count | Total Size (in bytes)" << std::endl;
    for (const auto& [extension, stats] : file_info) {
        std::cout << extension << " | " << stats.first << " | " << stats.second << std::endl;
    }

    return 0;
}

#include <iostream> // Подключение библиотеки для ввода-вывода.
#include <fstream>  // Подключение библиотеки для работы с файлами.
#include <vector>   // Подключение библиотеки для использования вектора.

struct Color {
    unsigned char r, g, b; // Структура для хранения цвета (красный, зеленый, синий).
};

void setPixel(std::vector<unsigned char>& image, int x, int y, int width, Color color) {
    int index = (x + y * width) * 3; // Вычисление индекса пикселя в векторе.
    image[index] = color.b;          // Установка синего компонента цвета.
    image[index + 1] = color.g;      // Установка зеленого компонента цвета.
    image[index + 2] = color.r;      // Установка красного компонента цвета.
}

void drawSquare(std::vector<unsigned char>& image, int width, int x, int y, int size, Color color) {
    for (int i = 0; i < size; ++i) {      // Цикл по горизонтали.
        for (int j = 0; j < size; ++j) {  // Цикл по вертикали.
            setPixel(image, x + i, y + j, width, color); // Рисование пикселя квадрата.
        }
    }
}

void drawTriangle(std::vector<unsigned char>& image, int width, int x, int y, int size, Color color) {
    for (int i = 0; i < size; ++i) {      // Цикл по высоте треугольника.
        for (int j = 0; j <= i; ++j) {    // Цикл по ширине треугольника.
            setPixel(image, x + j, y + i, width, color); // Рисование пикселя треугольника.
        }
    }
}

void saveBMP(const char* filename, const std::vector<unsigned char>& image, int width, int height) {
    std::ofstream file(filename, std::ios::out | std::ios::binary); // Открытие файла для записи в бинарном режиме.

    if (!file) {
        std::cout << "Cannot open file!" << std::endl; // Проверка на успешное открытие файла.
        return;
    }

    unsigned char bmpPad[3] = {0, 0, 0}; // Заполнение для выравнивания строк до кратности 4 байтам.
    const int paddingAmount = ((4 - (width * 3) % 4) % 4); // Вычисление количества байтов для заполнения.

    const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;
    const int fileSize = fileHeaderSize + infoHeaderSize + width * height * 3 + paddingAmount * height; // Вычисление общего размера файла.

    unsigned char fileHeader[fileHeaderSize] = {
      'B', 'M',  // Сигнатура файла BMP.
        0, 0, 0, 0,  // Размер файла в байтах (будет заполнен позже).
        0, 0, 0, 0,  // Зарезервировано.
        54, 0, 0, 0  // Начало массива пикселей (54 байта от начала файла).
    };

    unsigned char infoHeader[infoHeaderSize] = {
        40, 0, 0, 0,           // Размер информационного заголовка (40 байт).
        0, 0, 0, 0,            // Ширина изображения (будет заполнено позже).
        0, 0, 0, 0,            // Высота изображения (будет заполнено позже).
        1, 0,                  // Количество цветовых планов (всегда 1).
        24, 0,                 // Количество бит на пиксель (24 для RGB).
        0, 0, 0, 0,            // Тип сжатия (0, т.к. без сжатия).
        0, 0, 0, 0,            // Размер изображения в байтах (можно оставить 0, если без сжатия).
        0, 0, 0, 0,            // Горизонтальное разрешение (пикселей на метр, необязательно).
        0, 0, 0, 0,            // Вертикальное разрешение (пикселей на метр, необязательно).
        0, 0, 0, 0,            // Количество цветов в палитре (0 если используются все).
        0, 0, 0, 0             // Важные цвета (0, если все важны).
    };

    *(int*)&fileHeader[2] = fileSize;     // Заполнение размера файла в заголовке.
    *(int*)&infoHeader[4] = width;        // Заполнение ширины изображения.
    *(int*)&infoHeader[8] = height;       // Заполнение высоты изображения.

    file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize); // Запись заголовка файла.
    file.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize); // Запись информационного заголовка.

    for (int y = 0; y < height; ++y) {
        file.write(reinterpret_cast<const char*>(&image[(height - y - 1) * width * 3]), width * 3); // Запись строки изображения.
        file.write(reinterpret_cast<const char*>(bmpPad), paddingAmount); // Запись заполнения.
    }

    file.close(); // Закрытие файла.
}

int main() {
    int width = 800, height = 600; // Установка ширины и высоты изображения.
    std::vector<unsigned char> image(width * height * 3, 255); // Создание вектора для хранения изображения и заполнение его белым цветом.

    Color red = {255, 0, 0};   // Определение красного цвета.
    Color blue = {0, 0, 255};  // Определение синего цвета.
    Color green = {0, 255, 0}; // Определение зеленого цвета 

    drawSquare(image, width, 100, 100, 100, blue); // Рисование красного квадрата.
    drawTriangle(image, width, 400, 300, 150, blue); // Рисование синего треугольника.
    drawTriangle(image, width, 100, 300, 100, blue); // Рисование зеленого треугольника.

    saveBMP("output.bmp", image, width, height); // Сохранение изображения в файл.

    return 0;
}

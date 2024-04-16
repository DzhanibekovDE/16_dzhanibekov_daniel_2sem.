#include <iostream>
#include <fstream>
#include <vector>

struct Color {
    unsigned char r, g, b;
};

void setPixel(std::vector<unsigned char>& image, int x, int y, int width, Color color) {
    int index = (x + y * width) * 3;
    image[index] = color.b;
    image[index + 1] = color.g;
    image[index + 2] = color.r;
}

void drawSquare(std::vector<unsigned char>& image, int width, int x, int y, int size, Color color) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            setPixel(image, x + i, y + j, width, color);
        }
    }
}

void drawTriangle(std::vector<unsigned char>& image, int width, int x, int y, int size, Color color) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j <= i; ++j) {
            setPixel(image, x + j, y + i, width, color);
        }
    }
}

void saveBMP(const char* filename, const std::vector<unsigned char>& image, int width, int height) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    if (!file) {
        std::cout << "Cannot open file!" << std::endl;
        return;
    }

    unsigned char bmpPad[3] = {0, 0, 0};
    const int paddingAmount = ((4 - (width * 3) % 4) % 4);

    const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;
    const int fileSize = fileHeaderSize + infoHeaderSize + width * height * 3 + paddingAmount * height;

    unsigned char fileHeader[fileHeaderSize] = {
        'B', 'M',              // Signature
        0, 0, 0, 0,            // image file size in bytes
        0, 0, 0, 0,            // reserved
        54, 0, 0, 0            // start of pixel array
    };

    unsigned char infoHeader[infoHeaderSize] = {
        40, 0, 0, 0,           // info header size
        0, 0, 0, 0,            // image width
        0, 0, 0, 0,            // image height
        1, 0,                  // number of color planes
        24, 0,                 // bits per pixel
        0, 0, 0, 0,            // compression
        0, 0, 0, 0,            // image size
        0, 0, 0, 0,            // horizontal resolution
        0, 0, 0, 0,            // vertical resolution
        0, 0, 0, 0,            // colors in color table
        0, 0, 0, 0             // important color count
    };

    *(int*)&fileHeader[2] = fileSize;
    *(int*)&infoHeader[4] = width;
    *(int*)&infoHeader[8] = height;

    file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    file.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

    for (int y = 0; y < height; ++y) {
        file.write(reinterpret_cast<const char*>(&image[(height - y - 1) * width * 3]), width * 3);
        file.write(reinterpret_cast<const char*>(bmpPad), paddingAmount);
    }

    file.close();
}

int main() {
    int width = 800, height = 600;
    std::vector<unsigned char> image(width * height * 3, 255);

    Color red = {255, 0, 0};
    Color blue = {0, 0, 255};
    Color green = {0, 255};

    drawSquare(image, width, 100, 100, 100, red);
    drawTriangle(image, width, 400, 300, 150, blue);
    drawTriangle(image, width, 100, 300, 100, green);

    saveBMP("output.bmp", image, width, height);

    return 0;
}

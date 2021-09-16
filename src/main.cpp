extern "C" {
    #include "pngFile.h"
    #include "png.h"
}
#include <iostream>

int main(int, char**) {
    std::cout << "Hello, world!\n";

    if (readPngFile("..//images//a.png")) {
        printf("read file success!\n");
    } else {
        printf("read file failed!\n");
    }
}

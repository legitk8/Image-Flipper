/*
    main file
*/
#include <iostream>
#include "./header/image.hpp"

int main()
{
    Image test;
    test.readFile("./img/testImage.bmp");
    test.flipImage();
    return 0;
}

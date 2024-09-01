/*
*/
#include <iostream>
#include <fstream>
#include "./header/image.hpp"

//struct definitions
Color::Color()
: r(0), g(0), b(0)
{
}

Color::Color(float r, float g, float b)
: r(r), g(g), b(b)
{
}


// class definitions
Image::Image()
{
}

// Image::Image(int imgWidth, int imgHeight)
// : imgWidth(imgWidth), imgHeight(imgHeight), imgColor(std::vector<Color>(imgWidth * imgHeight))
// {
// }

void Image::readFile(const char* filename)
{
    std::ifstream f;
    f.open(filename, std::ios::in | std::ios::binary);

    //check if the file is open
    if(!f.is_open())
    {
        std::cout << "Couldn't open the file\n";
        return;
    }

    f.read((char*)(fileHeader), sizeof(fileHeader));
    f.read((char*)(infoHeader), sizeof(infoHeader));

    //first 2 bytes of .bmp should be BM
    if(fileHeader[0] != 'B' || fileHeader[1] != 'M')
    {
        std::cout << "Not a bitmap file\n";
        f.close();
        return;
    }

    //from wikipidea (offset of the width and height)
    imgWidth = infoHeader[4] + (infoHeader[5] << 8) + (infoHeader[6] << 16) + (infoHeader[7] << 24);
    imgHeight = infoHeader[8] + (infoHeader[9] << 8) + (infoHeader[10] << 16) + (infoHeader[11] << 24);

    //resizing the color vector
    imgColor.resize(imgWidth * imgHeight);

    //each row should have bytes divisible by 4 so padding might be added
    //width*3 because r, g, b for each pixel
    const int paddingAmount = (4 - (imgWidth * 3 % 4)) % 4;

    for(int i = 0; i < imgHeight; i++)
    {
        for(int j = 0; j < imgWidth; j++)
        {
            unsigned char color[3];
            f.read((char*)(color), sizeof(color));

            imgColor[imgWidth * i + j].r = (float)color[0] / 255.0f;
            imgColor[imgWidth * i + j].g = (float)color[1] / 255.0f;
            imgColor[imgWidth * i + j].b = (float)color[2] / 255.0f;
        }
        //ignores the padding amount at the end of each row
        f.ignore(paddingAmount);
    }
    f.close();

    std::cout << "File has been read successfully\n";
}

void Image::flipImage()
{
    const int paddingAmount = (4 - (imgWidth * 3 % 4)) % 4;
    int padding[] = {0, 0, 0};

    std::ofstream f;
    f.open("newImage.bmp", std::ios::out | std::ios::binary);

    f.write((char*)(fileHeader), sizeof(fileHeader));
    f.write((char*)(infoHeader), sizeof(infoHeader));

    for(int i = imgHeight-1; i >= 0; i--)
    {
        for(int j = 0; j < imgWidth; j++)
        {
            unsigned char color[3];
            color[0] = (char)(imgColor[imgWidth * i + j].r * 255.0f);
            color[1] = (char)(imgColor[imgWidth * i + j].g * 255.0f);
            color[2] = (char)(imgColor[imgWidth * i + j].b * 255.0f);

            f.write((char*)(color), sizeof(color));
        }
        f.write((char*)(padding), paddingAmount);
    }
    f.close();

    std::cout << "Image flip successful\n";
}

/*
*/
#include <vector>
#include <algorithm>

struct Color
{
    float r, g, b;
    Color();
    Color(float, float, float);
};

class Image
{
private:
    int imgWidth, imgHeight;
    std::vector<Color> imgColor;
    unsigned char fileHeader[14], infoHeader[40];
public:
    Image();
    // Image(int, int);
    void readFile(const char*);
    void flipImage();
};

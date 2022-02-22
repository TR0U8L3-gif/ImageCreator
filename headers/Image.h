#include <cstdio>
#include <cstring>
#include <stdint.h>

enum ImageType
{
    PNG, JPG, BMP, TGA
};

struct Image
{
    uint8_t* data = NULL;
    size_t size = 0;
    int width;
    int height;
    int channels;

    Image(const char* );
    Image(int , int , int );
    Image(const Image& );
    ~Image();
    bool read(const char* );
    bool write(const char* );
    ImageType getFileType(const char* );

    Image& grayScaleAVG();
    Image& grayScaleLUM();
    void textConvert(const char*);
};


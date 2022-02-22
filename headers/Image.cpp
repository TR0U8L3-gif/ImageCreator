#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../headers/stb_image.h"
#include "../headers/stb_image_write.h"
#include "../headers/Image.h"
#include <iostream>
#include <fstream>

using namespace std;

Image::Image(const char* filename)
{

    char path[100] = "img\\";
    strcat(path,filename);
    printf("Read %s\n", path);
    if(read(path)){
        printf("File %s opend\n", filename);
        size = width * height * channels;
    }
    else{
        printf("Fail to read file %s\n", filename);
    }
}

Image::Image(int width, int height, int channels) : width(width), height(height), channels(channels)
{
    size = width * height * channels;
    data = new uint8_t[size];
}
Image::Image(const Image& img) : Image(img.width, img.height, img.channels)
{
    memcpy(data, img.data, size);
}

Image::~Image(){
    stbi_image_free(data);
}

bool Image::read(const char* filename)
{
    data = stbi_load(filename, &width, &height, &channels, 0);
    if(data) 
    return data;
}

bool Image::write(const char* filename)
{
    char path[100] = "img\\";
    strcat(path,filename);
    printf("Save %s\n", path);
    ImageType type = getFileType(filename);
    int success;
    switch (type)
    {
    case PNG:
        success = stbi_write_png(path, width, height, channels, data, width*channels);
        break;

    case JPG:
        success = stbi_write_jpg(path, width, height, channels, data, 100);
        break;

    case BMP:
        success = stbi_write_bmp(path, width, height, channels, data);
        break;

    case TGA:
        success = stbi_write_tga(path, width, height, channels, data);
        break;

    default:
        success = stbi_write_png(path, width, height, channels, data, width*channels);
        break;
    }
    if(success!=0) return true;
    else return false;
}

ImageType Image::getFileType(const char* filename)
{
    const char* ext = strrchr(filename, '.');
    if(ext != nullptr)
    {
        if(strcmp(ext, ".png") == 0){
            return PNG;
        }
        else if(strcmp(ext, ".jpg") == 0){
            return JPG;
        }
        else if(strcmp(ext, ".bmp") == 0){
            return BMP;
        }
        else if(strcmp(ext, ".tga") == 0){
            return TGA;
        }
        else
        {
            return PNG;
        }
    }
}

Image& Image::grayScaleAVG()
{
  if(channels < 3)
  {
      printf("Error :(\n");
  }
  else
  {
      for( int i = 0; i < size; i+=channels)
      {
          int gray = (data[i] + data[i+1] + data[i+2])/3;
          memset(data+i, gray, 3);
      }
  }
  return *this;
}

Image& Image::grayScaleLUM()
{
if(channels < 3)
  {
      printf("Error :(\n");
  }
  else
  {
      for( int i = 0; i < size; i+=channels)
      {
          float gray = 0.2126*data[i] + 0.7152*data[i+1] + 0.0722*data[i+2];
          memset(data+i, gray, 3);
      }
  }
  return *this;
}

void Image::textConvert(const char* filename)
{
    if(channels < 3)
    {
        printf("Error :(\n");
    }
    else
    {
        int palet[256] = {0};
        for( int i = 0; i < size; i+=channels)
        {
            int gray = (data[i] + data[i+1] + data[i+2])/3;
            palet[gray]++;
        }

        for (int i = 0; i < 256; i++)
        {
            if(palet[i]!=0)
                printf("color: %d how_much: %d\n", i, palet[i]);
        }
        
        int mini = 0;
        int maxi = 0;

        for(int i = 0; i<256; i++){
            //printf("mini:: color: %d how_much: %d\n", i, palet[i]);
            if(palet[i]!=0){
                mini = i;
                break;
            }
        }
        for(int i = 255; i>=0; i--){
            //printf("maxi:: color: %d how_much: %d\n", i, palet[i]);
            if(palet[i]!= 0){
                maxi = i;
                break;
            }
        }

        char dictionary[] = {'.', '.', ',', '-', ':', '+', '#', '&'};
        int dictionary_size = sizeof(dictionary);
        float limit_size = (maxi - mini) / (dictionary_size - 1); 
        float limits[dictionary_size];

        for (int i = 0; i < dictionary_size; i++)
        {
            limits[i] = mini + limit_size * i;
            printf("limit[%i]: %lf\n", i, limits[i]);
        }
        ofstream file(filename);
        string line = "";
        for( int i = 0; i < size; i+=channels)
        {
            int gray = (data[i] + data[i+1] + data[i+2])/3;

            if(i%width == 0){
                line += " </br>\n";
                file << line;
                line = "";
            }
            for (int i = dictionary_size-1; i >= 0; i--)
            {
                if(gray >= (int)limits[i]){
                    //printf("i: %d gray: %d limit: %lf\n", i, gray, limits[i]);
                    line += dictionary[dictionary_size - 1 - i];
                    break;
                }
            }
        }
        file.close();
    }
}


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "../headers/stb_image.h"
#include "../headers/stb_image_write.h"
#include "../headers/stb_image_resize.h"
#include "../headers/Image.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Image::Image(const char* filename)
{
    if(read(filename)){
        printf("File %s opend\n", filename);
        printf("width: %d\n", width);
        printf("height: %d\n", height);
        printf("channels: %d\n", channels);
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

Image::~Image()
{
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
    ImageType type = getFileType(filename);
    int success;
    switch (type)
    {
    case PNG:
        success = stbi_write_png(filename, width, height, channels, data, width*channels);
        break;

    case JPG:
        success = stbi_write_jpg(filename, width, height, channels, data, 100);
        break;

    case BMP:
        success = stbi_write_bmp(filename, width, height, channels, data);
        break;

    case TGA:
        success = stbi_write_tga(filename, width, height, channels, data);
        break;

    default:
        success = stbi_write_png(filename, width, height, channels, data, width*channels);
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

Image& Image::resize(const char* filename)
{
    if(channels < 3)
    {
        printf("Error :(\n");
    }
    else
    {
        int w =  width;
        int h = height;
        printf("w: %d h: %d\n", w, h);
        while (w > 250 && h > 250)     
        {
            w /= 2;
            h /= 2;
        }
        printf("w: %d h: %d\n", w, h);

        auto *out = (unsigned char *) malloc(channels * w * h );
        // stbir_resize_uint8( data , width , height , 0, out, w, h, 0, channels);
        stbir_resize(data, width, height, 0, out, w, h, 0, STBIR_TYPE_UINT8, channels, STBIR_ALPHA_CHANNEL_NONE, 0,
                 STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
                 STBIR_FILTER_BOX, STBIR_FILTER_BOX,
                 STBIR_COLORSPACE_SRGB, nullptr
        );
        stbi_write_png(filename, w, h, channels, out, 0);
        stbi_image_free(out);
    }
    return *this;
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

        // for (int i = 0; i < 256; i++)
        // {
        //     if(palet[i]!=0)
        //         printf("color: %d how_much: %d\n", i, palet[i]);
        // }
        
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

        char dictionary[] = {' ', '`', '.', ',', '-', '~', '^', '*', ':', '!', '+', '&', '%', '#', '@'};
        int dictionary_size = sizeof(dictionary);
        float limit_size = (maxi - mini) / (dictionary_size - 1); 
        float limits[dictionary_size];

        for (int i = 0; i < dictionary_size; i++)
        {
            limits[i] = mini + limit_size * i;
            //printf("limit[%i]: %lf\n", i, limits[i]);
        }
        ofstream file(filename);
        string text = "";
        string line = "";
        cout << "size: " << size << endl;
        int ile = 0;
        for( int i = 0; i < size; i+=channels)
        {
            int gray = (data[i] + data[i+1] + data[i+2])/3;

            if(i%(width * channels) == 0){
                ile++;
                cout << i << endl;
                line += " </br>\n";
                file << line;
                text += line;
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
        cout << "ile: " << ile << endl;
        file.close();

        //html file
        string before = "";
        string after = "\n";
        line = "";
        fstream html("website/example.html");
        bool change = false;
        while (getline(html,line)) {
            if(line == "<!--insert-->")
            {
                change = true;
            }
            line += "\n";
            if(change){
                after += line;
            }
            else{
                before += line;
            }
        } 
        before += "\n";
        html.close();
        ofstream pic("website/picture.html");
        pic << before << text << after;
        pic.close();
    }
}


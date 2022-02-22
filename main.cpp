#include <iostream>
#include <string>
#include "headers/Image.h"

using namespace std;

int main (int argc, char** argv){

    Image test("test.jpg");

    Image gray_avg = test;
    gray_avg.grayScaleAVG();
    gray_avg.write("gray_avg.png");
    gray_avg.textConvert("website/test.txt");

    // Image gray_lum = test;
    // gray_lum.grayScaleLUM();
    // gray_lum.write("gray_lum.png");
    // gray_lum.textConvert("gray_lum.txt");

    return 0;
}

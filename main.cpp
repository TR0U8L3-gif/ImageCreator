#include <iostream>
#include <string>
#include "headers/Image.h"

using namespace std;

int main (int argc, char** argv){

    Image test("img/test.jpg");
    cout << "start" << endl;
    Image resize = test;
    resize.resize("test_resize.png");
    Image gray("test_resize.png");
    gray.textConvert("website/test.txt");
    gray.grayScaleAVG();
    gray.write("test_gray.png");
    

    return 0;
}

#include <iostream>

struct bracket {
    int order;
    float shutter;
    float fstop;
    float filter;
    float * image;
    float ev;
    int width;
    int height;
    int channels;
    std::string file_name;
};


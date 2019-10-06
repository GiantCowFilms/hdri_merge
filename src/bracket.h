#pragma once
#ifndef BRACKET_H
    #define BRACKET_H
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
#endif


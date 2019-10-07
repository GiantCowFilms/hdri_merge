#include <iostream>
#include<algorithm> 
#include "bracket.h"
using namespace std;
float weightFromPixel(float pixel) {
    float clip = 0.05;
    float weight =  1.0 - abs(0.5 - ((pixel - clip) * (2.0 - clip * 2.0))) * 2.0;
    return max(0.0f,weight);
}

void initalize_brackets(bracket ** brackets, int brackets_length) {
    for (int b= 0; b < brackets_length; b++) {
        brackets[b]->ev = log( ( pow(brackets[b]->fstop,2) /brackets[b]->shutter) ) / log(2.0) + brackets[b]->filter;
    }
}

float * merge_brackets(bracket ** brackets, int brackets_length) {
    // Init
    initalize_brackets(brackets,brackets_length);
    float base_ev = brackets[0]->ev;

    //Prepare writing buffers
    int pixels_length = brackets[0]->width * brackets[0]->height * brackets[0]->channels;
    float * pixels = new float[
        pixels_length
    ] { 0.0 };
    float * weights = new float[
        pixels_length
    ] { 0.0 };

    //Do merge
    for (int b= 0; b < brackets_length; b++) {
        for (int p = 0; p < pixels_length; p++) {
            if ((p + 1) % 4 == 0) {
                weights[p] = 1.0;
            } else {
                float weight = weightFromPixel(brackets[b]->image[p]);
                weights[p] += weight;
            }
        }
    }
    for (int b= 0; b < brackets_length; b++) {
        float ev_offset = brackets[b]->ev - base_ev;
        cout << "EV offset: " << ev_offset << endl;
        for (int p = 0; p < pixels_length; p++) {
            float weight = weightFromPixel(brackets[b]->image[p]);
            if ((p + 1) % 4 == 0) {
                pixels[p] = 1.0;
            } else {
                pixels[p] += brackets[b]->image[p] * powf(2.0,ev_offset) * (weight / weights[p]);
            }
        }
    }
    // either pixels or weights should be freed
    delete [] weights;
    return pixels;
}
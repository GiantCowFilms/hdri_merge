#include <iostream>
#include<algorithm> 
#include "bracket.h"
#include "merge_alg.h"
using namespace std;

/**
 * Generates an image made of slices of the brackets all with matched exposure.
 * 
 * This is useful for verifying that the exposure metadata is correct 
 * 
 */
float * debug_exp(bracket ** brackets, int brackets_length) {
    // Init
    initalize_brackets(brackets,brackets_length);
    float base_ev = brackets[0]->ev;
    int slice_size = brackets[0]->width / (brackets_length - 1);
    //Prepare writing buffers
    int pixels_length = brackets[0]->width * brackets[0]->height * brackets[0]->channels;
    float * pixels = new float[
        pixels_length
    ] { 0.0 };

    //Do merge
    for (int p = 0; p < pixels_length; p++) {
        int x = p % brackets[0]->width;
        float ev_offset = brackets[x/slice_size]->ev - base_ev;
        pixels[p] = brackets[x/slice_size]->image[p] * pow(2.0f,ev_offset);
    }
    return pixels;
}
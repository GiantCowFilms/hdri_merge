#include <iostream>
#include <fstream>
#include <OpenImageIO/imageio.h>
#include "bracket.h"

using namespace std;
using namespace OIIO;

bracket ** readConfig(string config_loc, size_t* config_length) {
    fstream config_file;
    config_file.open(config_loc);
    *config_length = 7;
    bracket ** brackets = new bracket*[*config_length];
    bracket *cur_bracket;
    int order = 0;
    for(std::string line; getline(config_file, line); ) {
        if (line.rfind("bracket:") == 0) {
            brackets[order] = new bracket();
            // Extend brackets by 7
            if (order >= *config_length - 1) {
                *config_length += 7;
                bracket ** new_brackets = new bracket*[*config_length];
                free(brackets);
                brackets = new_brackets;
            }
            cur_bracket = brackets[order];
            cur_bracket->order = order++;
        } else if (line.rfind("shutter=",0) == 0) {
            cur_bracket->shutter = std::strtof(line.substr(8).c_str(),NULL);
        } else if (line.rfind("filter=",0) == 0) {
            cur_bracket->filter = std::strtof(line.substr(7).c_str(),NULL);
        } else if (line.rfind("apature=",0) == 0) {
            cur_bracket->fstop = std::strtof(line.substr(8).c_str(),NULL);
        } else if (line.rfind("fn=",0) == 0) {
            cur_bracket->file_name = line.substr(3);
        }
    }
    *config_length = order;
    return brackets;
}

int main(int argc, char *argv[]) {
    cout << "HDRI Merge." << endl;
    size_t brackets_len;
    auto input = readConfig(argv[1], &brackets_len);
    ImageSpec &out_spec = ImageSpec();
    for (int i = 0; i < brackets_len; i++) {
        const char * safe_filename = input[i]->file_name.c_str();
        auto in = ImageInput::open(safe_filename);
        if (!in) {
            cerr << OpenImageIO_v2_1::geterror();
            //cerr << "Could not read image: " << input[i]->file_name << endl;
            return 1;
        }
        const ImageSpec &spec = in->spec();
        if (i == 0) {
            out_spec.copy_dimensions(spec);
        }
        float *image = (float*)malloc(spec.width * spec.height * spec.nchannels * sizeof(float));
        if(!in->read_image(TypeDesc::FLOAT,image)) {
            cerr << "Could not read pixels from: " << input[i]->file_name << endl;
            cerr << in->geterror();
            return 1;
        }
        input[i]->image = image;
        input[i]->width = spec.width;
        input[i]->height = spec.height;
        input[i]->channels = spec.nchannels;
        in->close();
    }
    auto out = ImageOutput::create(argv[2]);
    if (!out) {
        cout << "Cannot write to image: " << argv[2];
        return 1;
    }
    out_spec.set_format(TypeDesc::FLOAT);
    out_spec.channelnames = std::vector<string> {
        "R",
        "G",
        "B",
        "A"
    };
    out->open(argv[2],out_spec);
    if(!out->write_image(TypeDesc::FLOAT, input[0]->image)) {
        cerr << "Could not write pixels to: " << argv[2] << endl;
        cerr << out->geterror();
    }
    out->close();
    return 0;
}
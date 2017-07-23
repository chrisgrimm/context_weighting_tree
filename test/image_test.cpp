#include <iostream>
#include <tiff.h>
#include <vector>
#include <cmath>
#include <tuple>
#include "../cwt.h"
#include <chrono>
#include <sys/time.h>

using namespace std;
using namespace std::chrono;


uint8 get_pixel(uint8 *image, int x, int y, int w, int h) {
    if (x < w && x >= 0 && y < h && y >= 0) {
        return image[w*y + x];
    } else {
        return 0;
    }
}

tuple<double, double> feed_bits_to_tree(cwt *tree, uint8 inp_pixel, int inp_bit_num, int num_bits, uint8 *context, int len_context) {
    // build flat context array.
    uint8 context_flat[len_context*num_bits];
    int context_flat_pos = 0;
    for (int i=0; i<len_context; i++) {
        uint8 pixel = context[i];
        for (int c=0; c<num_bits; c++) {
            uint8 bit = (pixel >> c) & (uint8) 1;
            context_flat[context_flat_pos++] = bit;
        }
    }
    uint8 inp_bit = (inp_pixel >> inp_bit_num) & (uint8) 1;
    double bit_prob = exp(tree->update_and_logprob(context_flat, inp_bit));
    double recoding_prob = exp(tree->logprob(context_flat, inp_bit));
    return make_tuple(bit_prob, recoding_prob);
}

double process_image_with_cwt(vector<vector<vector<cwt *>>> cwt_array, uint8 *image, int w, int h, int num_bits) {
    double cum_prob = 1.0;
    double cum_recoding_prob = 1.0;
    for (int x=0; x<w; x++) {
        for (int y=0; y<h; y++) {
            for (int c=0; c<num_bits; c++) {
                cwt *tree = cwt_array[x][y][c];
                uint8 pixel_value = get_pixel(image, x, y, w, h);
                uint8 context[4];
                context[0] = get_pixel(image, x-1, y, w, h);
                context[1] = get_pixel(image, x-1, y-1, w, h);
                context[2] = get_pixel(image, x, y-1, w, h);
                context[3] = get_pixel(image, x+1, y-1, w, h);
                auto prob_pair = feed_bits_to_tree(tree, pixel_value, c, 3, context, 4);
                double bit_prob = std::get<0>(prob_pair);
                double recoding_prob = std::get<1>(prob_pair);
                cum_prob *= bit_prob;
                cum_recoding_prob *= recoding_prob;
            }
        }
    }
    double pseudocount = cum_prob*(1 - cum_recoding_prob)/(cum_recoding_prob - cum_prob);
    return pseudocount;
}

vector<vector<vector<cwt *>>> build_cwt_array(int w, int h, int num_bits) {
    vector<vector<vector<cwt *>>> outer_vector;
    for (int x=0; x<w; x++) {
        vector<vector<cwt *>> x_vector;
        for (int y=0; y<h; y++) {
            vector<cwt *> y_vector;
            for (int c=0; c<num_bits; c++) {
                cwt *tree = new cwt(4*num_bits);
                y_vector.push_back(tree);
            }
            x_vector.push_back(y_vector);
        }
        outer_vector.push_back(x_vector);
    }
    return outer_vector;
}

void destroy_cwt_array(vector<vector<vector<cwt *>>> cwt_array, int w, int h, int num_bits) {
    for (int x=0; x<w; x++) {
        for (int y=0; y<h; y++) {
            for (int c=0; c<num_bits; c++) {
                delete cwt_array[x][y][c];
            }
        }
    }
}




int main() {
    cout << "hi!" << "\n";
    int w = 32;
    int h = 32;
    int num_bits = 3;
    auto cwt_array = build_cwt_array(w, h, num_bits);
    uint8 image[w*h];
    for (int i=0; i< w*h; i++) {
        image[i] = 0;
    }
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    for (int x=0; x<1000; x++) {
        double image_pseudocount = process_image_with_cwt(cwt_array, image, w, h, num_bits);
        cout << image_pseudocount << "\n";
    }
    gettimeofday(&tp, NULL);
    long int ms2 = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    cout << (ms2 - ms)/1000.0 << "\n";

    destroy_cwt_array(cwt_array, w, h, num_bits);
}
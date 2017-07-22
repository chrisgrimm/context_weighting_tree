#include <iostream>
#include <tiff.h>
#include <vector>
#include <cmath>
#include "cwt.h"

using namespace std;


uint8 get_pixel(uint8 *image, int x, int y, int w, int h) {
    if (x < w && x >= 0 && y < h && y >= 0) {
        return image[h*y + w];
    } else {
        return 0;
    }
}

double feed_bits_to_tree(cwt *tree, uint8 inp_pixel, int num_bits, uint8 *context, int len_context) {
    // build flat context array.
    uint8 context_flat[len_context*num_bits];
    uint8 context_flat2[len_context*num_bits];
    int context_flat_pos = 0;
    for (int i=0; i<len_context; i++) {
        uint8 pixel = context[i];
        for (int c=0; c<num_bits; c++) {
            uint8 bit = (pixel >> c) & (uint8) 1;
            if (c < num_bits-1)
                context_flat2[context_flat_pos+1] = bit;
            context_flat[context_flat_pos++] = bit;
        }
    }
    double cumulative_pseudocounts = 1.0;
    for (int inp_bit_num=0; inp_bit_num<num_bits; inp_bit_num++) {
        uint8 inp_bit = (inp_pixel >> inp_bit_num) & (uint8) 1;
        context_flat2[0] = inp_bit;
        double bit_prob = exp(tree->update_and_logprob(context_flat, inp_bit));
        double recoding_prob = exp(tree->logprob(context_flat2, inp_bit));
        double pseudocount = (bit_prob)*(1-recoding_prob) / (recoding_prob - bit_prob);
        cumulative_pseudocounts *= pseudocount;
    }
    return cumulative_pseudocounts;
}

double process_image_with_cwt(vector<vector<vector<cwt *>>> cwt_array, uint8 *image, int w, int h, int num_bits) {
    double total_pseudocount = 1.0;
    for (int x=0; x<w; x++) {
        for (int y=0; y<h; y++) {
            for (int c=0; c<num_bits; c++) {
                cwt *tree = cwt_array[w][h][c];
                uint8 pixel_value = get_pixel(image, x, y, w, h);
                uint8 context[4];
                context[0] = get_pixel(image, x-1, y, w, h);
                context[1] = get_pixel(image, x-1, y-1, w, h);
                context[2] = get_pixel(image, x, y-1, w, h);
                context[3] = get_pixel(image, x+1, y-1, w, h);
                double pseudocount = feed_bits_to_tree(tree, pixel_value, 3, context, 4);
                total_pseudocount *= pseudocount;
            }
        }
    }
    return total_pseudocount;
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



int main() {
    int w = 5;
    int h = 5;
    int num_bits = 3;
    vector<vector<vector<cwt * >>> cwt_array = build_cwt_array(w, h, num_bits);
    uint8 *image;
    double image_pseudocount = process_image_with_cwt(cwt_array, image, w, h, num_bits);
}
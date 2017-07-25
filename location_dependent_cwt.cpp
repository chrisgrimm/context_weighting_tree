//
// Created by maroderi on 7/23/17.
//

#include <tiff.h>
#include "location_dependent_cwt.h"

#include <cmath>
#include <tuple>

using namespace std;

location_dependent_cwt::location_dependent_cwt(int w, int h, int num_bits) {
    m_cwt_array = build_cwt_array(w, h, num_bits);
    m_width = w;
    m_height = h;
    m_num_bits = num_bits;
}
location_dependent_cwt::~location_dependent_cwt() {
    destroy_cwt_array(m_cwt_array, m_width, m_height, m_num_bits);
}

uint8 location_dependent_cwt::get_pixel(uint8 *image, int x, int y, int w, int h) {
    if (x < w && x >= 0 && y < h && y >= 0) {
        return image[w*y + x];
    } else {
        return 0;
    }
}

tuple<double, double> location_dependent_cwt::feed_bits_to_tree(cwt *tree, uint8 inp_pixel, int inp_bit_num, int num_bits, uint8 *context, int len_context) {
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
    double bit_prob = tree->update_and_logprob(context_flat, inp_bit);
    double recoding_prob = tree->logprob(context_flat, inp_bit);
    return make_tuple(bit_prob, recoding_prob);
}

double location_dependent_cwt::process_image(uint8 *image) {
    int w = m_width, h = m_height, num_bits = m_num_bits;

    double cum_prob = 0.0;
    double cum_recoding_prob = 0.0;
    for (int x=0; x<w; x++) {
        for (int y=0; y<h; y++) {
            for (int c=0; c<num_bits; c++) {
                cwt *tree = m_cwt_array[x][y][c];
                uint8 pixel_value = get_pixel(image, x, y, w, h);
                uint8 context[4];
                context[0] = get_pixel(image, x-1, y, w, h);
                context[1] = get_pixel(image, x-1, y-1, w, h);
                context[2] = get_pixel(image, x, y-1, w, h);
                context[3] = get_pixel(image, x+1, y-1, w, h);
                auto prob_pair = feed_bits_to_tree(tree, pixel_value, c, 3, context, 4);
                double bit_prob = std::get<0>(prob_pair);
                double recoding_prob = std::get<1>(prob_pair);
                cum_prob += bit_prob;
                cum_recoding_prob += recoding_prob;
            }
        }
    }
    cum_prob = exp(cum_prob);
    cum_recoding_prob = exp(cum_recoding_prob);
    if (cum_prob >= cum_recoding_prob) {
        return 0;
    } else {
        double pseudocount = cum_prob*(1 - cum_recoding_prob)/(cum_recoding_prob - cum_prob);
        return pseudocount;
    }
}

vector<vector<vector<cwt *>>> location_dependent_cwt::build_cwt_array(int w, int h, int num_bits) {
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

void location_dependent_cwt::destroy_cwt_array(vector<vector<vector<cwt *>>> cwt_array, int w, int h, int num_bits) {
    for (int x=0; x<w; x++) {
        for (int y=0; y<h; y++) {
            for (int c=0; c<num_bits; c++) {
                delete cwt_array[x][y][c];
            }
        }
    }
}
//
// Created by maroderi on 7/23/17.
//

#ifndef CONTEXT_WEIGHTING_TREE_LOCATION_DEPENDENT_CWT_H
#define CONTEXT_WEIGHTING_TREE_LOCATION_DEPENDENT_CWT_H

#include <iostream>
#include <vector>
#include "cwt.h"

using namespace std;

class location_dependent_cwt {
public:
    location_dependent_cwt(int w, int h, int num_bits);
    ~location_dependent_cwt();
    double process_image(uint8 *image);

private:
    vector<vector<vector<cwt *>>> m_cwt_array;
    int m_width, m_height, m_num_bits;
    uint8 get_pixel(uint8 *image, int x, int y, int w, int h);
    tuple<double, double> feed_bits_to_tree(cwt *tree, uint8 inp_pixel, int inp_bit_num, int num_bits, uint8 *context, int len_context);
    vector<vector<vector<cwt *>>> build_cwt_array(int w, int h, int num_bits);
    void destroy_cwt_array(vector<vector<vector<cwt *>>> cwt_array, int w, int h, int num_bits);
};


#endif //CONTEXT_WEIGHTING_TREE_LOCATION_DEPENDENT_CWT_H

//
// Created by maroderi on 7/21/17.
//

#include <tiff.h>
#include <assert.h>
#include <iostream>
#include <cmath>
#include "../cwt.h"
#include <string>
using namespace std;

void display_tree_ab(cwt_node *tree, string prefix) {
    cout << prefix << tree->m_old_a << " , " << tree->m_old_b << "\n";
    cout << prefix << "P_e: " << exp(tree->m_old_P) << "\n";
    cout << prefix << "P_w: " << exp(tree->m_new_weighted_P) << "\n";

    if (tree->m_left == 0 & tree->m_right == 0) {
        return;
    } else {
        display_tree_ab(tree->m_left, "\t"+prefix);
        display_tree_ab(tree->m_right, "\t"+prefix);
    }
}

int main() {
    uint8 h1[] = {0, 1, 0};
    uint8 h2[] = {0, 0, 1};
    uint8 h3[] = {1, 0, 0};
    uint8 h4[] = {1, 1, 0};
    uint8 h5[] = {0, 1, 1};
    uint8 h6[] = {1, 0, 1};
    uint8 h7[] = {0, 1, 0};
    uint8 *hh[] = {h1, h2, h3, h4, h5, h6, h7};
    uint8 bit[] = {0, 1, 1, 0, 1, 0, 0};

    int context_length = 3;
    int history_length = context_length + 7;

    cwt *tree = new cwt(context_length);


    double logprob = 0;

    for (int i=0; i<7; i++) {
        logprob = tree->update_and_logprob(hh[i], bit[i]);
        std::cout << exp(logprob) << "\n";
    }

    /*for (int i = context_length; i < history_length; ++i) {
        logprob = tree->update_and_logprob(h + i - context_length, h[i]);
        std::cout << context_length <<
        std::cout << exp(logprob) << "\n";
    }*/

    display_tree_ab(tree->m_root_node, "");


    std::cout << exp(logprob);

//    for (int i = context_length; i < history_length; ++i) {
//        assert(tree->logprob(h + 3 - context_length, h[i]) - 1 < 0.000000001);
//    }
}
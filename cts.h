//
// Created by Christopher Grimm on 8/7/17.
//

#ifndef CONTEXT_WEIGHTING_TREE_CTS_H
#define CONTEXT_WEIGHTING_TREE_CTS_H


#include <tiff.h>
#include "cwt_node.h"
#include "cts_node.h"

class cts {
public:
    cts(int context_depth);
    ~cts();
    double update_and_logprob(uint8 *context, uint8 bit);
    double logprob(uint8 *context, uint8 bit);
    double logprob_block();
    cts_node *m_root_node;
    int m_context_depth;
};


#endif //CONTEXT_WEIGHTING_TREE_CTS_H

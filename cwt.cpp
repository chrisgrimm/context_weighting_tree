//
// Created by cgrimm on 7/21/17.
//

#include "cwt.h"
#include "cwt_node.h"

cwt_node *make_node_recursive(int depth_remaining) {
    if (depth_remaining == 0)
        return 0;

    cwt_node *left = make_node_recursive(depth_remaining-1);
    cwt_node *right = make_node_recursive(depth_remaining-1);

    return new cwt_node(left, right);
}

void kill_tree_recursive(cwt_node *ptr, int depth_remaining) {
    if (depth_remaining == 0)
        return;

    kill_tree_recursive(ptr->m_left, depth_remaining-1);
    kill_tree_recursive(ptr->m_right, depth_remaining-1);
    delete ptr;

}

cwt::cwt(int context_depth) {
    m_context_depth = context_depth;
    m_root_node = make_node_recursive(context_depth);
}

cwt::~cwt() {
    kill_tree_recursive(m_root_node, m_context_depth);
};

double cwt::logprob(uint8 *context, uint8 bit) {
    return m_root_node->update(context, bit, true);
}

double cwt::update_and_logprob(uint8 *context, uint8 bit) {
    return m_root_node->update(context, bit, true);
}
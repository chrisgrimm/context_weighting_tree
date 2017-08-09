//
// Created by Christopher Grimm on 8/7/17.
//

#include "cts.h"
#include "cts_node.h"


void kill_tree_recursive(cts_node *ptr) {
    if (ptr->m_left != 0)
        kill_tree_recursive(ptr->m_left);
    if (ptr->m_right != 0)
        kill_tree_recursive(ptr->m_right);
    delete ptr;

}

cts::cts(int context_depth) {
    m_context_depth = context_depth;
    m_root_node = new cts_node(this, 0, 0, 0, context_depth);
    m_alpha = 1;
}

cts::~cts() {
    kill_tree_recursive(m_root_node);
};

double cts::logprob(uint8 *context, uint8 bit) {
    m_alpha += 1;
    double before = logprob_block();
    cts_node *updated_nodes[m_context_depth];
    double after = m_root_node->update(context, bit, updated_nodes);

    for (int i = 0; i < m_context_depth; ++i) {
        updated_nodes[i]->reset();
    }
    m_alpha -= 1;
    return after - before;
}

double cts::update_and_logprob(uint8 *context, uint8 bit) {
    m_alpha += 1;
    double before = logprob_block();
    cts_node *updated_nodes[m_context_depth];
    double after = m_root_node->update(context, bit, updated_nodes);

    return after - before;
}

double cts::logprob_block() {
    return m_root_node->m_weighted_P;
}
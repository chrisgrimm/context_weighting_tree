//
// Created by Mel Roderick on 8/11/17.
//

#include "cwt_symbol.h"
#include "fastmath.hpp"

void kill_tree_recursive(cwt_node_symbol *ptr) {
    for (int i = 0; i < ptr->m_alphabet_size; ++i) {
        if (ptr->m_children[i] != 0) {
            kill_tree_recursive(ptr->m_children[i]);
        }
    }
    delete ptr;

}

cwt_symbol::cwt_symbol(int context_depth, int alphabet_size) {
    m_context_depth = context_depth;
    m_root_node = new cwt_node_symbol(this, 0, context_depth, alphabet_size);
    m_log_1_minus_alpha, m_log_alpha = 0;
}

cwt_symbol::~cwt_symbol() {
    kill_tree_recursive(m_root_node);
};

double cwt_symbol::logprob(uint8 *context, uint8 symbol) {
    return m_root_node->update(context, symbol, false);
}

double cwt_symbol::update_and_logprob(uint8 *context, uint8 symbol) {
    m_n += 1.0;
    m_log_alpha = fast_log(1.0 / (m_n + 1.0));
    m_log_1_minus_alpha = fast_log(m_n / (m_n + 1.0));

    return m_root_node->update(context, symbol, true);
}
//
// Created by cgrimm on 7/21/17.
//

#include <tiff.h>
#include <cmath>
#include <iostream>
#include "cwt_node.h"
#include "fastmath.hpp"

static const bool UseFastJacobianLog = true;
#define LOG_PT_5 -0.6931471805599453094172321 // log(0.5)

cwt_node::cwt_node(cwt_node *left, cwt_node *right, int depth, int max_depth) {
    m_old_a = 0;
    m_new_a = 0;

    m_old_b = 0;
    m_new_b = 0;

    m_old_P = 0;
    m_new_P = 0;

    m_old_weighted_P = 0;
    m_new_weighted_P = 0;

    m_left = left;
    m_right = right;

    m_depth = depth;
    m_max_depth = max_depth;
}

double cwt_node::update(uint8 *context, uint8 bit, cwt_node **updated_nodes) {

    m_old_a = m_new_a;
    m_old_b = m_new_b;
    m_old_P = m_new_P;
    m_old_weighted_P = m_new_weighted_P;

    updated_nodes[m_depth] = this;
    double result;

    if (bit == 1) m_new_b = m_new_b + 1;
    else m_new_a = m_new_a + 1;

    if (m_depth == m_max_depth) {
        result = update_P((bit == 0));
        return result;
    }
    if (*context == 1) {
        if (m_left == 0) {
            m_left = new cwt_node(0, 0, m_depth+1, m_max_depth);
        }
        m_left->update(context+1, bit, updated_nodes);
        result = update_P((bit == 0));
    } else {
        if (m_right == 0) {
            m_right = new cwt_node(0, 0, m_depth+1, m_max_depth);
        }
        m_right->update(context+1, bit, updated_nodes);
        result = update_P((bit == 0));
    }

    return result;
}

void cwt_node::reset() {
    m_new_a = m_old_a;
    m_new_b = m_old_b;
    m_new_P = m_old_P;
    m_new_weighted_P = m_old_weighted_P;
}

inline double ctsLogAdd(double log_x, double log_y) {

    if (UseFastJacobianLog) {

        if (log_x < log_y) {
            return fast_jacoblog(log_y - log_x) + log_x;
        } else {
            return fast_jacoblog(log_x - log_y) + log_y;
        }

    } else {
        if (log_x < log_y) {
            return std::log(1.0 + std::exp(log_y - log_x)) + log_x;
        } else {
            return std::log(1.0 + std::exp(log_x - log_y)) + log_y;
        }
    }
}

cwt_node::~cwt_node() {

}

double cwt_node::update_P(bool a_updated) {
    if (a_updated) {
        int a = m_new_a - 1, b = m_new_b;
        m_new_P = fast_log((a + 0.5) / (a + b + 1)) + m_new_P;
    } else {
        int a = m_new_a, b = m_new_b - 1;
        m_new_P = fast_log((b + 0.5) / (a + b + 1)) + m_new_P;
    }
    if ((m_left == 0 && m_right == 0)) {
        m_new_weighted_P = m_new_P;
    } else {
        double term1 = LOG_PT_5 + m_new_P;
        double weighted_p_left = (m_left == 0) ? 0 : m_left->m_new_weighted_P;
        double weighted_p_right = (m_right == 0) ? 0 : m_right->m_new_weighted_P;
        double term2 = LOG_PT_5 + weighted_p_left + weighted_p_right;
        m_new_weighted_P = ctsLogAdd(term1, term2);
    }
    return m_new_weighted_P;
}
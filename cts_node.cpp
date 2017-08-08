//
// Created by Christopher Grimm on 8/7/17.
//

#include "cts_node.h"

#include <tiff.h>
#include <cmath>
#include <iostream>
#include "fastmath.hpp"

static const bool UseFastJacobianLog = true;
#define LOG_PT_5 -0.6931471805599453094172321 // log(0.5)

cts_node::cts_node(cts_node *left, cts_node *right, int depth, int max_depth) {
    m_old_a = 0;
    m_a = 0;

    m_old_b = 0;
    m_b = 0;

    m_old_P = 0;
    m_P = 0;

    m_old_weighted_P = 0;
    m_weighted_P = 0;

    m_sc = LOG_PT_5;
    m_kc = LOG_PT_5;
    m_old_sc = LOG_PT_5;
    m_old_kc = LOG_PT_5;

    m_left = left;
    m_right = right;

    m_depth = depth;
    m_max_depth = max_depth;
}

double cts_node::update(uint8 *context, uint8 bit, cts_node **updated_nodes) {

    m_old_a = m_a;
    m_old_b = m_b;
    m_old_P = m_P;
    m_old_weighted_P = m_weighted_P;
    m_old_sc = m_sc;
    m_old_kc = m_kc;

    updated_nodes[m_depth] = this;
    double result;
    if (bit == 1) m_b = m_b + 1;
    else m_a = m_a + 1;
    if (m_depth == m_max_depth) {
        result = update_P((bit == 0));
    } else if (*context == 1) {
        if (m_left == 0) {
            m_left = new cts_node(0, 0, m_depth+1, m_max_depth);
        }
        m_left->update(context+1, bit, updated_nodes);
        result = update_P((bit == 0));
    } else {
        if (m_right == 0) {
            m_right = new cts_node(0, 0, m_depth+1, m_max_depth);
        }
        m_right->update(context+1, bit, updated_nodes);
        result = update_P((bit == 0));
    }

    return result;
}


void cts_node::reset() {
    m_a = m_old_a;
    m_b = m_old_b;
    m_P = m_old_P;
    m_weighted_P = m_old_weighted_P;
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

cts_node::~cts_node() {

}

double cts_node::update_P(bool a_updated) {
    if (a_updated) {
        int a = m_a - 1, b = m_b;
        m_P = fast_log((a + 0.5) / (a + b + 1)) + m_P;
    } else {
        int a = m_a, b = m_b - 1;
        m_P = fast_log((b + 0.5) / (a + b + 1)) + m_P;
    }
    if ((m_left == 0 && m_right == 0)) {
        m_weighted_P = m_P;
    } else {
        double term1 = m_kc + (m_P - m_old_P);
        double weighted_p_left = (m_left == 0) ? 0 : m_left->m_weighted_P;
        double weighted_p_right = (m_right == 0) ? 0 : m_right->m_weighted_P;
        double old_weighted_p_left = (m_left == 0) ? 0 : m_left->m_old_weighted_P;
        double old_weighted_p_right = (m_right == 0) ? 0 : m_right->m_old_weighted_P;

        double z = (weighted_p_left - old_weighted_p_left) + (weighted_p_right - old_weighted_p_right);
        double term2 = m_sc + z;
        double alpha = 1.0; // TODO set alpha
        m_kc = ctsLogAdd(log(alpha) + m_old_weighted_P, log(1 - 2*alpha) + term1);
        m_sc = ctsLogAdd(log(alpha) + m_old_weighted_P, log(1 - 2*alpha) + term2);
        m_weighted_P = ctsLogAdd(term1, term2);

    }
    return m_weighted_P;
}
//
// Created by cgrimm on 7/21/17.
//

#include <tiff.h>
#include <cmath>
#include "cwt_node.h"


cwt_node::cwt_node() {
    m_old_a = 0;
    m_new_a = 0;

    m_old_b = 0;
    m_new_b = 0;

    m_old_P = 0;
    m_new_P = log(1.0);

    m_new_weighted_P = log(1.0);

    m_left = left;
    m_right = right;
}

double cwt_node::update(uint8 *context, uint8 bit, bool non_destructive=true) {

    double result;

    if (bit == 1) m_new_b = m_old_b + 1;
    else m_new_a = m_old_a + 1;

    if (m_left == 0 && m_right == 0) {
        return update_P((bit == 0));
    }

    if (*context == 1) {
        m_left->update(context+1, bit);
        result = update_P((bit == 0));
    } else {
        m_right->update(context+1, bit);
        result = update_P((bit == 0));
    }

    if (!non_destructive) {
        m_old_a = m_new_a;
        m_old_b = m_new_b;
        m_old_P = m_new_P;
    }

    return result;
}

inline double ctsLogAdd(double log_x, double log_y) {

    /*if (UseFastJacobianLog) {

        if (log_x < log_y) {
            return fast_jacoblog(log_y - log_x) + log_x;
        } else {
            return fast_jacoblog(log_x - log_y) + log_y;
        }

    } else*/
    if (log_x < log_y) {
        return std::log(1.0 + std::exp(log_y - log_x)) + log_x;
    } else {
        return std::log(1.0 + std::exp(log_x - log_y)) + log_y;
    }
}

void cwt_node::~cwt_node() {

}

double cwt_node::update_P(bool a_updated) {
    if (a_updated) {
        int a = m_old_a - 1, b = m_old_b;
        m_new_P = log((a + 0.5) / (a + b + 1)) + m_old_P;
    } else {
        int a = m_old_a, b = m_old_b - 1;
        m_new_P = log((b + 0.5) / (a + b + 1)) + m_old_P;
    }
    if ((m_left == 0 && m_right == 0)) {
        m_new_weighted_P = m_new_P;
    } else {
        double term1 = m_logpt5 + m_new_P;
        double term2 = m_logpt5 + m_left->m_new_weighted_P + m_right->m_new_weighted_P;
        m_new_P = ctsLogAdd(term1, term2);
    }
    return m_new_P;
}
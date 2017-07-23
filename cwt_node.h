//
// Created by cgrimm on 7/21/17.
//

#ifndef CWT_CWT_NODE_H
#define CWT_CWT_NODE_H

class cwt_node {
public:
    cwt_node(cwt_node *left, cwt_node *right, int depth, int max_depth);
    ~cwt_node();
    double update_P(bool a_updated);
    double update(uint8 *context, uint8 bit, bool non_destructive=true);
    cwt_node* m_left = 0;
    cwt_node* m_right = 0;
    int m_old_a, m_new_a, m_old_b, m_new_b;
    double m_old_P, m_new_P, m_old_weighted_P, m_new_weighted_P;
    int m_depth;
    int m_max_depth;
};


#endif //CWT_CWT_NODE_H

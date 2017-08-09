//
// Created by maroderi on 7/21/17.
//

#include <cstdlib>
#include <iostream>
#include "time.h"
#include "../cwt.h"
#include "../cts.h"

#define SWITCHING true

int main() {

    std::cout << "Testing Tree construction and destruction";

    srand(uint(time(0)));
    int num_adds = 100000;
    int context_depth = 10;
    uint8 context[context_depth];

#if SWITCHING
    context_tree *tree = new cts(context_depth);
#else
    context_tree *tree = new cwt(context_depth);
#endif

    for (int i = 0; i < num_adds; ++i) {
        for (int j = 0; j < context_depth; ++j) {
            context[j] = uint8(rand() % 2);
        }

        tree->logprob(context, uint8(rand() % 2));
        tree->update_and_logprob(context, uint8(rand() % 2));
    }

    delete tree;

    return 0;
}

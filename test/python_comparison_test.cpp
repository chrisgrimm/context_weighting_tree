//
// Created by Mel Roderick on 8/11/17.
//

#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include "../cwt_symbol.h"
#include "../location_dependent_cwt_symbol.h"

int main() {
    srand (time(NULL));

    int context_length = 4;
    int alphabet_size = 5;

    cwt_symbol *c = new cwt_symbol(context_length, alphabet_size);

    uint8 context[] = {1, 1, 1, 1};

    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;


    // PART 1:
    double logprob;
    double logprob_prime;
    for (int j = 0; j < 1000; ++j) {
        logprob = c->update_and_logprob(context, 1);
        logprob_prime = c->logprob(context, 1);
//        cout << logprob << ' ' << logprob_prime << '\n';
    }
    assert(fabs(logprob - (-0.00080032017077)) < 10e-9);
    assert(fabs(logprob_prime - (-0.000799520330418)) < 10e-9);


    // PART 2:
    c = new cwt_symbol(context_length, alphabet_size);
    uint8 contexts[5][4] = {
            {1, 1, 1, 1},
            {1, 3, 0, 2},
            {2, 1, 3, 1},
            {2, 0, 4, 4},
            {1, 3, 1, 0}
    };

    for (int i = 0; i < 1000; ++i) {
        uint8 s = uint8(i % 5);
        logprob = c->update_and_logprob(contexts[s], s);
        logprob_prime = c->logprob(contexts[s], s);
        cout << logprob << ' ' << logprob_prime << '\n';
    }
    assert(fabs(logprob - (-0.00701757148749)) < 10e-9);
    assert(fabs(logprob_prime - (-0.00698489381155)) < 10e-9);


    gettimeofday(&tp, NULL);
    long int ms2 = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    cout << (ms2 - ms)/1000.0 << "\n";
}
//
// Created by Mel Roderick on 8/11/17.
//

#include <cstdlib>
#include <chrono>
#include <sys/time.h>
#include <tiff.h>
#include <iostream>
#include "../location_dependent_cwt_symbol.h"

using namespace std;
using namespace std::chrono;


int main() {
    srand (time(NULL));

    /* generate secret number between 1 and 10: */
    cout << "hi!" << "\n";
    int w = 42;
    int h = 42;
    int alphabet_size = 5;
    auto ldc = location_dependent_cwt_symbol(w, h, alphabet_size);
    uint8 image[w*h];
    for (int i=0; i< w*h; i++) {
        image[i] = (uint8) (rand() % alphabet_size);
    }
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    for (int x=0; x<1000; x++) {
        double image_pseudocount = ldc.process_image(image);
//        for (int i=0; i< w*h; i++) {
//            image[i] = (uint8) (rand() % 2);
//        }
        cout << x << ": " << image_pseudocount << "\n";
    }
    gettimeofday(&tp, NULL);
    long int ms2 = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    cout << (ms2 - ms)/1000.0 << "\n";
}
#include <iostream>
#include <tiff.h>
#include <vector>
#include <cmath>
#include <tuple>
#include "../location_dependent_cwt.h"
#include <chrono>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace std::chrono;


int main() {
    srand (time(NULL));

    /* generate secret number between 1 and 10: */
    cout << "hi!" << "\n";
    bool switching = true;
    int w = 42;
    int h = 42;
    int num_bits = 3;
    auto ldc = location_dependent_cwt(w, h, num_bits, switching);
    uint8 image[w*h];
    for (int i=0; i< w*h; i++) {
        image[i] = (uint8) (rand() % 2);
    }
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    for (int x=0; x<100; x++) {
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
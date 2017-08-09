//
// Created by maroderi on 7/23/17.
//

#include "cwt.h"
#include "location_dependent_cwt.h"

#define SWITCHING true

extern "C" location_dependent_cwt* constructCTS(int width, int height, int bits_per_symbol) {
    return new location_dependent_cwt(width, height, bits_per_symbol, SWITCHING);
}

extern "C" double psuedo_count_for_image(location_dependent_cwt* ldc, uint8 *image) {
    return ldc->process_image(image);
}

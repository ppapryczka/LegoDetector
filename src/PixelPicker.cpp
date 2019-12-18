#include "PixelPicker.hpp"


// PixelPicker

PixelPicker::~PixelPicker()
{}

// HSVPixelPicker

HSVPixelPicker::HSVPixelPicker(uint8_t minH, uint8_t maxH, uint8_t minS, uint8_t maxS, uint8_t minV, uint8_t maxV)
    :minH(minH), maxH(maxH), minS(minS), maxS(maxS), minV(minV), maxV(maxV)
{}

bool HSVPixelPicker::isCorrectPixel(uint8_t h, uint8_t s, uint8_t v) const {
    if (!(h>=minH && h<=maxH)){
        return false;
    } else if (!(s>=minS && s<=maxS)){
        return false;
    } else if (!(v>=minV && v<=maxV)){
        return false;
    }

    return true;
}




#include "PixelPicker.hpp"


// PixelPicker

PixelPicker::~PixelPicker()
{}

// HSVPixelPicker

HSVPixelPicker::HSVPixelPicker(float minH, float maxH, float minS, float maxS, float minV, float maxV)
    :minH(minH), maxH(maxH), minS(minS), maxS(maxS), minV(minV), maxV(maxV)
{}

bool HSVPixelPicker::isCorrectPixel(float h, float s, float v) const {
    if (!(h>=minH && h<=maxH)){
        return false;
    } else if (!(s>=minS && s<=maxS)){
        return false;
    } else if (!(v>=minV && v<=maxV)){
        return false;
    }

    return true;
}




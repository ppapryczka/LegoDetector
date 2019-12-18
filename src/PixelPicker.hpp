#ifndef PIXELPICKER_HPP
#define PIXELPICKER_HPP

#include<cstdint>

class PixelPicker
{
public:
    virtual bool isCorrectPixel(uint8_t c1, uint8_t c2, uint8_t c3) const = 0;
    virtual ~PixelPicker();
};

class HSVPixelPicker : public PixelPicker{
private:
    uint8_t minH, maxH;
    uint8_t minS, maxS;
    uint8_t minV, maxV;

public:
    HSVPixelPicker(uint8_t minH, uint8_t maxH, uint8_t minS, uint8_t maxS, uint8_t minV, uint8_t maxV);
    bool isCorrectPixel(uint8_t h, uint8_t s, uint8_t v) const;
};

const HSVPixelPicker filterHSV1(4, 12, 140, 220, 80, 200);

#endif // PIXELPICKER_HPP

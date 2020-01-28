/**
  * Header file for PixelPicker class, which can be use to choose pixels from image.
  */

#ifndef PIXELPICKER_HPP
#define PIXELPICKER_HPP

#include<cstdint>

/**
 * @class PixelPicker
 * @brief The PixelPicker class - abstract class to decide if color is valid.
 */
class PixelPicker
{
public:
    /**
     * @brief isCorrectPixel Check if given color(c1, c2, c3) is valid.
     * @param c1 First color channel.
     * @param c2 Second color channel.
     * @param c3 Thirs color channel.
     * @return True of color is valide, false otherwise.
     */
    virtual bool isCorrectPixel(float c1, float c2, float c3) const = 0;
    /**
     * @brief ~PixelPicker - dummy virtual destructor.
     */
    virtual ~PixelPicker();
};

/**
 * @class HSVPixelPicker
 * @brief The HSVPixelPicker class - class appropirate to valid HSV colors by
 * ranges of each channel.
 */

class HSVPixelPicker : public PixelPicker{
private:
    float minH, maxH;
    float minS, maxS;
    float minV, maxV;

public:
    /**
     * @brief HSVPixelPicker construct that only sets class fields.
     * @param minH Minimal H value.
     * @param maxH Maximal H value.
     * @param minS Minimal S value.
     * @param maxS Maximal S value.
     * @param minV Minimal V value.
     * @param maxV Maximal V value.
     */
    HSVPixelPicker(float minH, float maxH, float minS, float maxS, float minV, float maxV);

    /**
     * @brief isCorrectPixel Check color by given in constructor ranges.
     * @param h H(hue) value.
     * @param s S(saturation) value.
     * @param v V(value) value.
     * @return True if color is in given ranges, false otherwise.
     */
    bool isCorrectPixel(float h, float s, float v) const;
};


// picker for HSV in scale ised by GIMP program
const HSVPixelPicker FILTER_GIMP = HSVPixelPicker(14, 40, 40, 100, 20, 90);

#endif // PIXELPICKER_HPP

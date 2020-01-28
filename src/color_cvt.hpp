/**
  * Few functions to convert color scale to HSV.
  * #TODO - code duplications!
  */

#ifndef COLOR_CVT_HPP
#define COLOR_CVT_HPP

// opencv
#include <opencv2/core/core.hpp>

// std
#include <vector>
#include <cstdint>
#include <iostream>

// DEFINITIONS OF COLOR SCALES WHEN CONVERT TO HSV

// own scale, use by DuckDuckGO for example, but hue divided by 2
const unsigned int HUE_SCALE = 2; // should be 1
const unsigned int VALUE_SCALE = 100;
const unsigned int SATURATION_SCALE = 100;

// scale use by OpenCV
const unsigned int HUE_SCALE_OPENCV = 2;
const unsigned int VALUE_SCALE_OPENCV = 255;
const unsigned int SATURATION_SCALE_OPENCV = 255;

// GIMP scale
const unsigned int HUE_SCALE_GIMP = 1;
const unsigned int VALUE_SCALE_GIMP = 100;
const unsigned int SATURATION_SCALE_GIMP = 100;


typedef std::vector<uint8_t> (*cvtColorFuntion)(uint8_t r, uint8_t g, uint8_t b);


/**
 * @details cvtColorBGRtoHSV Convert BGR color to HSV color - for details look:
 * https://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html
 * @param b Blue color value.
 * @param g Green color value.
 * @param r Red color value.
 * @return HSV color as a vector 0<H<360, 0<S<1, 0<V<1.
 */
std::vector<double> cvtColorBGRToHSV(uint8_t b, uint8_t g, uint8_t r){
    double r_ = r/static_cast<double>(std::numeric_limits<uint8_t>::max());
    double g_ = g/static_cast<double>(std::numeric_limits<uint8_t>::max());
    double b_ = b/static_cast<double>(std::numeric_limits<uint8_t>::max());
    double max = std::max({r_, g_, b_});
    double min = std::min({r_, g_, b_});
    double delta = max - min;

    double hue, value, saturation;

    // hue
    if (delta!=0){
        if (max == r_){
            hue =60.0*(g_ - b_)/delta;
        } else if (max == g_){
            hue = 120.0 + 60.0*(b_ - r_)/delta;
        } else {
            hue = 240.0 + 60.0*(r_ - g_)/delta;
        }
    } else {
        hue = 0.0;
    }

    // saturation
    if (max == 0){
        saturation = 0;
    } else {
        saturation = delta/max;
    }

    // value
    value = max;

    if (hue<0){
        hue+=360;
    }

    return {hue, saturation, value};
}


/**
 * @details cvtColorBGRtoHSVOwnScale Convert BGR color to HSV color - for details look:
 * https://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html
 * This version uses own color scale.
 * @param b Blue color value.
 * @param g Green color value.
 * @param r Red color value.
 * @return HSV color as a vector, 0<H<180, 0<S<100, 0<V<100.
 */
std::vector<uint8_t> cvtColorBGRToHSVOwnScale(uint8_t b, uint8_t g, uint8_t r){
    // run convert value to HSV
    auto color = cvtColorBGRToHSV(b, g, r);

    // convert values to OpenCV scale
    uint8_t hue = static_cast<uint8_t>(color[0]/HUE_SCALE);
    uint8_t saturation = static_cast<uint8_t>(color[1]*SATURATION_SCALE);
    uint8_t value = static_cast<uint8_t>((color[2]*VALUE_SCALE));


    return {hue, saturation, value};
}

/**
 * @details cvtColorBGRtoHSVOpenCVScale Convert BGR color to HSV color - for details look:
 * https://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html
 * This version uses OpenCV color scale.
 * @param b Blue color value.
 * @param g Green color value.
 * @param r Red color value.
 * @return HSV color as a vector, 0<H<180, 0<S<255, 0<V<255.
 */
std::vector<uint8_t> cvtColorBGRToHSVOpenCVScale(uint8_t r, uint8_t g, uint8_t b){
    // run convert value to HSV
    auto color = cvtColorBGRToHSV(r, g, b);

    // convert values to OpenCV scale
    uint8_t hue = static_cast<uint8_t>(color[0]/HUE_SCALE_OPENCV);
    uint8_t saturation = static_cast<uint8_t>(color[1]*SATURATION_SCALE_OPENCV);
    uint8_t value = static_cast<uint8_t>((color[2]*VALUE_SCALE_OPENCV));

    return {hue, saturation, value};
}


std::vector<uint8_t> cvtColorHSVToBGROpenCVScale(uint8_t h, uint8_t s, uint8_t v){
    double h_ = h * static_cast<double>(HUE_SCALE_OPENCV);
    double s_ = s / static_cast<double>(SATURATION_SCALE_OPENCV);
    double v_ = v / static_cast<double>(VALUE_SCALE_OPENCV);

    h_ = h_/60.0;


    unsigned int mod = static_cast<unsigned int>(std::floor(h_)) % 6;

    double alfa = v_ * (1.0 - s_);
    double beta = v_ * (1.0 - (h_ - static_cast<double>(mod)) * s_);
    double gama = v_ * (1.0 - ( 1.0 - (h_ - static_cast<double>(mod))) * s_);

    double r, g, b;

    switch (mod)
    {

        case 0:
        r = v_;
        g = gama;
        b = alfa;
        break;

        case 1:
        r = beta;
        g = v_;
        b = alfa;
        break;

        case 2:
        r = alfa;
        g = v_;
        b = gama;
        break;

        case 3:
        r = alfa;
        g = beta;
        b = v_;
        break;

        case 4:
        r = gama;
        g = alfa;
        b = v_;
        break;

        case 5:
        r = v_;
        g = alfa;
        b = beta;
        break;
    }

    r = r * static_cast<double>(std::numeric_limits<uint8_t>::max());
    g = g * static_cast<double>(std::numeric_limits<uint8_t>::max());
    b = b * static_cast<double>(std::numeric_limits<uint8_t>::max());


    return {static_cast<uint8_t>(b),static_cast<uint8_t>(g), static_cast<uint8_t>(r) };
}

/**
 * @brief cvtImgColors Covert image using given convert pixel color function.
 * @param img Image to convertion.
 * @param cvtFunc Function that convert 3 channel image pixel.
 * @return Converted image.
 */
cv::Mat cvtImgColors(const cv::Mat& img, cvtColorFuntion cvtFunc){
    // create copy
    cv::Mat res(img.rows, img.cols, CV_8UC3);

    // get iterators
    cv::Mat_<cv::Vec3b> new_iter = res;
    cv::Mat_<cv::Vec3b> original_iter = img;

    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {
            auto color = cvtFunc(original_iter(i,j)[0], original_iter(i, j)[1], original_iter(i, j)[2]);
            new_iter(i, j)[0] = color[0];
            new_iter(i, j)[1] = color[1];
            new_iter(i, j)[2] = color[2];
        }
    }

    return res;
}

/**
 * @brief cvtImgColorsToGIMPHSV Convert image color to GIMP scale: 0<H<360, 0<S<100, 0<V<100.
 * @param img Image to convert
 * @return Converted image, 3 float channel image!
 */
cv::Mat cvtImgColorsToGIMPHSV(const cv::Mat& img){
    cv::Mat res(img.rows, img.cols, CV_32FC3);

    // get iterators
    cv::Mat_<cv::Vec3b> original_iter = img;
    cv::Mat_<cv::Vec3f> new_iter = res;

    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {
            auto color = cvtColorBGRToHSV(original_iter(i,j)[0], original_iter(i, j)[1], original_iter(i, j)[2]);
            new_iter(i, j)[0] = color[0]*HUE_SCALE_GIMP;
            new_iter(i, j)[1] = color[1]*SATURATION_SCALE_GIMP;
            new_iter(i, j)[2] = color[2]*VALUE_SCALE_GIMP;
        }
    }

    return res;
}



#endif // COLOR_CVT_HPP

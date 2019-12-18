#ifndef UTILS_HPP
#define UTILS_HPP

// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>

// std
#include <exception>
#include <limits>
#include <iostream>
#include <fstream>

// lego
#include "PixelPicker.hpp"

const std::vector<std::string> test_files_names_1 = {
    "DSC_6266.JPG",
    "DSC_6267.JPG",
    "DSC_6268.JPG",
    "DSC_6269.JPG",
    "DSC_6270.JPG",
    "DSC_6271.JPG"
};

const std::vector<std::string> test_files_names_2 = {
    "koc_1.JPG",
    "koc_2.JPG",
    "koc_3.JPG",
    "gazeta1_1.JPG",
    "gazeta1_2.JPG",
    "gazeta1_3.JPG",
    "gazeta2_1.JPG",
    "gazeta2_2.JPG",
    "gazeta3_1.JPG",
    "gazeta3_2.JPG",
    "gazeta3_3.JPG"
};

// own scale, use by DuckDuckGO for example, but hue divided by 2
const unsigned int HUE_SCALE = 30; // should be 60
const unsigned int VALUE_SCALE = 100;
const unsigned int SATURATION_SCALE = 100;

// scale use by OpenCV
const unsigned int HUE_SCALE_OPENCV = 2;
const unsigned int VALUE_SCALE_OPENCV = 255;
const unsigned int SATURATION_SCALE_OPENCV = 255;

typedef std::vector<uint8_t> (*cvtColorFuntion)(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief rankFilter Converts given image using neighbours and brightness information.
 * @param img Image to convertion.
 * @param width Width of neighbours window.
 * @param height Height of neighbours window.
 * @param rank ID of pixel in neighbours window sorted by brightness - pixel with given ID
 * is choose as a new value in result image.
 * @return Converted image.
 */
cv::Mat rankFilter(const cv::Mat& img, int width, int height, unsigned int rank){
    // check arguments
    if(width<0 || height<0){
        throw std::runtime_error("");
    }
    else if(rank>=static_cast<unsigned int>(width*height)){
        throw std::runtime_error("Wrong rank value!");
    }
    else if(height%2 == 0 || width%2 == 0){
        throw std::runtime_error("Filter size not odd!");
    }

    // create copy
    cv::Mat res(img.rows, img.cols, CV_8UC3);

    // get iterators
    cv::Mat_<cv::Vec3b> new_iter = res;
    cv::Mat_<cv::Vec3b> original_iter = img;

    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {

            // copy not change pixels
            if (i < (height / 2) || i>= (img.rows - height / 2) || j < (width / 2) || j>=(img.cols - width / 2)){
                new_iter(i, j)[0] = original_iter(i, j)[0];
                new_iter(i, j)[1] = original_iter(i, j)[1];
                new_iter(i, j)[2] = original_iter(i, j)[2];
            }
            // execute filter for other pixels
            else {
                std::vector<std::pair<float, std::pair<int, int>>> vec;
                for (int row = i - height/2; row<=i + height/2; ++row)
                {
                    for (int col = j - width / 2; col <= j + width / 2; ++col) {
                        double c = (original_iter(row, col)[0] + original_iter(row, col)[1] + original_iter(row, col)[2]) / 3.0;
                        vec.emplace_back(std::pair<double, std::pair<int, int>>(c, std::pair<int, int>(row, col)));
                    }
                }

                std::sort(vec.begin(), vec.end(),
                    [](const std::pair<double, std::pair<int, int>>&a, const std::pair<double, std::pair<int, int>>&b) -> bool{
                    return a.first < b.first;});

                new_iter(i, j)[0] = original_iter(vec[rank].second.first, vec[rank].second.second)[0];
                new_iter(i, j)[1] = original_iter(vec[rank].second.first, vec[rank].second.second)[1];
                new_iter(i, j)[2] = original_iter(vec[rank].second.first, vec[rank].second.second)[2];
            }
        }
    }

    return res;
}

/**
 * @details cvtColorRGBtoHSV Convert BGR color to HSV color - for details look:
 * https://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html
 * This version uses own color scale.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @return HSV color as a vector.
 */
std::vector<uint8_t> cvtColorRGBToHSVOwnScale(uint8_t r, uint8_t g, uint8_t b){
    float r_ = r/static_cast<float>(std::numeric_limits<uint8_t>::max());
    float g_ = g/static_cast<float>(std::numeric_limits<uint8_t>::max());
    float b_ = b/static_cast<float>(std::numeric_limits<uint8_t>::max());
    float max = std::max({r_, g_, b_});
    float min = std::min({r_, g_, b_});
    float delta = max - min;

    float hue, value, saturation;

    // hue
    if (max == r_){
        hue =(g_ - b_)/delta;
    } else if (max == g_){
        hue = 2 + (b_ - r_)/delta;
    } else {
        hue = 4 + (r_ - g_)/delta;
    }

    // saturation
    if (max == 0){
        saturation = 0;
    } else {
        saturation = delta/max;
    }

    // value
    value = max;

    hue = std::round(hue*HUE_SCALE);
    saturation = std::round(saturation*SATURATION_SCALE);
    value = std::round(value*VALUE_SCALE);

    if (hue<0){
        hue+=180;
    }

    return {static_cast<uint8_t>(hue), static_cast<uint8_t>(saturation), static_cast<uint8_t>(value)};
}

/**
 * @details cvtColorRGBtoHSV Convert BGR color to HSV color - for details look:
 * https://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html
 * This version uses OpenCV color scale.
 * @param r Red color value.
 * @param g Green color value.
 * @param b Blue color value.
 * @return HSV color as a vector.
 */
std::vector<uint8_t> cvtColorRGBToHSVOpenCVScale(uint8_t r, uint8_t g, uint8_t b){
    // run convert value to own scale
    auto color = cvtColorRGBToHSVOwnScale(r, g, b);

    // convert values to OpenCV scale
    uint8_t hue = static_cast<uint8_t>(color[0]/static_cast<float>(HUE_SCALE_OPENCV));
    uint8_t saturation = static_cast<uint8_t>((color[1]/static_cast<float>(SATURATION_SCALE))*(SATURATION_SCALE_OPENCV));
    uint8_t value = static_cast<uint8_t>((color[2]/static_cast<float>(VALUE_SCALE))*(VALUE_SCALE_OPENCV));

    return {hue, saturation, value};
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
            auto color = cvtFunc(original_iter(i,j)[2], original_iter(i, j)[1], original_iter(i, j)[0]);
            new_iter(i, j)[0] = color[0];
            new_iter(i, j)[1] = color[1];
            new_iter(i, j)[2] = color[2];
        }
    }

    return res;
}

cv::Mat pickPixels(const cv::Mat& img, const PixelPicker& pp){
    // create copy
    cv::Mat res(img.rows, img.cols, CV_8UC3);

    // get iterators
    cv::Mat_<cv::Vec3b> new_iter = res;
    cv::Mat_<cv::Vec3b> original_iter = img;

    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {
            if(pp.isCorrectPixel(original_iter(i, j)[0], original_iter(i, j)[1], original_iter(i, j)[2])){
                new_iter(i, j)[0] = 0;
                new_iter(i, j)[1] = 255;
                new_iter(i, j)[2] = 255;
            } else {
                new_iter(i, j)[0] = original_iter(i,j)[0];
                new_iter(i, j)[1] = original_iter(i,j)[1];
                new_iter(i, j)[2] = original_iter(i,j)[2];
            }
        }
    }

    return res;
}

/**
 * @brief showImgAndWait Show image and wait for ESC key.
 * @param img Image to display.
 * @param name Name of display window, default: image.
 */

void showImgAndWait(const cv::Mat& img, std::string name = "image" ){
    cv::namedWindow( name, cv::WINDOW_NORMAL );
    cv::imshow(name, img);
    cv::waitKey(-1);
}


/**
 * @brief saveImgColorsToCSV Simple function that saves colors of given
 * image to csv file.
 * @param img Image from which will be taken colors.
 * @param csvName Name of created csv file.
 */
void saveImgColorsToCSV(const cv::Mat& img, std::string csvName = "colors.csv"){
    std::fstream file(csvName, std::ios::out);

    cv::Mat_<cv::Vec3b> original_iter = img;
    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {
            if(!(original_iter(i, j)[0]==0 && original_iter(i, j)[1]==0 && original_iter(i, j)[2]==255)){
                file<<int(original_iter(i, j)[0])<<";"<<int(original_iter(i, j)[1])<<";"<<int(original_iter(i, j)[2])<<";\n";
            }
        }
    }
    file.close();
}

#endif // UTILS_HPP

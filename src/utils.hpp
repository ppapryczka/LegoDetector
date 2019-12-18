#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/core/core.hpp>
#include <exception>
#include <limits>
#include <iostream>

const std::vector<std::string> test_files_names = {
    "DSC_6266.JPG",
    "DSC_6267.JPG",
    "DSC_6268.JPG",
    "DSC_6269.JPG",
    "DSC_6270.JPG",
    "DSC_6271.JPG"
};

const unsigned int HUE_SCALE = 60;
const unsigned int VALUE_SCALE = 100;
const unsigned int SATURATION_SCALE = 100;
const uint8_t MAX_UINT8_T = 255;

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
    cv::Mat_<cv::Vec3b> pixel_iter = res;
    cv::Mat_<cv::Vec3b> original_iter = img;

    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {

            // copy not change pixels
            if (i < (height / 2) || i>= (img.rows - height / 2) || j < (width / 2) || j>=(img.cols - width / 2)){
                pixel_iter(i, j)[0] = original_iter(i, j)[0];
                pixel_iter(i, j)[1] = original_iter(i, j)[1];
                pixel_iter(i, j)[2] = original_iter(i, j)[2];
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

                pixel_iter(i, j)[0] = original_iter(vec[rank].second.first, vec[rank].second.second)[0];
                pixel_iter(i, j)[1] = original_iter(vec[rank].second.first, vec[rank].second.second)[1];
                pixel_iter(i, j)[2] = original_iter(vec[rank].second.first, vec[rank].second.second)[2];
            }
        }
    }

    return res;
}


void cvtColorRGBtoHSV(uint8_t r, uint8_t g, uint8_t b){
    float r_ = r/static_cast<float>(std::numeric_limits<uint8_t>::max());
    float g_ = g/static_cast<float>(std::numeric_limits<uint8_t>::max());
    float b_ = b/static_cast<float>(std::numeric_limits<uint8_t>::max());
    float max = std::max({r_, g_, b_});
    float min = std::min({r_, g_, b_});
    float delta = max - min;

    float hue, value, saturation;

    // hue
    if(delta == 0){
        throw std::runtime_error("Hue have undefinied value!");
    } else if (max == r_){
        hue =(g_ - b_)/delta;
    } else if (max == g_){
        hue = (b_ - r_)/delta + 2;
    } else {
        hue =(r_ - g_)/delta + 4;
    }

    // value
    value = max;

    // saturation
    if (value == 0){
        saturation = 0;
    } else {
        saturation = delta/value;
    }

    hue = hue*HUE_SCALE;
    value = value*VALUE_SCALE;
    saturation = saturation*SATURATION_SCALE;

    if (hue<0){
        hue+=360;
    }

    std::cout<<" H: "<<hue<<" S:"<<saturation<<" V: "<<value<<std::endl;
}




void cvtColorHSVtoRGB(){

}


#endif // UTILS_HPP

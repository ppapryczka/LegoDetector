#ifndef UTILS_HPP
#define UTILS_HPP

// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>

// std
#include <exception>
#include <limits>
#include <iostream>
#include <fstream>
#include <unordered_set>

// lego
#include "PixelPicker.hpp"

const int DEFUALT_RANK_FILTER_WIDTH = 5;
const int DEFUALT_RANK_FILTER_HEIGHT = 5;
const int DEFAULT_RANL_FILTER_RANK =5;

const int DEFUALT_PIX_CHOOSE_WIDTH = 31;
const int DEFUALT_PIX_CHOOSE_HEIGHT = 31;
const float DEFUALT_PIX_CHOOSE_PERCENT = 0.6f;



const std::vector<std::string> TEST_FILES_NAMES = {
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

using PixelsMap = std::vector<std::vector<bool>>;

const std::string BLUEST_QUOTE =std::string("We are on a mission from God!");

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
 * @brief pickPixels Pick pixels from image using given PixelPicker.
 * @param img Iamge to take pixels from.
 * @param pp Pixel validator.
 * @return Pixel map of true and false values.
 */
PixelsMap pickPixels(const cv::Mat& img, const PixelPicker& pp){
    // get iterator
    cv::Mat_<cv::Vec3f> original_iter = img;

    PixelsMap pixelsMap(img.rows);

    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {
            if(pp.isCorrectPixel(original_iter(i, j)[0], original_iter(i, j)[1], original_iter(i, j)[2])){
                pixelsMap[i].emplace_back(true);
                //pixels.emplace(cv::Point2i(i, j));
            }else{
                pixelsMap[i].emplace_back(false);
            }
        }
    }


    return pixelsMap;
}


/**
 * @brief neighbourAwarePixelPicker Pick pixel using local information.
 * @param img Soucre image.
 * @param pp Pixel validator.
 * @param width Width of neighbours window.
 * @param height Height of neighbours window.
 * @param percent Percent of chosen pixel in  neighbours window.
 * @return Pixels map of rue and flase values.
 */
PixelsMap neighbourAwarePixelPicker(const cv::Mat& img, const PixelPicker& pp, int width, int height, float percent){
    // check arguments
    if(width<0 || height<0){
        throw std::runtime_error("");
    }
    else if(height%2 == 0 || width%2 == 0){
        throw std::runtime_error("Filter size not odd!");
    }

    cv::Mat_<cv::Vec3b> original_iter = img;

    PixelsMap pixelsMap(img.rows);


    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {

            // copy not change pixels
            if (i < (height / 2) || i>= (img.rows - height / 2) || j < (width / 2) || j>=(img.cols - width / 2)){
                pixelsMap[i].emplace_back(false);
                continue;
            }
            // execute filter for other pixels
            else {
                int num = 0;
                for (int row = i - height/2; row<=i + height/2; ++row)
                {
                    for (int col = j - width / 2; col <= j + width / 2; ++col) {
                        if(pp.isCorrectPixel(original_iter(row, col)[0], original_iter(row, col)[1], original_iter(row, col)[2])){
                            ++num;
                        }
                    }
                }

                if (static_cast<float>(num)/static_cast<float>(width * height)>percent){
                    pixelsMap[i].emplace_back(true);
                } else{
                    pixelsMap[i].emplace_back(false);
                }
            }
        }
    }
    return pixelsMap;
}
/**
  *
  */
cv::Mat colorGivenPixelMap(const cv::Mat& img, const PixelsMap& pixelsMap, std::vector<uint8_t> color = {255, 0, 0}){
    // create copy
    cv::Mat res(img.rows, img.cols, CV_8UC3);

    // get iterators
    cv::Mat_<cv::Vec3b> new_iter = res;
    cv::Mat_<cv::Vec3b> original_iter = img;


    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {
            if(pixelsMap[i][j]){
                new_iter(i, j)[0] = color[0];
                new_iter(i, j)[1] = color[1];
                new_iter(i, j)[2] = color[2];
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
 * @brief closing
 * @param pixMap
 * @param width
 * @param height
 * @return
 */
PixelsMap closing(const PixelsMap& pixMap, int width, int height){
    // check arguments

    PixelsMap copy;

    if(width<0 || height<0){
        throw std::runtime_error("");
    }
    else if(height%2 == 0 || width%2 == 0){
        throw std::runtime_error("Filter size not odd!");
    }

    for (int i = 0; i < pixMap.size(); ++i){
        copy.emplace_back(std::vector<bool>());
        for (int j = 0; j < pixMap[i].size(); ++j) {

            if (!(i < (height / 2) || i>= (pixMap.size()- height / 2) || j < (width / 2) || j>=(pixMap[i].size() - width / 2))){
                bool found = false;
                for (int row = i - height/2; row<=i + height/2; ++row){
                    for (int col = j - width / 2; col <= j + width / 2; ++col) {
                        if ( pixMap[row][col]){
                            found = true;
                            break;
                        }
                    }
                }

                if(found){
                    copy[i].emplace_back(true);
                } else{
                    copy[i].emplace_back(false);
                }

            } else {
                copy[i].emplace_back(pixMap[i][j]);
            }
        }
    }

    return copy;
};


PixelsMap opening(const PixelsMap& pixMap, int width, int height){
    // check arguments

    PixelsMap copy;

    if(width<0 || height<0){
        throw std::runtime_error("");
    }
    else if(height%2 == 0 || width%2 == 0){
        throw std::runtime_error("Filter size not odd!");
    }

    for (int i = 0; i < pixMap.size(); ++i){
        copy.emplace_back(std::vector<bool>());
        for (int j = 0; j < pixMap[i].size(); ++j) {

            if (!(i < (height / 2) || i>= (pixMap.size()- height / 2) || j < (width / 2) || j>=(pixMap[i].size() - width / 2))){
                bool found = false;
                for (int row = i - height/2; row<=i + height/2; ++row){
                    for (int col = j - width / 2; col <= j + width / 2; ++col) {
                        if ( pixMap[row][col] == false){
                            found = true;
                            break;
                        }
                    }
                }

                if(found){
                    copy[i].emplace_back(false);
                } else{
                    copy[i].emplace_back(true);
                }

            } else {
                copy[i].emplace_back(pixMap[i][j]);
            }
        }
    }

    return copy;
};



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

    cv::Mat_<cv::Vec3f> original_iter = img;
    for (int i = 0; i < img.rows ; ++i){
        for (int j = 0; j < img.cols; ++j) {
            if(!(original_iter(i, j)[0]==0 && original_iter(i, j)[1]==0 && original_iter(i, j)[2]==255)){
                file<<float(original_iter(i, j)[0])<<";"<<float(original_iter(i, j)[1])<<";"<<float(original_iter(i, j)[2])<<"\n";
            }
        }
    }
    file.close();
}

#endif // UTILS_HPP

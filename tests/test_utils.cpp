// catch2
#include "catch2.hpp"

// lego
#include "../src/utils.hpp"

// std
#include<vector>
#include<cmath>
#include<limits>

// opencv
#include <opencv2/opencv.hpp>
#include <opencv2/core/hal/interface.h>


TEST_CASE("Tests for rankFilter function", "[utils][rankFilter]"){
    SECTION("convert to HSV with own scale pure red color"){
        cv::Mat img(3, 3, CV_8UC3);
        cv::Mat_<cv::Vec3b> m = img;
        for ( int row =0; row<img.rows; ++row){
            for(int col = 0; col<img.cols; ++col){
                uint8_t c = row;
                m(row, col) = {c, c, c};
            }
        }

        auto res = rankFilter(img, 3, 3, 4);
        m = res;
        cv::Vec3b expected = {2, 2, 2};
        REQUIRE(m(2, 2) == expected);
    }
}

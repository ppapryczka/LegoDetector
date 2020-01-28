// catch2
#include "catch2.hpp"

// lego
#include "../src/color_cvt.hpp"

// std
#include<vector>
#include<cmath>
#include<limits>

// opencv
#include <opencv2/opencv.hpp>
#include <opencv2/core/hal/interface.h>

TEST_CASE("Tests for BGR to HSV functions", "[color_cvt][cvtColorBGRToHSV]"){
    SECTION("convert to HSV with own scale pure green color"){
        std::vector<uint8_t> color = {0, 255, 0};
        std::vector<uint8_t> cvt_result = cvtColorBGRToHSVOwnScale(color[0], color[1], color[2]);
        std::vector<uint8_t> expected_result = {60, 100, 100};

        REQUIRE(cvt_result == expected_result);
    }

    SECTION("convert to HSV with own scale pure red color"){
        std::vector<uint8_t> color = {0, 0, 255};
        std::vector<uint8_t> cvt_result = cvtColorBGRToHSVOwnScale(color[0], color[1], color[2]);
        std::vector<uint8_t> expected_result = {0, 100, 100};

        REQUIRE(cvt_result == expected_result);
    }


    SECTION("convert to HSV with own scale pure blue color"){
        std::vector<uint8_t> color = {255, 0, 0};
        std::vector<uint8_t> cvt_result = cvtColorBGRToHSVOwnScale(color[0], color[1], color[2]);
        std::vector<uint8_t> expected_result = {120, 100, 100};

        REQUIRE(cvt_result == expected_result);
    }

    SECTION("convert to HSV with own scale few choosen colors"){
        std::vector<std::vector<uint8_t>> colors = {
            {127, 201, 90},
            {189, 60, 193},
            {189, 193, 60},
            {60, 193, 155},
            {60, 115, 193}
        };

        std::vector<std::vector<uint8_t>> expected_result = {
            {70, 55, 79},
            {151, 69, 76},
            {89, 69, 76},
            {38, 69, 76},
            {12, 69, 76}
        };

        for(size_t i = 0; i < colors.size(); ++i){
            auto color = cvtColorBGRToHSVOwnScale(colors[i][0], colors[i][1], colors[i][2]);
            REQUIRE( std::abs(static_cast<int>(color[0]) - static_cast<int>(expected_result[i][0])) <= 1);
            REQUIRE( std::abs(static_cast<int>(color[1]) - static_cast<int>(expected_result[i][1])) <= 1);
            REQUIRE( std::abs(static_cast<int>(color[2]) - static_cast<int>(expected_result[i][2])) <= 1);
        }
    }

    SECTION("convert to HSV with openCV scale few choosen colors"){
        cv::Mat img(5, 1, CV_8UC3);

        cv::Mat_<cv::Vec3b> m = img;

        m(0, 0) = {90, 201, 127};
        m(0, 1) = {193, 60, 189};
        m(0, 2) = {60, 193, 189};
        m(0, 3) = {155, 193, 60};
        m(0, 4) = {193, 115, 60};

        auto own_result = cvtImgColors(img, &cvtColorBGRToHSVOpenCVScale);
        cv::Mat_<cv::Vec3b> k = own_result;

        cv::cvtColor(img, img, cv::COLOR_BGR2HSV);


        for (int i = 0; i < img.rows ; ++i){
            for (int j = 0; j < img.cols; ++j) {
                REQUIRE(std::abs(static_cast<int>(m(i, j)[0]) - static_cast<int>(k(i, j)[0]))<= 1);
                REQUIRE(std::abs(static_cast<int>(m(i, j)[0]) - static_cast<int>(k(i, j)[0]))<= 1);
                REQUIRE(std::abs(static_cast<int>(m(i, j)[0]) - static_cast<int>(k(i, j)[0]))<= 1);
            }
        }
    }


    SECTION("convert to HSV with openCV scale few random colors"){
        srand(time(NULL));

        cv::Mat img(5, 1, CV_8UC3);

        cv::Mat_<cv::Vec3b> m = img;

        for( int i = 0; i<5; ++i){
            uint8_t r= rand()%std::numeric_limits<uint8_t>::max();
            uint8_t g= rand()%std::numeric_limits<uint8_t>::max();
            uint8_t b= rand()%std::numeric_limits<uint8_t>::max();
            m(0, i) = {r, g, b};
        }

        auto own_result = cvtImgColors(img, &cvtColorBGRToHSVOpenCVScale);
        own_result = cvtImgColors(own_result, &cvtColorHSVToBGROpenCVScale);

        cv::Mat_<cv::Vec3b> k = own_result;

        for (int i = 0; i < img.rows ; ++i){
            for (int j = 0; j < img.cols; ++j) {
                REQUIRE(std::abs(static_cast<int>(m(i, j)[0]) - static_cast<int>(k(i, j)[0]))<= 5);
                REQUIRE(std::abs(static_cast<int>(m(i, j)[0]) - static_cast<int>(k(i, j)[0]))<= 5);
                REQUIRE(std::abs(static_cast<int>(m(i, j)[0]) - static_cast<int>(k(i, j)[0]))<= 5);
            }
        }
    }
}

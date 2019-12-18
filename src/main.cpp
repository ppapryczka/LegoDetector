// cv
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// lego
#include "utils.hpp"


int main(int argc, char** argv)
{    
    for ( auto n : test_files_names_2){
        cv::Mat img = cv::imread(n);

        img = cvtImgColors(img, &cvtColorRGBToHSVOpenCVScale);

        img = rankFilter(img, 5, 5, 8);

        cv::Mat pickPixelsResult = pickPixels(img, filterHSV1);
        cv::cvtColor(pickPixelsResult, pickPixelsResult, cv::COLOR_HSV2BGR);

        cv::imwrite("test"+n, pickPixelsResult);
        std::cout<<n<<"\n";
    }

    std::cout<<"We are on a mission from God!\n";
    return 0;
}

/*
Notes:

// 1
cv::Mat img = cv::imread("pure.png");
cv::Mat copy;

cv::cvtColor(img, copy, cv::COLOR_BGR2HSV);

img = cvtImgColors(img, &cvtColorRGBToHSVOpenCVScale);


// 2
cv::Mat img = cv::imread("pure.png");
cv::Mat copy;

cv::cvtColor(img, copy, cv::COLOR_BGR2HSV);

img = cvtImgColors(img, &cvtColorRGBToHSVOpenCVScale);

cv::Mat pickColors = pickPixels(img, filterHSV1);
cv::cvtColor(pickColors, pickColors, cv::COLOR_HSV2BGR);

cv::imwrite("new_pure.png",pickColors);
*/

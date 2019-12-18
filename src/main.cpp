// cv
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// lego
#include "utils.hpp"


int main(int argc, char** argv )
{
    cvtColorRGBtoHSV(140, 56, 137);
    std::string name = "test.png";

    cv::Mat image = cv::imread(name);

    cv::cvtColor(image, image, cv::COLOR_BGR2HSV);


    /*
    for ( auto n : test_files_names){
        cv::Mat image = cv::imread(n);
        cv::Mat out;

        double scale = 0.15;
        int width = static_cast<int>(image.cols *scale);
        int height = static_cast<int>(image.rows *scale);

        cv::resize(image, out, cv::Size(width, height));

        cv::Mat res = rankFilter(out, 7, 7, 45);

        cv::namedWindow("Result", cv::WINDOW_AUTOSIZE );
        cv::imshow("Result", res);

        //cv::namedWindow("Original", cv::WINDOW_AUTOSIZE );
        //cv::imshow("Original", image);

        cv::waitKey(0);
    }
    */

    return 0;
}

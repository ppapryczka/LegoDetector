// cv
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// lego
#include "color_cvt.hpp"
#include "utils.hpp"


int main(int argc, char** argv)
{    

    std::vector<std::string> v = {"test.png"};

    for ( auto n : test_files_names_2){
        cv::Mat img = cv::imread(n);

        img = cvtImgColors(img, &cvtColorBGRToHSVOpenCVScale);
        //cv::cvtColor(img, img, cv::COLOR_BGR2HSV);

        img = rankFilter(img, 5, 5, 12);

        auto pixels = pickPixels(img, filterHSV1);

        auto copy = closing(pixels, 7, 7);
        //copy = opening(copy, 3, 1);
        copy = opening(copy, 7, 7);
        copy = opening(copy, 3, 3);

        //copy = closing(copy, 3, 1);
        //copy = closing(copy, 3, 3);



        //copy = opening(copy, 3, 3);
        //copy = closing(copy, 3, 3);

        //copy = opening(copy, 5, 5);


        cv::Mat pixelsImg = cvtImgColors(img, &cvtColorHSVToBGROpenCVScale);

        pixelsImg = colorGivenPixels(pixelsImg, copy);

        //cv::cvtColor(pickPixelsResult, pickPixelsResult, cv::COLOR_HSV2BGR);

        cv::imwrite("test"+n, pixelsImg);
        std::cout<<n<<std::endl;;
    }

    /*
    for ( int i = 1 ; i < 15; ++i){
        cv::Mat img = cv::imread("circle_"+std::to_string(i)+".png");

        img = cvtImgColors(img, &cvtColorRGBToHSVOpenCVScale);

        saveImgColorsToCSV(img, "colors_"+std::to_string(i)+".csv");

    }
    */


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



/*
 * #include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "utils.hpp"

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";


void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(1,1) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  imshow( window_name, dst );
 }



int main( int argc, char** argv )
{

  for ( auto n : test_files_names_2){
      /// Load an image
      src = imread(n);

      /// Create a matrix of the same type and size as src (for dst)
      dst.create( src.size(), src.type() );

      /// Convert the image to grayscale
      cvtColor( src, src_gray, cv::COLOR_BGR2GRAY );

      /// Create a window
      namedWindow( window_name, cv::WINDOW_AUTOSIZE );

      /// Create a Trackbar for user to enter threshold
      createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

      /// Show the image
      CannyThreshold(0, 0);

      /// Wait until user exit program by pressing a key
      waitKey(0);
  }

  return 0;
}
*/

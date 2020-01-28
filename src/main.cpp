// cv
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// lego
#include "color_cvt.hpp"
#include "utils.hpp"
#include "segmentation.hpp"
#include "moments.hpp"

// std
#include <random>
#include <experimental/filesystem>

int main(int argc, char** argv)
{
    // init random
    srand(time(nullptr));

    // check if arguments number is correct
    if(argc < 2 || argc > 4){
        std::cout<<"Usage <input file> <min segment size> <'-s' - optional: step mode>\n";
        return 0;
    }

    // load arguments
    std::string input_file = argv[1];
    int min_segment_size = std::atoi(argv[2]);
    bool step_mode = true;


    if (!std::experimental::filesystem::exists(input_file)){
        std::cout<<"Given file don't exist!\n";
        return 0;
    }

    if(min_segment_size<0){
        std::cout<<"Min segment size must be no negative!\n";
        return 0;
    }

    if(argc == 4 && std::strcmp(argv[3], "-d") == 0){
        step_mode = true;
    }

    cv::Mat img = cv::imread(input_file);
    cv::Mat copy = rankFilter(img, 5, 5, 5);

    auto res = cvtImgColorsToGIMPHSV(copy);

    auto pixels = neighbourAwarePixelPicker(res, FILTER_GIMP, 31, 31, 0.6f);

    std::vector<Segment> segments = findSegments(pixels);

    std::vector<Segment> chosen;
    for(auto seg : segments){
        if (isValidSegment(seg)){
            chosen.emplace_back(seg);
            drawBoundingRectForSegment(copy, seg);
        }
    }

    cv::imwrite("test_"+input_file, copy);


    // print the bluest quote ever
    std::cout<<BLUEST_QUOTE<<std::endl;
    return 0;
}

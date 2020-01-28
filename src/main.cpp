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
#include <fstream>


void proccessImage(std::string inputImg, std::string outputImg, int minSegSize, bool step_mode = false){
    // read image
    cv::Mat orginal_img = cv::imread(inputImg);

    // filter img
    cv::Mat filter_img = rankFilter(orginal_img, DEFUALT_RANK_FILTER_WIDTH,
                                    DEFUALT_RANK_FILTER_HEIGHT ,
                                    DEFAULT_RANK_FILTER_RANK);
    // save filter img
    if(step_mode)
        cv::imwrite("rank_filter_"+outputImg, filter_img);

    // convert image to Gimp HSV
    auto res = cvtImgColorsToGIMPHSV(filter_img);

    // chose pixels
    auto pixels = neighbourAwarePixelPicker(res, FILTER_GIMP,
                                            DEFUALT_PIX_CHOOSE_WIDTH,
                                            DEFUALT_PIX_CHOOSE_HEIGHT,
                                            DEFUALT_PIX_CHOOSE_PERCENT);
    // save pixels img
    if(step_mode){
        auto tmp = colorGivenPixelMap(filter_img, pixels);
        cv::imwrite("pixels_"+outputImg, tmp);
    }

    // find segments
    std::vector<Segment> segments = findSegments(pixels);
    // save segments img
    if(step_mode){
        auto tmp = filter_img.clone();
        colorSegmentsWithRandomColor(tmp, segments);
        cv::imwrite("segments_"+outputImg, tmp);
    }

    std::vector<Segment> chosen = removeAdditionalSegments(minSegSize, segments);
    // remove to small segments
    if(step_mode){
        auto tmp = filter_img.clone();
        colorSegmentsWithRandomColor(tmp, chosen);
        cv::imwrite("chosen_segments_"+outputImg, tmp);
    }

    // chose segments using moments
    for(auto seg : chosen){
        if (isValidSegment(seg)){
            chosen.emplace_back(seg);
            drawBoundingRectForSegment(orginal_img, seg);
        }
    }

    cv::imwrite(outputImg, orginal_img);

}

int main(int argc, char** argv)
{
    // init random
    srand(time(nullptr));

    // check if arguments number is correct
    if(argc < 2 || argc > 5){
        std::cout<<"Usage <input file> <output_file> <min segment size> <'--step' - optional: step mode>\n";
        return 0;
    }

    // load arguments
    std::string input_file = argv[1];
    std::string output_file = argv[2];
    int min_segment_size = std::atoi(argv[3]);
    bool step_mode = false;

    // check file
    std::fstream file;
    file.open(input_file, std::ios_base::in);
    if (!file.is_open()){
        std::cout<<"Given file don't exist!\n";
        file.close();
        return 0;
    }
    file.close();

    // check segment min size
    if(min_segment_size<0){
        std::cout<<"Min segment size must be no negative!\n";
        return 0;
    }

    // check if program run in step mode
    if(argc == 5 && std::strcmp(argv[4], "--step") == 0){
        step_mode = true;
    }

    // proccess image
    proccessImage(input_file, output_file, min_segment_size, step_mode);

    // print the bluest quote ever
    std::cout<<BLUEST_QUOTE<<std::endl;

    return 0;
}

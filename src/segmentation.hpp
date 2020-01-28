/**
  * Module that contains function design to find image segments.
  */

#ifndef SEGMENTATION_HPP
#define SEGMENTATION_HPP

// std
#include<list>
#include<utility>
#include<vector>
#include<queue>
#include<limits>

// lego
#include "utils.hpp"

using PixelPos = std::pair<unsigned int, unsigned int>;

/**
 * @brief The Segment struct - it describe segment at image.
 * Segment pixels are save to list. Each segment have its own ID.
 */
struct Segment{
    std::list<PixelPos> pixels;
    unsigned int id;
};

/**
 * @brief findSegments Find segments in given pixels map using simple floodfill variant.
 * @param pixels Map of chosen pixels.
 * @return Vector of segments.
 */
std::vector<Segment> findSegments(const PixelsMap& pixels){
    std::vector<Segment> result;
    std::vector<std::vector<unsigned int>> segmentsMatrix;

    unsigned int currentSegmentID = 0;

    for(auto k : pixels){
        segmentsMatrix.emplace_back(std::vector<unsigned int>());
        for( auto j : k){
            segmentsMatrix.back().emplace_back(0);
        }
    }

    unsigned int maxHeight = pixels.size();
    unsigned int maxWidth = pixels[0].size();

    for (unsigned int row = 0; row<pixels.size(); ++row){
        for (unsigned int col = 0; col < pixels[row].size(); ++col){
            // if pixel was picked and don't belong to any segment start flood fill
            if(pixels[row][col] && segmentsMatrix[row][col] == 0 ){
                ++currentSegmentID;

                std::list<PixelPos> segmentPixels;
                std::queue<PixelPos> pixelQueue;

                // three operations:
                // - push to queue
                // - set as a current segment ID in segmentsMatrix
                // - push to segmentPixels
                pixelQueue.push(std::make_pair(row, col));

                while(!pixelQueue.empty()){
                    PixelPos current = pixelQueue.front();
                    pixelQueue.pop();


                    if(pixels[current.first][current.second] && segmentsMatrix[current.first][current.second] == 0){
                        // two operations:
                        // - set as a current segment ID in segmentsMatrix
                        // - push to segmentPixels
                        // DON'T NEED TO CHECK IF ALREADY USED - IT WAS DONE BEFORE ADDING!
                        segmentsMatrix[current.first][current.second] = currentSegmentID;
                        segmentPixels.emplace_back(std::make_pair(current.first, current.second));
                    } else {
                        continue;
                    }

                    // left
                    if(current.first > 0){
                        if(pixels[current.first-1][current.second] && segmentsMatrix[current.first-1][current.second] == 0){
                            pixelQueue.emplace(std::make_pair(current.first-1, current.second));
                        }
                    }
                    // right
                    if(current.first + 1 < maxHeight ){
                        if(pixels[current.first+1][current.second] && segmentsMatrix[current.first+1][current.second] == 0){
                            pixelQueue.emplace(std::make_pair(current.first+1, current.second));
                        }
                    }
                    // bottom
                    if(current.second > 0){
                        if(pixels[current.first][current.second-1] && segmentsMatrix[current.first][current.second-1] == 0){
                            pixelQueue.emplace(std::make_pair(current.first, current.second-1));
                        }
                    }
                    // top
                    if(current.second + 1 < maxWidth ){
                        if(pixels[current.first][current.second+1] && segmentsMatrix[current.first][current.second+1] == 0){
                            pixelQueue.emplace(std::make_pair(current.first, current.second+1));
                        }
                    }
                }

                Segment resultSeg;
                resultSeg.pixels = segmentPixels;
                resultSeg.id = currentSegmentID;
                result.emplace_back(resultSeg);
            }
        }
    }

    return result;
};

/**
 * @brief colorSegmentsWithRandomColor Take random color for each segment and color with it segment pixels.
 * @param img Image in which will be placed segment pixels.
 * @param segments Vector of segments.
 */
void colorSegmentsWithRandomColor(cv::Mat& img, const std::vector<Segment>& segments){
    cv::Mat_<cv::Vec3b> iter = img;

    for(auto seg : segments){
        // generate ranodm color
        uint8_t b = rand()%std::numeric_limits<uint8_t>::max();
        uint8_t g = rand()%std::numeric_limits<uint8_t>::max();
        uint8_t r = rand()%std::numeric_limits<uint8_t>::max();

        // color segment pixels
        for(auto pos : seg.pixels){
            iter(pos.first, pos.second)[0] = b;
            iter(pos.first, pos.second)[1] = g;
            iter(pos.first, pos.second)[2] = r;
        }
    }
}

/**
 * @brief removeAdditionalSegments Simple function that removes big and small segments.
 * @param min_size Minimal size of segment.
 * @param max_size Maximal size of segment.
 * @param original Vector of segments to chose.
 * @return Vector with subset of segments from original.
 */
std::vector<Segment> removeAdditionalSegments(int min_size, const std::vector<Segment>& original){
    std::vector<Segment> result;

    for (auto s : original){
        if(s.pixels.size()>min_size){
            result.push_back(s);
        }
    }

    return result;
}

/**
 * @brief segmentBoundingRectPoints Find points that describe segment - least x,
 * most x, least y, most y.
 * @param segment Segment for which will be found points.
 * @return Vector of points listed above.
 */
std::vector<unsigned int> segmentBoundingRectPoints(const Segment& segment) {
    unsigned int most_x= 0;
    unsigned int least_x = std::numeric_limits<unsigned int>::max();
    unsigned int most_y = 0;
    unsigned int least_y = std::numeric_limits<unsigned int>::max();

    for (auto pix : segment.pixels){
        if (pix.first > most_x) {
            most_x = pix.first;
        }
        if (pix.first < least_x) {
            least_x = pix.first;
        }
        if (pix.second > most_y) {
            most_y = pix.second;
        }
        if (pix.second < least_y) {
            least_y = pix.second;
        }
    }

    return { least_x, most_x, least_y, most_y };
}

/**
 * @brief drawBoundingRectForSegment Draw segment bounding rectange in given image.
 * @param img Image in which will be draw bounding rectangle.
 * @param segment Segment to draw in image.
 */
void drawBoundingRectForSegment(cv::Mat& img, const Segment& segment){
    // take bounding points
    std::vector<unsigned int> points = segmentBoundingRectPoints(segment);
    std::pair<unsigned int, unsigned int> top_left = std::make_pair(points[0], points[2]);
    std::pair<unsigned int, unsigned int> top_right = std::make_pair(points[1], points[2]);
    std::pair<unsigned int, unsigned int> bottom_left = std::make_pair(points[0], points[3]);
    std::pair<unsigned int, unsigned int> bottom_right = std::make_pair(points[1], points[3]);

    cv::Mat_<cv::Vec3b> iter = img;

    // draw leftline
    for(unsigned int i=top_left.first; i<=top_right.first; ++i){
        iter(i, top_left.second)[0] = 0;
        iter(i, top_left.second)[1] = 0;
        iter(i, top_left.second)[2] = 255;
        iter(i, top_left.second+1)[0] = 0;
        iter(i, top_left.second+1)[1] = 0;
        iter(i, top_left.second+1)[2] = 255;
    }

    // draw right line
    for(unsigned int i=bottom_left.first; i<=bottom_right.first; ++i){
        iter(i, bottom_left.second)[0] = 0;
        iter(i, bottom_left.second)[1] = 0;
        iter(i, bottom_left.second)[2] = 255;
        iter(i, bottom_left.second-1)[0] = 0;
        iter(i, bottom_left.second-1)[1] = 0;
        iter(i, bottom_left.second-1)[2] = 255;
    }

    // draw top line
    for(unsigned int i=top_left.second; i<=bottom_left.second; ++i){
        iter(bottom_left.first, i)[0] = 0;
        iter(bottom_left.first, i)[1] = 0;
        iter(bottom_left.first, i)[2] = 255;
        iter(bottom_left.first+1, i)[0] = 0;
        iter(bottom_left.first+1, i)[1] = 0;
        iter(bottom_left.first+1, i)[2] = 255;
    }

    // draw bottom line
    for(unsigned int i=top_right.second; i<=bottom_right.second; ++i){
        iter(top_right.first, i)[0] = 0;
        iter(top_right.first, i)[1] = 0;
        iter(top_right.first-1, i)[2] = 255;
        iter(top_right.first-1, i)[0] = 0;
        iter(top_right.first-1, i)[1] = 0;
        iter(top_right.first-1, i)[2] = 255;
    }

}



#endif // SEGMENTATION_HPP

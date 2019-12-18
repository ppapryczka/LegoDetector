// catch2
#include "catch2.hpp"

// lego
#include "../src/utils.hpp"

// std
#include<vector>

TEST_CASE("Tests for rank filter", "[utils][rank_filter]"){

}

TEST_CASE("Tests for RGB to HSV functions", "[utils][cvtColorRGBToHSV]"){
    SECTION("convert to HSV with own scale pure green color"){
        std::vector<uint8_t> color = {0, 255, 0};
        std::vector<uint8_t> cvt_result = cvtColorRGBToHSVOwnScale(color[0], color[1], color[2]);
        std::vector<uint8_t> expected_result = {60, 100, 100};

        REQUIRE(cvt_result == expected_result);
    }

    SECTION("convert to HSV with own scale pure red color"){
        std::vector<uint8_t> color = {255, 0, 0};
        std::vector<uint8_t> cvt_result = cvtColorRGBToHSVOwnScale(color[0], color[1], color[2]);
        std::vector<uint8_t> expected_result = {0, 100, 100};

        REQUIRE(cvt_result == expected_result);
    }


    SECTION("convert to HSV with own scale pure blue color"){
        std::vector<uint8_t> color = {0, 0, 255};
        std::vector<uint8_t> cvt_result = cvtColorRGBToHSVOwnScale(color[0], color[1], color[2]);
        std::vector<uint8_t> expected_result = {120, 100, 100};

        REQUIRE(cvt_result == expected_result);
    }

}

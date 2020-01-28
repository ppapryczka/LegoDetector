/**
  * Functions to deal with segment moments.
  */

#ifndef MOMENTS_HPP
#define MOMENTS_HPP

// std
#include<map>
#include<string>

// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>

// lego
#include"segmentation.hpp"

using Moments = std::map<std::string, double>;


/**
 * @brief getMoments Count moments for given segment.
 * @param seg Segment for which will be count moments.
 * @return Map of moments - key: name, value: moment value.
 */
Moments getMoments(const Segment& seg)
{
    Moments moments;

    double m00 = 0.0, m01 = 0.0, m10 = 0.0, m11 = 0.0, m20 = 0.0, m02 = 0.0, m21 = 0.0, m12 = 0.0, m30 = 0.0, m03 = 0.0;

    for (auto pix : seg.pixels){
        m00 += 1.0;
        m10 += static_cast<double>(pix.second);
        m01 += static_cast<double>(pix.first);
        m11 += static_cast<double>(pix.first) * static_cast<double>(pix.second);
        m20 += std::pow(static_cast<double>(pix.second), 2.0);
        m02 += std::pow(static_cast<double>(pix.first), 2.0);
        m21 += std::pow(static_cast<double>(pix.second), 2.0) * static_cast<double>(pix.first);
        m12 += std::pow(static_cast<double>(pix.first), 2.0) * static_cast<double>(pix.second);
        m30 += std::pow(static_cast<double>(pix.second), 3.0);
        m03 += std::pow(static_cast<double>(pix.first), 3.0);
    }

    double xCent = m10 / m00, yCent = m01 / m00;
    double M00 = m00, M01 = 0.0, M10 = 0.0;
    double M11 = m11 - m10 * m01 / m00;
    double M20 = m20 - std::pow(m10, 2.0) / m00;
    double M02 = m02 - std::pow(m01, 2.0) / m00;
    double M21 = m21 - 2.0 * m11 * xCent - m20 * yCent + 2.0 * m01 * std::pow(xCent, 2.0);
    double M12 = m12 - 2.0 * m11 * yCent - m02 * xCent + 2.0 * m10 * std::pow(yCent, 2.0);
    double M30 = m30 - 3.0 * m20 * xCent + 2.0 * m10 * std::pow(xCent, 2.0);
    double M03 = m03 - 3.0 * m02 * yCent + 2.0 * m01 * std::pow(yCent, 2.0);

    moments["M1"] = (M20 + M02) / std::pow(m00, 2.0);
    moments["M2"] = (std::pow(M20 - M02, 2.0) + 4.0 * std::pow(M11, 2.0)) / std::pow(m00, 4.0);
    moments["M3"] = (std::pow(M30 - 3.0 * M12, 2.0) + std::pow(3.0 * M21 - M03, 2.0)) / std::pow(m00, 5.0);
    moments["M4"] = (std::pow(M30 + M12, 2.0) + std::pow(M21 + M03, 2.0)) / std::pow(m00, 5.0);
    moments["M5"] = ((M30 - 3.0 * M12) * (M30 + M12) * (std::pow(M30 + M12, 2.0) - 3.0 * std::pow(M21 + M03, 2.0))
        + (3.0 * M21 - M03) * (M21 + M03) * (3.0 * std::pow(M30 + M12, 2.0) - std::pow(M21 + M03, 2.0))) / std::pow(m00, 10.0);
    moments["M6"] = ((M20 - M02) * (std::pow(M30 + M12, 2.0) - std::pow(M21 + M03, 2.0)) + 4.0 * M11 * (M30 + M12) * (M21 + M03)) / std::pow(m00, 7.0);
    moments["M7"] = (M20 * M02 - std::pow(M11, 2.0)) / std::pow(m00, 4.0);
    moments["M8"] = (M30 * M12 + M21 * M03 - std::pow(M12, 2.0) - std::pow(M21, 2.0)) / std::pow(m00, 5.0);
    moments["M9"] = (M20 * (M21 * M03 - std::pow(M12, 2.0)) + M02 * (M03 * M12 - std::pow(M21, 2.0)) - M11 * (M30 * M03 - M21 * M12)) / std::pow(m00, 7.0);
    moments["M10"] = (std::pow(M30 * M03 - M12 * M21, 2.0) - 4.0 * (M30 * M12 - std::pow(M21, 2)) * (M03 * M21 - M12)) / std::pow(m00, 10.0);

    return moments;
}

/**
 * @brief saveMomentsToCSV Simple function that saves moments from given vector to csv file.
 * @param moments Vector fo moments to save.
 * @param csvName Name of csv file.
 */
void saveMomentsToCSV(std::vector<Moments>& moments, std::string csvName = "moments.csv"){
    std::fstream file(csvName, std::ios::out);

    // iterate over vector moments
    for ( auto& m : moments){
        file<<float(m["M1"])<<";";
        file<<float(m["M2"])<<";";
        file<<float(m["M3"])<<";";
        file<<float(m["M4"])<<";";
        file<<float(m["M5"])<<";";
        file<<float(m["M6"])<<";";
        file<<float(m["M7"])<<";";
        file<<float(m["M8"])<<";";
        file<<float(m["M9"])<<";";
        file<<float(m["M10"]);
        file<<"\n";
    }

    file.close();
}

/**
 * @brief isValidSegment Check if given segment is a valid Lego wheel.
 * @param seg Segment which will be check using moments.
 * @return True if it is a wheel like object, false otherwise.
 */
bool isValidSegment(Segment& seg){
    auto moments = getMoments(seg);

    if (moments["M1"] > 0.2 || moments["M1"]< 0.15)
        return false;

    if (moments["M2"] > 0.002)
        return false;

    if (moments["M3"] > 0.001)
        return false;

    if (moments["M7"] > 0.1 || moments["M9"] <-0.01){
        return false;
    }

    if (moments["M8"] > 0.002){
        return false;
    }

    if (moments["M9"] > 0.0005 || moments["M9"] <-0.0005){
        return false;
    }

    return true;
}


#endif // MOMENTS_HPP

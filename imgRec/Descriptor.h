#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class Descriptor {
private:

public:
	enum {
		partialDesLen = 16,
		descriptorLength = partialDesLen * 2, 
	};
	Descriptor(std::vector<cv::Point> & contour);
	cv::Point2d center;
	double data[descriptorLength];
};

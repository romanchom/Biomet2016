#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class Descriptor {
private:

public:
	enum {
		descriptorLength = 32, 
	};
	Descriptor(std::vector<cv::Point> & contour);
	std::vector<cv::Point2d> posOfLength;
	std::vector<double> curvatureOfLength;
	cv::Point2d center;
	double data[descriptorLength];

	void relaxContour(int iterations);
	void drawCurvature(cv::Mat & dst);
	void drawSignature(cv::Mat & dst);
	void drawDescriptor(cv::Mat & dst);
};

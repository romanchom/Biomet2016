#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class ImageFilter {
private:
	cv::VideoCapture cap;

	int xPos[9], yPos[9];
	cv::Vec3b min[9];
	cv::Vec3b max[9];
	cv::Mat frame;
	cv::Mat temp;
	cv::Mat hsvFrame;

public:
	ImageFilter();
	~ImageFilter();
	void sampleTrackedColor();
	void getTrackedContours();
	bool shouldExit();
	std::vector<std::vector<cv::Point>> contours;
};

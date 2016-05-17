#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class ImageFilter {
private:
	cv::VideoCapture cap;

	int xPos[9], yPos[9];
	int hue;
	int sat;
	int val;

	cv::Mat temp;
	cv::Mat hsvFrame;
public:
	cv::Mat frame;

	ImageFilter();
	~ImageFilter();
	void sampleTrackedColor();
	void getTrackedContours();
	bool shouldExit();
	std::vector<std::vector<cv::Point>> contours;
};

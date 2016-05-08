#include "ImageFilter.h"

#include <iostream>
#include <algorithm>

ImageFilter::ImageFilter() :
	cap(0)
{
	cv::namedWindow("preview",1);
	cap >> frame;
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			int index = 3 * i + j;
			xPos[index] = frame.cols * (9 + i) / 20;
			yPos[index] = (frame.rows * 10 + frame.cols * (j - 1)) / 20;
		}
	}
}

ImageFilter::~ImageFilter(){

}

void ImageFilter::sampleTrackedColor(){
	while(true)	{
		cap >> frame;

		cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV_FULL);
		cv::GaussianBlur(hsvFrame, hsvFrame, cv::Size(5, 5), 0);
		size_t w = frame.rows;
		size_t h = frame.cols;
		hue = 0;
		for(int i = 0; i < 9; ++i){
			cv::Vec3b sample = hsvFrame.at<cv::Vec3b>(yPos[i], xPos[i]);
			cv::circle(frame, cv::Point(xPos[i], yPos[i]), 5, cv::Scalar(255, 255, 255));
			hue += sample[0];
		}
		hue /= 9;
		cv::imshow("preview", frame);
		if(cv::waitKey(1) >= 0) {
			break;
		}
	}
}

void ImageFilter::getTrackedContours(){
	cv::Mat edges(frame.rows, frame.cols, CV_8UC1, cv::Scalar(0, 0, 0));
	cv::Mat temp(frame.rows, frame.cols, CV_8UC1, cv::Scalar(0, 0, 0));

	cap >> frame;

	//cv::boxFilter(frame, hsvFrame, -1, cv::Size(3, 3));
	cv::medianBlur(frame, hsvFrame, 5);

	cv::cvtColor(hsvFrame, hsvFrame, cv::COLOR_BGR2HSV_FULL);
	std::vector<cv::Mat> chs;
	split(hsvFrame, chs);

	cv::absdiff(chs[0], hue, temp);

	//cv::GaussianBlur(temp, temp, cv::Size(5, 5), 0);
	cv::medianBlur(temp, temp, 5);

	cv::cvtColor(temp, frame, cv::COLOR_GRAY2BGR);
	//cv::cvtColor(temp, frame, cv::COLOR_GRAY2BGR);
	//cv::divide(temp, chs[1], temp);   // t = diff / sat
	cv::threshold(temp, edges, 10, 255, cv::THRESH_BINARY_INV);
	cv::medianBlur(edges, edges, 5);
	//cv::erode(edges, edges, cv::Mat(), cv::Point(-1, -1), 3);
	//cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);



	cv::Canny(edges, edges, 0, 255, 3);

	cv::cvtColor(edges, hsvFrame, cv::COLOR_GRAY2BGR);

	cv::addWeighted(frame, 1, hsvFrame, 1, 0, frame);

	//cv::imshow("preview", frame);

	std::vector<cv::Vec4i> hierarchy;

	std::vector<cv::Point> * best;
	uint64_t bestLength = 0;

	cv::findContours(edges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
}

bool ImageFilter::shouldExit(){
	return cv::waitKey(1) >= 0;
}

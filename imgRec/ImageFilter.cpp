#include "ImageFilter.h"

#include <iostream>
#include <algorithm>

ImageFilter::ImageFilter() :
	cap(0)
{
	init();
}

ImageFilter::ImageFilter(const std::string & url) :
	cap(url)
{
	init();
}

ImageFilter::~ImageFilter(){

}

void ImageFilter::init(){
	cv::namedWindow("preview",1);

	cap >> frame;
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			int index = 3 * i + j;
			xPos[index] = frame.cols * (19 + i) / 40;
			yPos[index] = (frame.rows * 20 + frame.cols * (j - 1)) / 40;
		}
	}
}

bool ImageFilter::isGood(){
	return cap.isOpened();
}

void ImageFilter::sampleTrackedColor(){
	while(true)	{
		cap >> frame;

		cv::medianBlur(frame, hsvFrame, 7);
		cv::cvtColor(hsvFrame, hsvFrame, cv::COLOR_BGR2HSV_FULL);
		cv::GaussianBlur(hsvFrame, hsvFrame, cv::Size(5, 5), 0);
		size_t w = frame.rows;
		size_t h = frame.cols;
		hue = 0;
		sat = 0;
		val = 0;
		for(int i = 0; i < 9; ++i){
			cv::Vec3b sample = hsvFrame.at<cv::Vec3b>(yPos[i], xPos[i]);
			cv::Vec3b rgbSample = frame.at<cv::Vec3b>(yPos[i], xPos[i]);
			cv::circle(frame, cv::Point(xPos[i], yPos[i]), 5, cv::Scalar(255, 255, 255));
			hue += sample[0];
			sat += sample[1];
			val += sample[2];
		}
		hue /= 9;
		sat /= 9;
		val /= 9;
		cv::imshow("preview", frame);
		if(cv::waitKey(1) >= 0) {
			break;
		}
	}
}

void closeness(cv::Mat & a, int v) {
	size_t c = a.rows * a.cols;
	unsigned char * p = a.data;
	unsigned char * e = p + c;
	do {
		*p = std::max(0, 255 - abs(*p - v) * 3);
		++p;
	} while (p != e);
}

void hueCloseness(cv::Mat & a, int v) {
	size_t c = a.rows * a.cols;
	unsigned char * p = a.data;
	unsigned char * e = p + c;
	do {
		int d = *p;
		d = (unsigned char) (d - v - 128);
		d = std::abs(128 - d);
		d = 255 - d * 2;
		*p = d;
		++p;
	} while (p != e);
}


void ImageFilter::getTrackedContours(){
	cv::Mat edges(frame.rows, frame.cols, CV_8UC1, cv::Scalar(0, 0, 0));

	cap >> frame;
	
	cv::medianBlur(frame, hsvFrame, 3);
	cv::cvtColor(hsvFrame, hsvFrame, cv::COLOR_BGR2HSV_FULL);
	std::vector<cv::Mat> chs;
	split(hsvFrame, chs);

	hueCloseness(chs[0], hue); 
	closeness(chs[1], sat);
	closeness(chs[2], val);
	/*cv::imshow("hue", chs[0]);
	cv::imshow("sat", chs[1]);
	cv::imshow("val", chs[2]);//*/


	cv::multiply(chs[0], chs[1], temp, 1.0 / 255);
	cv::multiply(temp, chs[2], temp, 1.8 / 255);
	cv::cvtColor(temp, frame, cv::COLOR_GRAY2BGR);


	cv::threshold(temp, edges, 128, 255, cv::THRESH_BINARY);
	cv::medianBlur(edges, edges, 3);

	//cv::erode(edges, edges, cv::Mat(), cv::Point(-1, -1), 2);
	//cv::dilate(edges, edges, cv::Mat(), cv::Point(-1, -1), 2);

	cv::Canny(edges, edges, 0, 255, 3);

	cv::cvtColor(edges, hsvFrame, cv::COLOR_GRAY2BGR);

	cv::addWeighted(frame, 1, hsvFrame, 1, 0, frame);

	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(edges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
}



bool ImageFilter::shouldExit(){
	return cv::waitKey(1) >= 0;
}

#include "Descriptor.h"

#include <cmath>
#include <complex>

Descriptor::Descriptor(std::vector<cv::Point> & contour){
	cv::Point2d first = contour[0];
	cv::Point2d second = contour[1];
	double length = 0;
	size_t index = 1;
	do{
		double t = posOfLength.size() - length;
		if(t > 1){
			cv::Point v = first - second;
			double len = sqrt(v.dot(v));
			//if(len != 1) std::cout << "asd" << len;
			length += len;
			first = second;
			index++;
			second = contour[index % contour.size()];
		}else{
			cv::Point2d p = first * t + second * (1 - t);
			posOfLength.push_back(p);
		}
	}while(index <= contour.size());

	relaxContour(10);

	const size_t count = posOfLength.size();
	size_t optimalLength = count;//cv::getOptimalDFTSize(count);

	curvatureOfLength.resize(optimalLength);

	const size_t boxSize = 50;
	for (size_t i = 0; i < count; ++i) {
		size_t indexLow = (i - boxSize + count) % count;
		size_t indexHigh = (i + boxSize + count) % count;

		cv::Point2d low = posOfLength[indexLow];
		cv::Point2d mid = posOfLength[i];
		cv::Point2d hi = posOfLength[indexHigh];

		cv::Point2d diff1 = mid - low;
		cv::Point2d diff2 = hi - mid;
		diff2 = cv::Point2d(diff2.y, -diff2.x);
		double len = diff1.dot(diff1);
		len *= diff2.dot(diff2);
		len = sqrt(len);

		double val = diff1.dot(diff2) / len;
		//val = asin(val);

		curvatureOfLength[i] = val;
	}

	std::vector<double> fourier;

	cv::dft(curvatureOfLength, fourier);
	double sum = 0;
	for(int i = 0; i < descriptorLength; ++i){
		size_t b = 2 * i + 1;
		data[i] = sqrt(fourier[b + 0] * fourier[b + 0] + fourier[b + 1] * fourier[b + 1]) * i;
		sum = std::max(data[i], sum);
	}
	//sum = descriptorLength / sum;
	for(int i = 0; i < descriptorLength; ++i){
		data[i] /= sum;
		//data[i] = log(data[i]);
	}

	/*std::cout << "Fourier Length " << fourier.size() << std::endl;
	std::cin >> a;

	for(int i = 0; i < 16; ++i){
		std::cout << data[i] << ", ";
	}
	std::cout << std::endl;
	//std::cin >> a;*/
}

void Descriptor::relaxContour(int iterations) {
	size_t count = posOfLength.size();
	std::vector<cv::Point2d> newPos(count);

	for (int iter = 0; iter < iterations; ++iter) {
		for (size_t i = 0; i < count; ++i) {
			newPos[i] = (posOfLength[(i - 1u + count) % count] + posOfLength[(i + 1u + count) % count]) / 2;
		}
		posOfLength = newPos;
	}
}


void Descriptor::drawCurvature(cv::Mat & dst) {
	int count = posOfLength.size();
	for (int i = 0; i < count; ++i) {
		double curva = curvatureOfLength[i] * 100;
		cv::Scalar color(0, -curva, curva);
		cv::line(dst, posOfLength[i], posOfLength[(i + 1) % count], color, 2);
	}
}


void Descriptor::drawSignature(cv::Mat & dst) {
	cv::Scalar white(255, 0, 0);
	cv::Point2d ll(10, 60);
	cv::line(dst, ll + cv::Point2d(0, -100), ll, white);
	cv::line(dst, ll + cv::Point2d(200, 0), ll, white);

	double last = curvatureOfLength[0];
	size_t count = curvatureOfLength.size();
	double x = 200.0 / count;
	double y = -30;

	for (int i = 1; i < count; ++i) {
		cv::line(dst, ll + cv::Point2d((i - 1) * x, last * y), ll + cv::Point2d(i * x, curvatureOfLength[i] * y), white);
		last = curvatureOfLength[i];
	}
}

void Descriptor::drawDescriptor(cv::Mat & dst) {
cv::Scalar white(255, 0, 0);
	cv::Point2d ll(10, 210);
	cv::line(dst, ll + cv::Point2d(0, -100), ll, white);
	//cv::line(dst, ll + cv::Point2d(200, 0), ll, white);

	double last = data[0];
	double x = 200.0 / descriptorLength;
	double y = -100;

	for (int i = 1; i < descriptorLength; ++i) {
	cv::line(dst, ll + cv::Point2d((i - 1) * x, last * y), ll + cv::Point2d(i * x, data[i] * y), white);
	last = data[i];
	}
}

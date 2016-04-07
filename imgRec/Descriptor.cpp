#include "Descriptor.h"

#include <cmath>
#include <complex>

Descriptor::Descriptor(std::vector<cv::Point> & contour){
	std::vector<cv::Point2d> posOfLength;
	cv::Point last = contour.front();
	center = last;

	for(auto it = contour.begin() + 1; it != contour.end(); ++it){
		cv::Point diff = *it - last;
		double inc = diff.x * diff.x + diff.y * diff.y;
		inc = 1.0 / ( round(2.0 * sqrt(inc)));
		for(double t = 0; t < 1; t += inc){
			cv::Point2d p = diff;
			p *= t;
			p += cv::Point2d(last);
			posOfLength.push_back(p);
		}
		center += cv::Point2d(*it);
		last = *it;
	}

	center /= (double) contour.size();
	size_t optimalLength = cv::getOptimalDFTSize(posOfLength.size());

	std::vector<double> distOfLength;
	std::vector<double> angleOfLength;

	distOfLength.resize(optimalLength);
	angleOfLength.resize(optimalLength);

	for(int i = 0; i < posOfLength.size(); ++i){
		cv::Point2d diff = center - posOfLength[i];
		double length = sqrt(diff.dot(diff));
		double angle = atan2(diff.x, diff.y);
		distOfLength[i] = length;
		angleOfLength[i] = angle;
	}

	std::vector<double> fourier;

	cv::dft(distOfLength, fourier);
	double sum = 0;
	for(int i = 0; i < partialDesLen; ++i){
		size_t b = 2 * i + 1;
		data[i] = sqrt(fourier[b + 0] * fourier[b + 0] + fourier[b + 1] * fourier[b + 1]);
		sum += data[i];
	}
	sum = partialDesLen / sum;
	for(int i = 0; i < partialDesLen; ++i){
		data[i] *= sum;
		data[i] = log(data[i]);
	}

	sum = 0;
	for(int i = partialDesLen; i < descriptorLength; ++i){
		size_t b = 2 * i + 1;
		data[i] = sqrt(fourier[b + 0] * fourier[b + 0] + fourier[b + 1] * fourier[b + 1]);
		sum += data[i];
	}
	sum = partialDesLen / sum;
	for(int i = partialDesLen; i < descriptorLength; ++i){
		data[i] *= sum;
		data[i] = log(data[i]);
	}


	/*std::cout << "Fourier Length " << fourier.size() << std::endl;
	std::cin >> a;*/

	for(int i = 0; i < 16; ++i){
		std::cout << data[i] << ", ";
	}
	std::cout << std::endl;
	//std::cin >> a;
}

#include <iostream>

#include "ImageFilter.h"

#include "PatternBase.h"
#include "Descriptor.h"

int main()
{
	PatternBase base("patterns.txt", "classes.txt");

	ImageFilter filter;
	filter.sampleTrackedColor();
	do{
		filter.getTrackedContours();
		size_t bestLength = 0;
		std::vector<cv::Point> * best = nullptr;
		for (auto & contour : filter.contours)
		{
			uint64_t length = contour.size();
			if(length > bestLength){
				best = &contour;
				bestLength = length;
			}
		}
		char key = cv::waitKey(1);
		if(key == 'q') break;
		if (best) {
			Descriptor desc(*best);
			desc.drawCurvature(filter.frame);
			desc.drawSignature(filter.frame);
			desc.drawDescriptor(filter.frame);
			if(key == 'a') base.addNew();
			std::cout << base.classifyObject(desc.data, 5.0) << std::endl;
		}
		cv::imshow("preview", filter.frame);
	}while(true);
	return 0;
}

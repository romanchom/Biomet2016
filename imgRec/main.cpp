#include <iostream>

#include "ImageFilter.h"

#include "PatternBase.h"
#include "Descriptor.h"
#include "GestureEngine.h"


int main()
{	
	PatternBase base("patterns.txt", "classes.txt");
	GestureEngine gestureEngine;

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
		if (bestLength > 200) {
			Descriptor desc(*best);
			desc.drawCurvature(filter.frame);
			desc.drawSignature(filter.frame);
			desc.drawDescriptor(filter.frame);
			if(key == 'a') base.addNew();
			std::string reco = base.classifyObject(desc.data, 2);
			std::cout << reco << std::endl;
			gestureEngine.processGesture(reco);
		}
		cv::imshow("preview", filter.frame);
	}while(true);
	return 0;
}

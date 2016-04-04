#include <iostream>
#include <opencv2/opencv.hpp>

#include "PatternBase.h"
/*
struct MyPoint
{
	int x;
	int y;
};

struct MyObject
{
	vector<MyPoint> vPoints;
};

PatternBase patternBase("patterns.txt", "classes.txt");
enum {
	descriptorLength = 16
};

void shapeRecognize(IplImage* img, MyObject object) {
	char txt[32];
	const int N = 128;
	int n;
	int indx;
	double x;
	double y;
	double v;
	double di;
	double cx;
	double cy;
	double max;
	double tab[N];
	double des[descriptorLength];
	CvMat *dft_tab;
	CvFont font;
	// oblicz punkt centralny
	cx = 0.0;
	cy = 0.0;
	n = object.vPoints.size();
	for (int i = 0; i<n; i++)
	{
		cx += object.vPoints[i].x;
		cy += object.vPoints[i].y;
	}
	cx /= (double)n;
	cy /= (double)n;
	cvDrawLine(img, cvPoint((int)(cx - 4), (int)cy), cvPoint((int)(cx + 4), (int)cy), cvScalar(255, 255, 255));
	cvDrawLine(img, cvPoint((int)cx, (int)(cy - 4)), cvPoint((int)cx, (int)(cy + 4)), cvScalar(255, 255, 255));
	// wyznacz sygnaturę
	di = n / (double)N;
	for (int i = 0; i<N; i++)
	{
		indx = i*di;
		if (indx>n - 1)
		{
			indx = n - 1;
		}
		x = object.vPoints[indx].x;
		y = object.vPoints[indx].y;
		tab[i] = sqrt((x - cx)*(x - cx) + (y - cy)*(y - cy));
	}
	// rysuj sygnaturę
	for (int i = 1; i<N; i++)
	{
		cvDrawLine(img, cvPoint(10 + i - 1, (int)(10.0 + tab[i - 1])), cvPoint(10 + i, 10 + tab[i]), cvScalar(255, 255, 255));
	}
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5);
	cvPutText(img, "sygnatura", cvPoint(40, 130), &font, cvScalar(255, 255, 255));
	// oblicz deskryptory fourierowskie
	dft_tab = cvCreateMat(1, N, CV_64FC2);
	for (int i = 0; i<N; i++)
	{
		dft_tab->data.db[2 * i + 0] = tab[i];
		dft_tab->data.db[2 * i + 1] = 0.0;
	}
	cvDFT(dft_tab, dft_tab, cv::DFT_SCALE);
	// normalizacja dla uzyskania inwariancji na zmianę skali i obrót
	max = 0.0;
	des[0] = 0.0;
	for (int i = 1; i < descriptorLength; i++)
	{
		x = dft_tab->data.db[2 * i + 0];
		y = dft_tab->data.db[2 * i + 1];
		v = sqrt(x*x + y*y);
		des[i] = v;
		if (des[i]>max)
		{
			max = des[i];
		}
	}
	// normalize
	if (max>0.0)
	{
		for (int i = 0; i < descriptorLength; i++)
		{
			des[i] /= max;
		}
	}
	// draw descriptor
	for (int i = 0; i < descriptorLength; i++)
	{
		v = 100.0*des[i];
		cvDrawRect(img, cvPoint(200 + 8 * i, 110), cvPoint(200 + 8 * (i + 1), 110 - v), cvScalar(255, 255, 255));
	}
	cvPutText(img, "deskryptor", cvPoint(200, 130), &font, cvScalar(255, 255, 255));
	// klasyfikacja
	std::string objectClass = patternBase.classifyObject(des);

	sprintf(txt, "Rozpoznano: %s", objectClass.c_str());
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.9, 0.9);
	cvPutText(img, txt, cvPoint(320, 100), &font, cvScalar(255, 255, 255));

	cvReleaseData(dft_tab);
}

void analyzeImage(IplImage* img, unsigned int nThreshold)
{
	CvPoint* pa;
	MyPoint point;
	MyObject object;
	CvSeq* contours, *ca, *best = 0;
	CvMemStorage* store;
	uint64_t bestLength = 0;
	store = cvCreateMemStorage(1048576);
	contours = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), store);
	cvFindContours(img, store, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	ca = contours;
	cvSet(img, CV_RGB(0, 0, 0));
	for (; ca; ca = ca->h_next)
	{
		if (CV_IS_SEQ_CLOSED(ca))
		{
			uint64_t length = ca->total;
			if(length > bestLength){
				best = ca;
				bestLength = length;
			}
		}
	}

	if (best != 0)
	{
		cvDrawContours(img, best, cvScalar(255, 255, 255), cvScalar(0, 0, 0), 0);
		object.vPoints.clear();
		for (int i = 0; i<best->total; i++)
		{
			pa = (CvPoint*)CV_GET_SEQ_ELEM(CvPoint, best, i);
			point.x = pa->x;
			point.y = pa->y;
			object.vPoints.push_back(point);
		}
		shapeRecognize(img, object);
	}
	cvReleaseMemStorage(&store);
}

void processImage(IplImage* img, unsigned int minH, unsigned int maxH, unsigned int minS, unsigned int minV)
{
	unsigned char h;
	unsigned char s;
	unsigned char v;
	unsigned char* pDataHSV;
	unsigned char* pDataMSK;
	unsigned int indx;
	unsigned int width;
	unsigned int height;
	IplImage* pImgHSV;
	IplImage* pImgMSK;
	width = img->width;
	height = img->height;
	// filtracja wstępna
	cvSmooth(img, img, CV_GAUSSIAN, 7);
	// konwersja RGB do HSV
	pImgHSV = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	pImgMSK = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	cvCvtColor(img, pImgHSV, CV_RGB2HSV);
	// segementacja
	pDataHSV = (unsigned char*)(pImgHSV->imageData);
	pDataMSK = (unsigned char*)(pImgMSK->imageData);
	for (unsigned int i = 0; i<width; i++)
	{
		for (unsigned int j = 0; j<height; j++)
		{
			indx = 3 * i + j*pImgHSV->widthStep;
			h = pDataHSV[indx + 0];
			s = pDataHSV[indx + 1];
			v = pDataHSV[indx + 2];
			//if (h >= minH && h <= maxH && s >= minS && v >= minV)
			if (h > 0 && h < 50 && s > 128 && v > 45)
			{
				pDataMSK[i + j*pImgMSK->widthStep] = 255;
			} else
			{
				pDataMSK[i + j*pImgMSK->widthStep] = 0;
			}
		}
	}
	// filtracja po segmentacji
	cvSmooth(pImgMSK, pImgMSK, CV_MEDIAN, 3);
	cvDilate(pImgMSK, pImgMSK, 0, 2);
	cvErode(pImgMSK, pImgMSK, 0, 1);
	cvCanny(pImgMSK, pImgMSK, 0, 255, 3);
	// analiza obrazu
	analyzeImage(pImgMSK, 200);
	// wyswietlenie obrazu i sprzatanie
	cvShowImage("Rozpoznawanie", pImgMSK);
	cvReleaseImage(&pImgHSV);
	cvReleaseImage(&pImgMSK);
}

void MouseCallback(int event, int x, int, int, void*)
{
	if (event == CV_EVENT_LBUTTONDOWN){
		patternBase.addNew();
	}
}

void grabFromCamera(int cameraId)
{
	IplImage* pImage;
	CvCapture* capturedDevice;
	capturedDevice = cvCaptureFromCAM(CV_CAP_ANY);
	if (capturedDevice != 0)
	{
		cvNamedWindow("Wejscie", 1);
		cvNamedWindow("Rozpoznawanie", 1);
		cvSetMouseCallback("Rozpoznawanie", MouseCallback, NULL);
		do
		{
			pImage = cvQueryFrame(capturedDevice);
			cvShowImage("Wejscie", pImage);
			processImage(pImage, minHue, maxHue, 20, 50);
		} while (cvWaitKey(20) == -1);
		cvReleaseCapture(&capturedDevice);
		cvDestroyWindow("Wejscie");
		cvDestroyWindow("Rozpoznawanie");
	} else
	{
		std::cout << "Problem z otwarciem dojscia do kamery." << std::endl;
	}
}


void selectHue()
{
	unsigned int indx;
	unsigned char* pData;
	IplImage* pImgHSV;
	pImgHSV = cvCreateImage(cvSize(540, 256), IPL_DEPTH_8U, 3);
	pData = (unsigned char*)(pImgHSV->imageData);
	for (unsigned int i = 0; i<540; i++)
	{
		for (unsigned int j = 0; j<256; j++)
		{
			indx = 3 * i + j*pImgHSV->widthStep;
			pData[indx + 0] = i / 3;
			pData[indx + 1] = 255;
			pData[indx + 2] = 200;
		}
	}
	cvCvtColor(pImgHSV, pImgHSV, CV_HSV2RGB);
	cvNamedWindow("Wybor HSV", 1);
	cvSetMouseCallback("Wybor HSV", MouseCallback, NULL);
	cvShowImage("Wybor HSV", pImgHSV);
	cvWaitKey(0);
	cvDestroyWindow("Wybor HSV");
	cvReleaseImage(&pImgHSV);
}

*/
int main()
{
	cv::VideoCapture cap(0);
	if(!cap.isOpened())
		return -1;
	cv::Mat temp;
	cv::Mat edges;
	cv::Mat frame;
	cv::namedWindow("preview",1);
	cv::Vec3b min[9];
	cv::Vec3b max[9];
	while(true)	{
		cap >> frame;
		cv::imshow("preview", frame);
		if(cv::waitKey(1) >= 0) {
			cv::medianBlur(frame, frame, 25);
			size_t w = frame.rows;
			size_t h = frame.cols;
			int i = 0;
			for(double x = w * 4 / 10; x < w * 6 / 10; x += w / 10){
				for(double y = h * 4 / 10; y < h * 6 / 10; y += h / 10){
					cv::Vec3b sample = frame.at<cv::Vec3b>(x, y);
					int tol = 25;
					min[i] = sample - cv::Vec3b(tol, tol, tol);
					max[i++] = sample + cv::Vec3b(tol, tol, tol);
				}
			}
			break;
		}
	}
	while(true){
		cap >> frame;
		edges = cv::Scalar(0, 0, 0);
		for(int i = 0; i < 9; ++i){
			cv::inRange(frame, min[i], max[i], temp);
			cv::bitwise_or(temp, edges, edges);
		}
		cv::imshow("preview", edges);
		if(cv::waitKey(1) >= 0) break;

	}
	return 0;
}

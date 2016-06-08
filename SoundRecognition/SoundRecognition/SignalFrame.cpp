#include "stdafx.h"
#include "SignalFrame.h"


const float SignalFrame::PreemphasisConst = 0.97;
const float SignalFrame::HammingAlpha = 0.53836;
const float SignalFrame::HammingBeta = 0.46164;
const float SignalFrame::LogYpsilon = 2;
const float SignalFrame::frequency = 41000.0f;


SignalFrame::SignalFrame()
{
	fftCfg = kiss_fft_alloc(windowSize, 0, NULL, NULL);
	fftIn.resize(windowSize);
	fftOut.resize(windowSize);
	helpVector.resize(windowSize);
	hammingWindow.resize(windowSize);
	amplitudeSpectrumVector.resize(K);
	dctCache.resize(MFCCParametersCount * K);


	for (int i = 0; i < windowSize; ++i) {
		fftIn[i].i = 0;
	}
	for (int k = 1; k <= K; ++k) {
		filtersVector.emplace_back(k, d);
	}
	for (int i = 0; i < windowSize; ++i) {
		hammingWindow[i] = HammingAlpha - (HammingBeta * cos((M_PI * 2 * i) / (windowSize - 1)));
	}
	for (int n = 0; n < MFCCParametersCount; ++n)
	{
		for (int k = 0; k < K; ++k)
		{
			dctCache[n * K + k] = std::cos(M_PI * ((2 * k + 1) * n) / (2 * K));
		}
	}
}

SignalFrame::~SignalFrame()
{
	kiss_fft_free(fftCfg);
}

inline float cmplxAbs(const kiss_fft_cpx & cmplx) {
	return std::sqrt(cmplx.r * cmplx.r + cmplx.i * cmplx.i);
}

SignalFrame::MFCC_t SignalFrame::CreateMFCCVector(const int16_t * data) {
	for (int i = 0; i < windowSize; ++i) {
		fftIn[i].r = data[i] * hammingWindow[i];
	}
	kiss_fft(fftCfg, fftIn.data(), fftOut.data());

	for (int j = 0; j < windowSize; ++j)
	{
		helpVector[j] = cmplxAbs(fftOut[j]);
	}

	for (int k = 0; k < K; ++k)
	{
		float v = 0;
		Filter & f = filtersVector[k];
		for (int i = 0; i <= windowSize / 2; ++i)
		{
			float filterArg = (frequency / windowSize) * i;
			v += helpVector[i] * f(filterArg);
		}

		amplitudeSpectrumVector[k] = (std::pow(std::log(v), LogYpsilon));
	}

	MFCC_t ret;

	//std::cout << "MFCC: ";

	for (int n = 0; n < MFCCParametersCount; ++n)
	{
		float kos = 0;
		for (int k = 0; k < K; ++k)
		{
			kos += amplitudeSpectrumVector[k] * dctCache[n * K + k];
		}
		ret[n] = kos;
		//std::cout << kos << ", ";

	}
	//std::cout << std::endl;

	return ret;
}
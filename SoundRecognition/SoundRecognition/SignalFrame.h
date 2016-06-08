#pragma once

#include <array>

#include "AudioClip.h"

#include "kiss_fft.h"

struct Filter
{

private:
	float lk;
	float ck;
	float rk;
	float uFunction(float m)
	{
		return 700 * (pow(10, m / 2595) - 1);
	}

	float c(int k, int d)
	{
		float arg = k * d;
		return uFunction(arg);
	}

	float l(int k, int d)
	{
		float arg = (k - 1) * d;
		return uFunction(arg);
	}

	float r(int k, int d)
	{
		float arg = (k + 1) * d;
		return uFunction(arg);
	}

public:
	Filter(int k, int d) :
		lk(l(k, d)),
		ck(c(k, d)),
		rk(r(k, d))
	{}

	float operator()(float f)
	{
		if (f >= lk && f <= ck)
		{
			return (f - lk) / (ck - lk);
		}
		else if (f >= ck && f <= rk)
		{
			return (rk - f) / (rk - ck);
		}
		else
		{
			return 0;
		}
	}
};

class SignalFrame
{
private:
	static const float PreemphasisConst;

	static const float HammingAlpha;
	static const float HammingBeta;

	//constants for filterings

	static const float LogYpsilon;
	static const float frequency;

	kiss_fft_cfg fftCfg;
	std::vector<Filter> filtersVector;
	
	std::vector<kiss_fft_cpx> fftIn;
	std::vector<kiss_fft_cpx> fftOut;
	std::vector<float> helpVector;
	
	std::vector<float> amplitudeSpectrumVector;
	std::vector<float> hammingWindow;
	std::vector<float> dctCache;
public:
	enum {
		MFCCParametersCount = 13,
		K = 30,
		d = 100,
		windowSize = 1024,
	};
public:
	typedef std::array<float, MFCCParametersCount> MFCC_t;
	SignalFrame();
	~SignalFrame();

	MFCC_t CreateMFCCVector(const int16_t * data);
	void PrintMFCCCoefficients();

private:
	void PrintEachStep(char* title, std::vector<float>& vector);

	void HammingWindowing();
	void FFT();
	void CreateFilters();
	void Filtering();
	void EnergyLogarithm();
	void DCT();

};


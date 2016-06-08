#pragma once
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

	float filterFunction(float f)
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
	const int MFCCParametersCount = 13;

	const float PreemphasisConst = 0.97;

	const float HammingAlpha = 0.53836;
	const float HammingBeta = 0.46164;

	//constants for filterings
	const int K = 30;
	const int d = 100;

	const float LogYpsilon = 2;
	const double Euler = 2.71828182845904523536;

public:
	int size = 1024;

	std::vector<float> preemphasisVector;
	std::vector<Filter> filtersVector;
	std::vector<float> amplitudeSpectrumVector;

	std::vector<float> helpVector;
	std::vector<float> mfccCoefficients;

public:
	SignalFrame();
	~SignalFrame();

	void CreateMFCCVector(AudioClip* ac);
	void PrintMFCCCoefficients();

private:
	void PrintEachStep(char* title, std::vector<float>& vector);
	float ComplexAbs(float&, float&);

	void Preemphasis(AudioClip* ac);
	void HammingWindowing();
	void FFT();
	void CreateFilters();
	void Filtering();
	void EnergyLogarithm();
	void DCT();
};


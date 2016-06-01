#pragma once
#include "AudioClip.h"

struct Filter
{
public:
	std::vector<float> values;

private:
	float uFunction(int& m)
	{
		return 700 * (pow(10, m / 2595) - 1);
	}

	float c(int& k, int& d)
	{
		int arg = k * d;
		return uFunction(arg);
	}

	float l(int& k, int& d)
	{
		int arg = (k - 1) * d;
		return uFunction(arg);
	}

	float r(int& k, int& d)
	{
		int arg = (k + 1) * d;
		return uFunction(arg);
	}

public:
	float filterFunction(int& f, int& k, int d)
	{
		float ck = c(k, d);
		float lk = l(k, d);
		float rk = r(k, d);

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

class MFCC
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

protected:
	int frameLength = 10;

	std::vector<float> preemphasisVector;
	std::vector<Filter> filtersVector;
	std::vector<float> amplitudeSpectrumVector;

	std::vector<float> helpVector;
	std::vector<float> mfccVector;
	//std::vector<SignalFrame> mfccVector;

public:
	MFCC();
	~MFCC();

	void CreateMFCCVector(AudioClip* ac);

private:
	void Preemphasis(AudioClip* ac);
	void HammingWindowing();
	void FFT();
	void CreateFilters();
	void Filtering();
	void EnergyLogarithm();
	void DCT();

};


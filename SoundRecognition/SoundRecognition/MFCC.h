#pragma once
#include "AudioClip.h"

class MFCC
{
private:
	const float PreemphasisConst = 0.97;

	const float HammingAlpha = 0.53836;
	const float HammingBeta = 0.46164;

protected:
	int frameLength = 1;

	std::vector<float> preemphasisVector;
	std::vector<float> mfccVector;

public:
	MFCC();
	~MFCC();

	void CreateMFCCVector(AudioClip* ac);

private:
	void Preemphasis(AudioClip* ac);
	void HammingWindowing();
	void FFT();
	void Filtering();
	void EnergyLogarithm();
	void DCT();

};


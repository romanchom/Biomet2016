#pragma once
#include "AudioClip.h"
#include "SignalFrame.h"

class MFCC
{
public:
	std::vector<SignalFrame> framedSignal;
	std::vector<std::vector<float>> mfcc;

public:
	MFCC();
	~MFCC();

	void CreateMFCCCoefficients(AudioClip* ac);
	void SignalFraming(AudioClip*);

};


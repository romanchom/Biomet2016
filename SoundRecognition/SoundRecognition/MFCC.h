#pragma once
#include "AudioClip.h"
#include "SignalFrame.h"

class MFCC
{
protected:
	std::vector<SignalFrame> framedSignal;

public:
	MFCC();
	~MFCC();

	void CreateMFCCCoefficients(AudioClip* ac);
	void SignalFraming(AudioClip*);

};


#pragma once

#include <array>

#include "AudioClip.h"
#include "SignalFrame.h"



double euclideanDistance(const SignalFrame::MFCC_t & a, const SignalFrame::MFCC_t & b);

class MFCC
{
public:
	std::vector<SignalFrame::MFCC_t> mfcc;

public:
	MFCC();
	~MFCC();

	void CreateMFCCCoefficients(AudioClip* ac);
};


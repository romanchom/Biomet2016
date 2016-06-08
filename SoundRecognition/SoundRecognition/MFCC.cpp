#include "stdafx.h"
#include "MFCC.h"

#include <chrono>

double euclideanDistance(const SignalFrame::MFCC_t & a, const SignalFrame::MFCC_t & b)
{
	float out = 0;
	for (int i = 0; i < a.size(); ++i)
	{
		float d = a[i] - b[i];
		out += d * d;
	}

	return std::sqrt(out);
}

MFCC::MFCC()
{

}

MFCC::~MFCC()
{
}

void MFCC::CreateMFCCCoefficients(AudioClip* ac)
{
	unsigned int size = ac->GetDataBufferPtr()->size();

	std::chrono::high_resolution_clock time;

	auto start = time.now();

	if (size != 0)
	{
		SignalFrame sf;
		int count = ac->GetDataBufferPtr()->size() / 512 - 1;

		for (int i = 0; i < count; ++i)
		{
			int sampleStart = i * 512;
			
			mfcc.push_back(sf.CreateMFCCVector(&ac->GetDataBufferPtr()->at(sampleStart)));
		}
		auto end = time.now();

		std::cout << "Created " << mfcc.size() << " frames" << std::endl;
		std::chrono::duration<double> diff = end - start;
		std::cout << "Time : " << diff.count() << " s" << std::endl;
		std::cout << "speed : " << ac->GetDataBufferPtr()->size() / 41000.0f / diff.count() << " s/s" << std::endl;
	}
	else
	{
		std::cout << "Signal is empty" << std::endl;
	}
}
/*
void MFCC::SignalFraming(AudioClip* ac)
{
	for (int i = 0; i < ac->GetDataBufferPtr()->size(); i += 512)
	{
		SignalFrame sf;

		if (i + sf.windowSize > ac->GetDataBufferPtr()->size())
		{
			break;
		}

		for (int j = 0; j < sf.windowSize; ++j)
		{
			sf.helpVector.push_back(ac->GetDataBufferPtr()->at(i + j));
		}

		framedSignal.push_back(sf);
	}

	std::cout << "Created " << framedSignal.size() << " frames" << std::endl;
}
*/
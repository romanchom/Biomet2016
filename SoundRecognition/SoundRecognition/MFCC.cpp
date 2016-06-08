#include "stdafx.h"
#include "MFCC.h"


MFCC::MFCC()
{

}

MFCC::~MFCC()
{
}

void MFCC::CreateMFCCCoefficients(AudioClip* ac)
{
	unsigned int size = ac->GetDataBufferPtr()->size();

	if (size != 0)
	{
		SignalFraming(ac);
		for (int i = 0; i < framedSignal.size(); ++i)
		{
			framedSignal[i].CreateMFCCVector(ac);
			//framedSignal[i].PrintMFCCCoefficients();
			mfcc.push_back(framedSignal[i].mfccCoefficients);
		}
	}
	else
	{
		std::cout << "Signal is empty" << std::endl;
	}
}

void MFCC::SignalFraming(AudioClip* ac)
{
	for (int i = 0; i < ac->GetDataBufferPtr()->size(); i += 512)
	{
		SignalFrame sf;

		if (i + sf.size > ac->GetDataBufferPtr()->size())
		{
			break;
		}

		for (int j = 0; j < sf.size; ++j)
		{
			sf.helpVector.push_back(ac->GetDataBufferPtr()->at(i + j));
		}

		framedSignal.push_back(sf);
	}

	std::cout << "Created " << framedSignal.size() << " frames" << std::endl;
}

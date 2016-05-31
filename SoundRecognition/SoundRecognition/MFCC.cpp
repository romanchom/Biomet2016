#include "stdafx.h"
#include "MFCC.h"

#include "kiss_fft.h"

MFCC::MFCC()
{
}

MFCC::~MFCC()
{
}

void MFCC::CreateMFCCVector(AudioClip* ac)
{
	Preemphasis(ac);
	HammingWindowing();
	FFT();
	Filtering();
	EnergyLogarithm();
	DCT();
}

void MFCC::Preemphasis(AudioClip* ac)
{
	preemphasisVector.push_back( (float)ac->GetDataBufferPtr()->at(0) );
	for (int i = 1; i < ac->GetDataBufferPtr()->size(); ++i)
	{
		preemphasisVector.push_back( (float) ac->GetDataBufferPtr()->at(i) - (PreemphasisConst * ac->GetDataBufferPtr()->at(i - 1)) );
	}
	std::cout << "MFCC: Preemphasis completed" << std::endl;
}

void MFCC::HammingWindowing()
{
	do
	{
		frameLength++;											// here goes infinite loop
	} while (preemphasisVector.size() % frameLength != 0);

	for (int i = 0; i < preemphasisVector.size() / frameLength; ++i)
	{
		for (int j = 0; j < frameLength; ++j)
		{
			preemphasisVector[(frameLength * i) + j] *= HammingAlpha - (HammingBeta * cos(2 * 3.14 * j) / frameLength - 1);
		}
	}
	std::cout << "MFCC: Hamming Windowing completed" << std::endl;
}

void MFCC::FFT()
{
	kiss_fft_cfg fftCfg = kiss_fft_alloc(1024, 0, NULL, NULL);

	kiss_fft_free(fftCfg);
}

void MFCC::Filtering()
{
}

void MFCC::EnergyLogarithm()
{
}

void MFCC::DCT()
{
}

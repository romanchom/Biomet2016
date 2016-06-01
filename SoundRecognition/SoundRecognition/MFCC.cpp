#include "stdafx.h"
#include "MFCC.h"

#include "kiss_fft.h"

MFCC::MFCC()
{
	for (int i = 0; i < MFCCParametersCount; ++i)
	{
		mfccVector.push_back(0);
	}
}

MFCC::~MFCC()
{
}

void MFCC::CreateMFCCVector(AudioClip* ac)
{
	if (ac->GetDataBufferPtr()->size() != 0)
	{
		Preemphasis(ac);
		HammingWindowing();
		FFT();
		Filtering();
		EnergyLogarithm();
		DCT();
	}
	else
	{
		std::cout << "Signal is empty" << std::endl;
	}
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

//void MFCC::SignalFraming()
//{
//	for (int i = 0; i <= std::ceil(preemphasisVector.size() / frameLength); ++i)
//	{
//		SignalFrame sf;
//		for (int j = 0; j < frameLength; ++j)
//		{
//			if((frameLength * i) + j < preemphasisVector.size())
//				sf.samples.push_back(preemphasisVector[(frameLength * i) + j]);
//		}
//		mfccVector.push_back(sf);
//	}
//}

void MFCC::HammingWindowing()
{
	//SignalFraming();

	//for (int i = 0; i < mfccVector.size(); ++i)
	//{
	//	for (int j = 0; j < mfccVector[i].samples.size(); ++j)
	//	{
	//		mfccVector[i].samples[j] *= HammingAlpha - (HammingBeta * cos(2 * 3.14 * j) / frameLength - 1);
	//	}
	//}

	int frameCount = (preemphasisVector.size() / frameLength) + 1;

	for (int i = 0; i < frameCount; ++i)
	{
		for (int j = 0; j < frameLength; ++j)
		{
			if ((frameLength * i) + j >= preemphasisVector.size())
				break;
			else
				helpVector.push_back(preemphasisVector[(frameLength * i) + j] * HammingAlpha - (HammingBeta * cos(2 * 3.14 * j) / frameLength - 1));
		}
	}

	std::cout << "MFCC: Hamming Windowing completed" << std::endl;
}

void MFCC::FFT()
{
	kiss_fft_cfg fftCfg;
	int size = helpVector.size();

	if ((fftCfg = kiss_fft_alloc(size, 0, NULL, NULL)) != NULL)
	{
		kiss_fft_cpx* in;
		kiss_fft_cpx* out;
		in = new kiss_fft_cpx[size];
		out = new kiss_fft_cpx[size];

		for (int i = 0; i < size; ++i)
		{
			in[i].r = helpVector[i];
			in[i].i = 0;
		}
		kiss_fft(fftCfg, in, out);

		for (int j = 0; j < helpVector.size(); ++j)
		{
			helpVector[j] = out[j].r;
		}

		delete[] out;
		delete[] in;

		kiss_fft_free(fftCfg);

		std::cout << "MFCC: Fast Fourier Transformation completed" << std::endl;
	}
	else
	{
		std::cout << "Not enough memory" << std::endl;
		exit(-1);
	}
}

void MFCC::CreateFilters()
{
	for (int k = 1; k <= K; ++k)
	{
		Filter filter;
		filtersVector.push_back(filter);
	}
}

void MFCC::Filtering()
{
	CreateFilters();

	//44100 because I don't want to access the const value from audioclip (it's protected anyway)
	int filterArg = 44100 / helpVector.size();

	for (int k = 0; k <= K-1; ++k)
	{
		amplitudeSpectrumVector.push_back(0);
		for (int i = 0; i <= helpVector.size() / 2; ++i)
		{
			filterArg *= i;
			amplitudeSpectrumVector[k] += helpVector[i] * filtersVector[k].filterFunction(filterArg, k, d);
		}
	}

	std::cout << "MFCC: Filtering completed" << std::endl;
}

void MFCC::EnergyLogarithm()
{
	for (int k = 0; k <= K - 1; ++k)
	{
		amplitudeSpectrumVector[k] = std::pow(std::log(amplitudeSpectrumVector[k]), LogYpsilon);
	}

	std::cout << "MFCC: Energy Logarithming completed" << std::endl;
}

void MFCC::DCT()
{
	for (int n = 0; n < MFCCParametersCount; ++n)
	{
		for (int k = 0; k <= K - 1; ++k)
		{
			float cosArg = 2 * 3.14 * ((2 * k + 1) * n / 4 * K);
			mfccVector[n] += amplitudeSpectrumVector[k] * std::cos(cosArg);
		}
	}

	std::cout << "MFCC: Discrete Cosine Transform completed" << std::endl;
}

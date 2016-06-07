#include "stdafx.h"
#include "SignalFrame.h"


SignalFrame::SignalFrame()
{
}


SignalFrame::~SignalFrame()
{
}

void SignalFrame::CreateMFCCVector(AudioClip * ac)
{
	//Preemphasis(ac);
	HammingWindowing();
	FFT();
	Filtering();
	EnergyLogarithm();
	DCT();
}

void SignalFrame::PrintMFCCCoefficients()
{
	std::cout << "MFCC Coefficients: ";
	for (int i = 0; i < mfccCoefficients.size(); ++i)
	{
		std::cout << mfccCoefficients[i] << " ";
	}
	std::cout << std::endl;
}

void SignalFrame::PrintEachStep(char * title, std::vector<float>& vector)
{
	std::cout << title;
	for (int i = 0; i < vector.size(); ++i)
	{
		std::cout << vector[i] << " ";
	}
	std::cout << std::endl;
}

float SignalFrame::ComplexAbs(float &r, float &i)
{
	return std::sqrt((r * r) + (i * i));
}

void SignalFrame::Preemphasis(AudioClip * ac)
{
	preemphasisVector.push_back((float)ac->GetDataBufferPtr()->at(0));
	for (int i = 1; i < ac->GetDataBufferPtr()->size(); ++i)
	{
		preemphasisVector.push_back((float)ac->GetDataBufferPtr()->at(i) - (PreemphasisConst * ac->GetDataBufferPtr()->at(i - 1)));
	}
	//std::cout << "MFCC: Preemphasis completed" << std::endl;
}

void SignalFrame::HammingWindowing()
{
	//int frameCount = (preemphasisVector.size() / size) + 1;

	//for (int i = 0; i < frameCount; ++i)
	//{
	//	for (int j = 0; j < size; ++j)
	//	{
	//		if ((size * i) + j >= preemphasisVector.size())
	//			break;
	//		else
	//			helpVector.push_back(preemphasisVector[(size * i) + j] * HammingAlpha - (HammingBeta * cos(2 * 3.14 * j) / size - 1));
	//	}
	//}

	for (int i = 0; i < helpVector.size(); ++i)
	{
		//helpVector[i] = std::abs(helpVector[i]);
		//Fixing equilibrium. A 16 bit sound has a sample values ranging from -32768 to 32767. Adding 32768 to every sample fixes equilibrium from 0 to 32768
		//helpVector[i] += 32768;
		helpVector[i] *= HammingAlpha - (HammingBeta * cos((M_PI * 2 * i) / (size - 1)));
	}

	//std::cout << "MFCC: Hamming Windowing completed" << std::endl;
}

void SignalFrame::FFT()
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
			//helpVector[j] = std::abs(out[j].r);
			helpVector[j] = ComplexAbs(out[j].r, out[j].i);
		}

		delete[] out;
		delete[] in;

		kiss_fft_free(fftCfg);

		//std::cout << "MFCC: Fast Fourier Transformation completed" << std::endl;
	}
	else
	{
		std::cout << "Not enough memory" << std::endl;
		exit(-1);
	}
}

void SignalFrame::CreateFilters()
{
	for (int k = 1; k <= K; ++k)
	{
		Filter filter;
		filtersVector.push_back(filter);
	}
}

void SignalFrame::Filtering()
{
	CreateFilters();

	//44100 because I don't want to access the const value from audioclip (it's protected anyway)
	float filterArg;
	Filter filter;

	for (int k = 1; k <= K; ++k)
	{
		amplitudeSpectrumVector.push_back(0);
		for (int i = 0; i <= helpVector.size() / 2; ++i)
		{
			filterArg = (44100.0f / helpVector.size()) * i;
			amplitudeSpectrumVector[k - 1] += helpVector[i] * filter.filterFunction(filterArg, k, d);
		}
	}

	//std::cout << "MFCC: Filtering completed" << std::endl;
}

void SignalFrame::EnergyLogarithm()
{
	for (int k = 0; k < K; ++k)
	{
		if (amplitudeSpectrumVector[k] == 0)
		{
			break;
		}
		else
		{
			amplitudeSpectrumVector[k] = std::pow(std::log(amplitudeSpectrumVector[k]), LogYpsilon);
		}
	}

	//std::cout << "MFCC: Energy Logarithming completed" << std::endl;
}

void SignalFrame::DCT()
{

	PrintEachStep("MFCC Amplitude: ", amplitudeSpectrumVector);

	for (int n = 0; n < MFCCParametersCount; ++n)
	{
		double kos = 0;
		for (int k = 0; k < K; ++k)
		{
			double cosArg = M_PI * 2 * (((2 * k + 1) * n) / (4 * K));
			kos += amplitudeSpectrumVector[k] * std::cos(cosArg);
		}
		mfccCoefficients.push_back(kos);
	}

	PrintEachStep("MFCC Coefficients: ", mfccCoefficients);

	//std::cout << "MFCC: Discrete Cosine Transform completed" << std::endl;
}
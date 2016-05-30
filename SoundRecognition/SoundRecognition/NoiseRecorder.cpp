#include "stdafx.h"
#include "NoiseRecorder.h"

#include <fstream>


NoiseRecorder::NoiseRecorder()
{
}


NoiseRecorder::~NoiseRecorder()
{
}

void NoiseRecorder::Initialize()
{
	_noiseAmplitude = 0;

	std::fstream config = std::fstream(FILE_PATH, std::ios::in);
	if (!config.fail())
	{
		config >> _noiseAmplitude;
		config.close();
	}
}

void NoiseRecorder::Shutdown()
{
	std::fstream config = std::fstream(FILE_PATH, std::ios::out);
	if (!config.fail())
	{
		config.clear();
		config << _noiseAmplitude;
		config.close();
	}

	_noiseAmplitude = 0;
	_recordClip.Shutdown();
}

void NoiseRecorder::StartRecord()
{
	std::string dummyName = "DUMMY";
	_recordClip.Shutdown();
	_recordClip.InitializeFromMic(&dummyName, &dummyName, 0);
}

void NoiseRecorder::StopRecord()
{
	_recordClip.StopInitializingFromMic();
	const std::vector<short>* data = _recordClip.GetDataBufferPtr();
	_noiseAmplitude = GetNoiseAmplitude(data, (int)((double)SAMPLE_PER_SECOND_COUNT * _recordClip.GetTimeSeconds()));
}

short NoiseRecorder::GetNoiseAmplitude(const std::vector<short>* data, int sampleCount)
{
	short* tab = new short[sampleCount];
	ZeroMemory(tab, sampleCount);
	int dataSize = data->size();
	int cIndex = 0;

	for (int i = 0; i < dataSize; ++i)
	{
		for (int j = 0; j < sampleCount; ++j)
		{
			if ((*data)[i] > tab[j])
			{
				tab[j] = (*data)[i];
				break;
			}
		}
	}

	float avg = 0.0;
	for (int i = 0; i < sampleCount; ++i)
	{
		avg += (float)tab[i];
	}
	avg /= (float)sampleCount;

	delete tab;
	return (short)avg;
}
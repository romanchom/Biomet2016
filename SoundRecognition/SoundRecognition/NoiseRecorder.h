#pragma once

#include "AudioClip.h"

class AudioClip;

class NoiseRecorder
{
protected:

#pragma region const

	const int SAMPLE_PER_SECOND_COUNT = 5;
	const std::string FILE_PATH = ".\\Config\\NoiseLast.txt";

#pragma endregion
	
#pragma region protected

	float _noiseAmplitude = 0;
	AudioClip _recordClip;

#pragma endregion

#pragma region protectedFunctions

	inline short GetNoiseAmplitude(const std::vector<short>* data, int sampleCount);

#pragma endregion

public:
#pragma region publicFunctions

	NoiseRecorder();
	~NoiseRecorder();

	void Initialize();
	void Shutdown();

	void StartRecord();
	void StopRecord();

	float GetNoiseAmplitude() { return _noiseAmplitude; }

#pragma endregion
};


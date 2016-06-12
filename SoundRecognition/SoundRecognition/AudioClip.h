#pragma once

#include "MFCC.h"

class MFCC;

class AudioClip
{
protected:

#pragma region const

	const int DESIRED_CHANNELS = 1;
	const int DESIRED_SAMPLERATE_HZ = 44100;
	const int DESIRED_FORMAT = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	const int DESIRED_FORMAT_PA = paInt16 | paNonInterleaved; 
	const int FRAMES_PER_BUFFER = 0;
	const int NOISE_MIN_LENGTH = 4410;

#pragma endregion

#pragma region protected

	std::string _name;
	std::string _fullFilePath;
	bool _wasLoadedFromFile = false;

	std::vector<short> _data;
	unsigned long _dataSize = 0;
	SF_INFO _dataInfo;

	MFCC _mfcc;

	short _noiseLevel;

	PaStream* _streamPtr = nullptr;

#pragma endregion

#pragma region functionsProtected

	static int StreamCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

	void CutoutNoise(short noiseLevel);

#pragma endregion

public:
	AudioClip();
	~AudioClip();

#pragma region functionsPublic

	void InitializeFromMic(const std::string* const name, const std::string* const clipsPath, short noiseLevel);
	void InitializeFromFile(const std::string* const name, const std::string* const fullFilePath);
	void SaveToFileAudio();

	void StopInitializingFromMic();

	void Shutdown();

	void ToString(std::string* const str);

	double GetDTWDistance(const AudioClip* const other) const;

#pragma endregion

#pragma region Getters

	const std::string* const GetName() { return &_name; }
	const std::string* const GetFullFilePath() { return &_fullFilePath; }
	bool GetWasLoadedFromFile() { return _wasLoadedFromFile; }
	const std::vector<short>* const GetDataBufferPtr() { return &_data; }
	unsigned long GetDataSize() { return _dataSize; }
	int GetSampleRate() { return _dataInfo.samplerate; }
	double GetTimeSeconds() { return (double)_dataSize / (double)_dataInfo.samplerate; }
	const MFCC* const GetMelCoefficents() { return &_mfcc; }

#pragma endregion

#pragma region Setters

#pragma endregion
};


#pragma once

class AudioClip
{
protected:

#pragma region const

	const int DESIRED_CHANNELS = 1;
	const int DESIRED_SAMPLERATE_HZ = 44100;
	const int DESIRED_FORMAT = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	const int DESIRED_FORMAT_PA = paInt16 | paNonInterleaved; 
	const int FRAMES_PER_BUFFER = 0;

#pragma endregion

#pragma region protected

	std::string _name;
	std::string _fullFilePath;
	bool _wasLoadedFromFile = false;

	std::vector<short> _data;
	unsigned long _dataSize = 0;
	SF_INFO _dataInfo;

	PaStream* _streamPtr = nullptr;

#pragma endregion

#pragma region functionsProtected

	static int StreamCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

#pragma endregion

public:
	AudioClip();
	~AudioClip();

#pragma region functionsPublic

	void InitializeFromMic(const std::string* const name, const std::string* const clipsPath);
	void InitializeFromFile(const std::string* const name, const std::string* const fullFilePath);
	void SaveToFileAudio();

	void StopInitializingFromMic();

	void Shutdown();

	void ToString(std::string* const str);

#pragma endregion

#pragma region Getters

	std::string GetName() { return _name; }
	std::string GetFullFilePath() { return _fullFilePath; }
	bool GetWasLoadedFromFile() { return _wasLoadedFromFile; }

#pragma endregion

#pragma region Setters

#pragma endregion
};


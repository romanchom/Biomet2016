#pragma once

#include <map>

class AudioClip;

class ClipComparator
{
private:

#pragma region privateConst

	const int SPEAKER_COUNT = 3;
	const int CLIPS_PER_SPEAKER = 5;

	const int PER_ONE_SPEAKER_MIN_POSITIVE = 3;
	const double MAX_DISTANCE_TO_POSITIVE = 35000.0;

#pragma endregion

#pragma region private

	std::vector<AudioClip*>* _allClips;

	std::map<std::string, std::string> _recognizedWords;	// key is letter in file, value is word itself
	std::map <std::string, std::vector<std::vector<AudioClip*>>> _data;

	std::map<std::string, int> _successPerWord;

#pragma endregion

#pragma region privateFunctions

	inline void ClearSuccessfulRecognitions();

#pragma endregion

public:

#pragma region public

#pragma endregion

#pragma region publicFunctions

	ClipComparator();
	~ClipComparator();

	void Initialize(std::vector<AudioClip*>* allClips);
	void Shutdown();

	void ClipToString(const AudioClip* const ac, std::string* outString, int* outSuccesses);
	void GetRecognizableClipsList(std::vector<std::string>* outWordList);

#pragma endregion
};


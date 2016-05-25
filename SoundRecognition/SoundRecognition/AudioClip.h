#pragma once
class AudioClip
{
protected:

#pragma region protected

	std::string _name;
	std::string _fullFilePath;
	bool _wasLoadedFromFile = false;

#pragma endregion

public:
	AudioClip();
	~AudioClip();

#pragma region functionsPublic

	void InitializeFromMic(std::string* const name);
	void InitializeFromFile(std::string* const filePath);
	void SaveToFileAudio(std::string* const filePath = nullptr);

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


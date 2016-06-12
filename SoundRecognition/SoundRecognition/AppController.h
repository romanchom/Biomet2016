#pragma once

#include "Chart.h"
#include "NoiseRecorder.h"
#include "ClipComparator.h"

#define ClearScreen() system("cls")

class AudioClip;

class AppController
{
private:

#pragma region const

	const char CH_ESC = 27;
	const char CH_ENTER = 13;
	const char CH_YES = 'y';
	const char CH_NO = 'n';
	const char CH_SELECT_RECORD = '1';
	const char CH_SELECT_RECOGNIZE = '2';
	const char CH_SELECT_RECOGNIZE_DATABASE = '3';
	const char CH_SELECT_LIST = '4';
	const char CH_SELECT_NOISE = '5';
	const char CH_SELECT_EXIT = '6';

	const std::string CLIPS_PATH = ".\\Samples\\";

	const bool SHOW_PORTAUDIO_INIT_INFO = false;

#pragma endregion

#pragma region enum

	enum MenuMode
	{
		MENU,
		LIST,
		RECORD,
		RECOGNIZE,
		RECOGNIZE_DATABASE,
		NOISE
	};

#pragma endregion

#pragma region privateVars

	bool _working;
	std::string _errString;
	MenuMode _mode;
	std::vector<AudioClip*> _clips;
	Chart _chart;
	NoiseRecorder _noiseRecorder;
	ClipComparator _clipComparator;

#pragma endregion

#pragma region privateFuncs

	inline void PrintMenu();
	inline void PrintRecordBegin();
	inline void PrintRecordMiddle();
	inline void PrintRecordEnd();
	inline void PrintRecognizeBegin();
	inline void PrintRecognizeEnd(const std::string* const result, int successes);
	inline void PrintRecDatabaseBegin();
	inline void PrintRecDatabaseEnd(const std::string* const result, int resultProbes);
	inline void PrintClipCollection();
	inline void PrintNoiseRecordBegin();
	inline void PrintNoiseRecordMiddle();
	inline void PrintNoiseRecordEnd();
	inline void PrintErrorString();
	inline void ClearErrorString();
	inline void SetErrorString(std::string str);
	inline bool CheckIfNameIsUnique(const std::string* const str);

	inline void PopulateCollectionFromDisk();
	inline void SaveToDiskAndDestroyCollection();

#pragma endregion

public:
	AppController();
	~AppController();

#pragma region functionsPublic

	void Initialize();
	void Run();
	void Shutdown();

#pragma endregion
};


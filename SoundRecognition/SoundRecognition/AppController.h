#pragma once

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
	const char CH_SELECT_LIST = '3';
	const char CH_SELECT_EXIT = '4';

#pragma endregion

#pragma region enum

	enum MenuMode
	{
		MENU,
		LIST,
		RECORD,
		RECOGNIZE
	};

#pragma endregion

#pragma region privateVars

	bool _working;
	std::string _errString;
	MenuMode _mode;
	std::vector<AudioClip*> _clips;

#pragma endregion

#pragma region privateFuncs

	inline void PrintMenu();
	inline void PrintRecordBegin();
	inline void PrintRecordMiddle();
	inline void PrintRecordEnd();
	inline void PrintRecognizeBegin();
	inline void PrintRecognizeEnd(const std::string* const result);
	inline void PrintClipCollection();
	inline void PrintErrorString();
	inline void ClearErrorString();
	inline void SetErrorString(std::string str);
	inline bool CheckIfNameIsUnique(const std::string* const str);

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


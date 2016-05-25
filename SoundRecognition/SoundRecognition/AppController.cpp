#include "stdafx.h"
#include "AppController.h"

#include "AudioClip.h"


AppController::AppController()
{
}


AppController::~AppController()
{
}

void AppController::Initialize()
{
	_mode = MENU;
	_working = true;
}

void AppController::Run()
{
	while (_working)
	{
		ClearScreen();
		PrintErrorString();
		ClearErrorString();
		
		char selection;

		switch (_mode)
		{
		case AppController::MENU:
		{
			PrintMenu();
			while (!_kbhit());
			selection = _getch();

			if (selection == CH_SELECT_RECORD)
			{
				_mode = RECORD;
			}
			else if (selection == CH_SELECT_RECOGNIZE)
			{
				_mode = RECOGNIZE;
			}
			else if (selection == CH_SELECT_LIST)
			{
				_mode = LIST;
			}
			else if (selection == CH_SELECT_EXIT)
			{
				_working = false;
			}
			else
			{
				SetErrorString("Wrong option selected in menu.");
			}

		}
		break;
		case AppController::RECORD:
		{
			PrintRecordBegin();

			std::string newName;
			do
			{
				std::cin >> newName;
				if (CheckIfNameIsUnique(&newName))
				{
					break;
				}
				else
				{
					std::cout << "\r";
					std::cout << "Name already in use!" << std::endl <<
							"Name: ";
				}
			} while (true);


			PrintRecordMiddle();

			// here will recording start

			AudioClip* ac = new AudioClip();
			ac->InitializeFromMic(&newName);

			bool esc = false;
			while (true)
			{
				while (!kbhit());
				selection = _getch();

				if (selection == CH_ESC)
				{
					esc = true;
					break;
				}
				else if (selection == CH_ENTER)
				{
					break;
				}
				else
				{
					std::cout << "\r";
				}
			}

			// here will recording end

			ac->StopInitializingFromMic();

			if (esc)
			{
				_mode = MENU;
				ac->Shutdown();
				delete ac;
				break;
			}

			// here recording will be saved to collection
			_clips.push_back(ac);

			PrintRecordEnd();

			while (true)
			{
				while (!kbhit());
				selection = _getch();

				if (selection == CH_NO)
				{
					_mode = MENU;
					break;
				}
				else if (selection == CH_YES)
				{
					break;
				}
				else
				{
					std::cout << "\r";
				}
			}
		}
		break;
		case AppController::RECOGNIZE:
		{
			PrintRecognizeBegin();

			std::string tempName = "REC_CLIP";
			AudioClip* ac = new AudioClip();
			ac->InitializeFromMic(&tempName);

			bool esc = false;
			while (true)
			{
				while (!kbhit());
				selection = _getch();

				if (selection == CH_ESC)
				{
					esc = true;
					break;
				}
				else if (selection == CH_ENTER)
				{
					break;
				}
				else
				{
					std::cout << "\r";
				}
			}

			ac->StopInitializingFromMic();

			if (esc)
			{
				ac->Shutdown();
				delete ac;
				_mode = MENU;
				break;
			}

			// here recognition will take place

			std::string temp = "Not implemented";
			PrintRecognizeEnd(&temp);

			while (!kbhit());
			selection = _getch();

			if (selection == CH_ESC)
			{
				_mode = MENU;
			}
		}
		break;

		case AppController::LIST:
		{
			PrintClipCollection();
			while (!_kbhit());
			selection = _getch();
			_mode = MENU;
		}
		break;

		default:
		{

		}
			break;
		}
	}
}

void AppController::Shutdown()
{
}

void AppController::PrintMenu()
{
	std::cout << "[" << CH_SELECT_RECORD << "] Record sample" << std::endl
		<< "[" << CH_SELECT_RECOGNIZE << "] Recognize words" << std::endl
		<< "[" << CH_SELECT_LIST << "] Show clip collection" << std::endl
		<< "[" << CH_SELECT_EXIT << "] Exit program" << std::endl;
}

inline void AppController::PrintRecordBegin()
{
	std::cout << "Provide a name for a new sample. Recording will begin after you hit ENTER." << std::endl
		<< "Press ENTER again to save it, press ESC at any time to cancel." << std::endl
		<< "Name: ";
}

inline void AppController::PrintRecordMiddle()
{
	std::cout << "Recording..." << std::endl;
}

inline void AppController::PrintRecordEnd()
{
	std::cout << "Recorded sample saved to collection. Do you want to record another one? [y/n]" << std::endl;
}

inline void AppController::PrintRecognizeBegin()
{
	std::cout << "Capturing word in progress..." << std::endl 
				<< "Press ENTER to finish and show recognition results. Press ESC to return to menu." << std::endl;
}

inline void AppController::PrintRecognizeEnd(const std::string* const result)
{
	std::cout << "Recognized word is: " << *result << std::endl 
				<< "Press ESC to return to menu or any key to recognize again." << std::endl;
}

inline void AppController::PrintClipCollection()
{
	int clipCount = _clips.size();
	std::string tmp;
	tmp.clear();
	for (int i = 0; i < clipCount; ++i)
	{
		_clips[i]->ToString(&tmp);
		std::cout << tmp << std::endl;
		tmp.clear();
	}

	std::cout << std::endl
		<< "Total clips in collection: " << clipCount << std::endl
		<< "Press any key to return to menu." << std::endl;
}

inline void AppController::PrintErrorString()
{
	if (_errString.size() != 0)
	{
		std::cout << _errString << std::endl;
	}
}

inline void AppController::ClearErrorString()
{
	_errString.clear();
}

inline void AppController::SetErrorString(std::string str)
{
	_errString = "Error: " + str + "\n";
}

inline bool AppController::CheckIfNameIsUnique(const std::string * const str)
{
	for (std::vector<AudioClip*>::iterator it = _clips.begin(); it != _clips.end(); ++it)
	{
		if ((*it)->GetName() == *str)
		{
			return false;
		}
	}
	return true;
}
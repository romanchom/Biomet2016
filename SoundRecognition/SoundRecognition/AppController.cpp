#include "stdafx.h"
#include "AppController.h"
#include "MFCC.h"
#include "AudioClip.h"
#include "NoiseRecorder.h"


AppController::AppController()
{
}


AppController::~AppController()
{
}

void AppController::Initialize()
{
	// initializing variables

	_mode = MENU;
	_working = true;

	// loading all samples from folder to collection

	PopulateCollectionFromDisk();

	// load PortAudio
	Pa_Initialize();

	if (SHOW_PORTAUDIO_INIT_INFO)
	{
		std::cout << "PortAudio Initialized. Press any key to continue.";
		while (!_kbhit());
	}

	_noiseRecorder.Initialize();
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
			else if (selection == CH_SELECT_NOISE)
			{
				_mode = NOISE;
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
			ac->InitializeFromMic(&newName, &CLIPS_PATH, _noiseRecorder.GetNoiseAmplitude());

			bool esc = false;
			while (true)
			{
				while (!_kbhit());
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
				while (!_kbhit());
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
			ac->InitializeFromMic(&tempName, &CLIPS_PATH, _noiseRecorder.GetNoiseAmplitude());

			bool esc = false;
			while (true)
			{
				while (!_kbhit());
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
			MFCC mel;
			mel.CreateMFCCVector(ac);

			std::string temp = "Not implemented";
			PrintRecognizeEnd(&temp);

			while (!_kbhit());
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

			int id = -1;
			std::string input;
			int siz = _clips.size();
			while (true)
			{
				std::cin.clear();
				std::cin >> input;
				char* p;
				id = std::strtol(input.c_str(), &p, 10);

				if (!(*p) && id >= 0 && id < _clips.size())
				{
					AudioClip* first = _clips[id];
					_chart.Initialize(first->GetDataBufferPtr(), first->GetTimeSeconds(), first->GetName());
					_chart.Draw();
				}
				else
				{
					break;
				}
			}
			_mode = MENU;
		}
		break;

		case AppController::NOISE:
		{
			PrintNoiseRecordBegin();

			bool any = false;
			while (true)
			{
				while (!_kbhit());
				selection = _getch();

				if (selection == CH_ESC)
				{
					any = true;
					break;
				}
				else
				{
					break;
				}
			}

			if (any)
			{
				_mode = MENU;
				break;
			}

			PrintNoiseRecordMiddle();
			_noiseRecorder.StartRecord();

			while (!_kbhit());
			selection = _getch();

			_noiseRecorder.StopRecord();

			PrintNoiseRecordEnd();

			while (!_kbhit()) ;
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
	_chart.Shutdown();
	_noiseRecorder.Shutdown();
	Pa_Terminate();

	SaveToDiskAndDestroyCollection();
}

void AppController::PrintMenu()
{
	std::cout << "[" << CH_SELECT_RECORD << "] Record sample" << std::endl
		<< "[" << CH_SELECT_RECOGNIZE << "] Recognize words" << std::endl
		<< "[" << CH_SELECT_LIST << "] Show clip collection" << std::endl
		<< "[" << CH_SELECT_NOISE << "] Capture noise" << std::endl
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
		std::cout << "[" << i << "]" << std::endl << tmp << std::endl;
		tmp.clear();
	}

	std::cout << std::endl
		<< "Total clips in collection: " << clipCount << std::endl
		<< "Input a valid clip number to plot its graph. Input anything else to return to menu." << std::endl;
}

inline void AppController::PrintNoiseRecordBegin()
{
	std::cout << "Press any key to start noise recording or press ESC to cancel." << std::endl
		<< "Currently noise max amplitude is: " << _noiseRecorder.GetNoiseAmplitude() << std::endl;
}

inline void AppController::PrintNoiseRecordMiddle()
{
	std::cout << "Noise recording in progress... Press any key to cancel." << std::endl;
}

inline void AppController::PrintNoiseRecordEnd()
{
	std::cout << "Noise recording finished. Press any key to return to menu." << std::endl
		<< "Currently noise max amplitude is: " << _noiseRecorder.GetNoiseAmplitude() << std::endl;
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
		if (*(*it)->GetName() == *str)
		{
			return false;
		}
	}
	return true;
}

inline void AppController::PopulateCollectionFromDisk()
{
	std::wstring path(CLIPS_PATH.begin(), CLIPS_PATH.end());
	path += L"*";
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fData;

	hFind = FindFirstFile(path.c_str(), &fData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{

		std::wstring name = fData.cFileName;
		std::string sName(name.begin(), name.end());
		std::istringstream iss(sName);
		std::vector<std::string> sSplit;
		std::string item;
		while (std::getline(iss, item, '.'))
		{
			sSplit.push_back(item);
		}
		std::string ext = sSplit[sSplit.size() - 1];
		if (ext == "wav")
		{
			AudioClip* ac = new AudioClip();
			std::string filePath = CLIPS_PATH + sName;
			ac->InitializeFromFile(&sSplit[0], &filePath);
			_clips.push_back(ac);
		}
	} while (FindNextFile(hFind, &fData) != 0);
}

inline void AppController::SaveToDiskAndDestroyCollection()
{
	for (std::vector<AudioClip*>::iterator it = _clips.begin(); it != _clips.end(); ++it)
	{
		if (!(*it)->GetWasLoadedFromFile())
		{
			(*it)->SaveToFileAudio();
		}
		(*it)->Shutdown();
		delete (*it);
	}
	_clips.clear();
}

#include "stdafx.h"
#include "ClipComparator.h"
#include "AudioClip.h"


ClipComparator::ClipComparator()
{
	
}


ClipComparator::~ClipComparator()
{
}

void ClipComparator::Initialize(std::vector<AudioClip*>* allClips)
{
	_allClips = allClips;
	_recognizedWords.emplace("a", "Janusz");
	_recognizedWords.emplace("b", "Andrzej");

	int rWSize = _recognizedWords.size();
	for (std::map<std::string, std::string>::iterator it = _recognizedWords.begin(); it != _recognizedWords.end(); ++it)
	{
		std::vector<std::vector<AudioClip*>> vec;

		for (int i = 0; i < SPEAKER_COUNT; ++i)
		{
			std::vector<AudioClip*> smallVec;
			vec.push_back(smallVec);
		}
		_data.emplace((*it).first, vec);

		_successPerWord.emplace((*it).first, 0);
	}

	for (std::vector<AudioClip*>::iterator it = _allClips->begin(); it != _allClips->end(); ++it)
	{
		AudioClip* ac = *it;
		std::string acName = *ac->GetName();

		std::istringstream iss(acName);
		std::vector<std::string> sSplit;
		std::string item;
		while (std::getline(iss, item, '_'))
		{
			sSplit.push_back(item);
		}

		if (sSplit.size() != 3)
		{
			std::cout << "ClipComparator ERROR - file name not supported." << std::endl;
			continue;
		}

		std::string clipKey = sSplit[1];
		int speaker = 0;
		if (sSplit[0] == "m")
		{
			speaker = 0;
		}
		else if (sSplit[0] == "j")
		{
			speaker = 1;
		}
		else if (sSplit[0] == "r")
		{
			speaker = 2;
		}

		((_data[clipKey])[speaker]).push_back(ac);
	}
}

void ClipComparator::Shutdown()
{
}

void ClipComparator::ClipToString(const AudioClip * const ac, std::string * outString, int* outSuccesses)
{
	*outString = "Not recognized";
	*outSuccesses = 0;

	ClearSuccessfulRecognitions();

	for (std::map <std::string, std::vector<std::vector<AudioClip*>>>::iterator it = _data.begin(); it != _data.end(); ++it)
	{
		for (std::vector<std::vector<AudioClip*>>::iterator jt = (*it).second.begin(); jt != (*it).second.end(); ++jt)
		{
			for (std::vector<AudioClip*>::iterator gt = (*jt).begin(); gt != (*jt).end(); ++gt)
			{
				double dist = ac->GetDTWDistance((*gt));
				if (dist < MAX_DISTANCE_TO_POSITIVE)
				{
					++_successPerWord[it->first];
					++*outSuccesses;
				}
			}
		}
	}

	int maxSuccesses = 0;
	std::string maxSuccessesKey;
	for (std::map<std::string, int>::iterator it = _successPerWord.begin(); it != _successPerWord.end(); ++it)
	{
		if (it->second >= maxSuccesses)
		{
			maxSuccesses = it->second;
			maxSuccessesKey = it->first;
		}
	}

	if (maxSuccesses >= PER_ONE_SPEAKER_MIN_POSITIVE)
	{
		*outString = _recognizedWords[maxSuccessesKey];
	}
}

void ClipComparator::GetRecognizableClipsList(std::vector<std::string>* outWordList)
{
	for (std::map<std::string, std::string>::iterator it = _recognizedWords.begin(); it != _recognizedWords.end(); ++it)
	{
		outWordList->push_back((*it).second);
	}
}

inline void ClipComparator::ClearSuccessfulRecognitions()
{
	for (std::map<std::string, int>::iterator it = _successPerWord.begin(); it != _successPerWord.end(); ++it)
	{
		(*it).second = 0;
	}
}
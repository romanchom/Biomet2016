#include "stdafx.h"
#include "AudioClip.h"


AudioClip::AudioClip()
{
}


AudioClip::~AudioClip()
{
}

void AudioClip::InitializeFromMic(std::string* const name)
{
	_name = *name;
	_wasLoadedFromFile = false;
}

void AudioClip::InitializeFromFile(std::string * const filePath)
{
	_wasLoadedFromFile = true;
}

void AudioClip::SaveToFileAudio(std::string * const filePath)
{
}

void AudioClip::StopInitializingFromMic()
{
}

void AudioClip::Shutdown()
{
}

void AudioClip::ToString(std::string * const str)
{
	*str += _name;
}

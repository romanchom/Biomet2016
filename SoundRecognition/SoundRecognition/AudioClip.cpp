#include "stdafx.h"
#include "AudioClip.h"


AudioClip::AudioClip()
{
}


AudioClip::~AudioClip()
{
}

void AudioClip::InitializeFromMic(const std::string* const name, const std::string* const clipsPath)
{
	_name = *name;
	_fullFilePath = *clipsPath + *name + ".wav";
	_wasLoadedFromFile = false;

	PaDeviceIndex mic = Pa_GetDefaultInputDevice();
	const PaDeviceInfo* info = Pa_GetDeviceInfo(mic);
}

void AudioClip::InitializeFromFile(const std::string* const name, const std::string * const fullFilePath)
{
	_name = *name;
	_fullFilePath = *fullFilePath;
	_wasLoadedFromFile = true;

	_dataInfo.format = 0;
	SNDFILE* file = sf_open(_fullFilePath.c_str(), SFM_READ, &_dataInfo);

	if (_dataInfo.channels != DESIRED_CHANNELS ||
		_dataInfo.samplerate != DESIRED_SAMPLERATE_HZ ||
		_dataInfo.format != DESIRED_FORMAT)
	{
		sf_close(file);
		return;
	}

	_dataSize = _dataInfo.frames * _dataInfo.channels;
	_data.resize(_dataSize);
	sf_readf_short(file, &(*_data.begin()), _dataSize);
	sf_close(file);
}

void AudioClip::SaveToFileAudio()
{
	_dataInfo.channels = DESIRED_CHANNELS;
	_dataInfo.format = DESIRED_FORMAT;
	_dataInfo.samplerate = DESIRED_SAMPLERATE_HZ;

	SNDFILE* file = sf_open(_fullFilePath.c_str(), SFM_WRITE, &_dataInfo);
	sf_writef_short(file, &(*_data.begin()), _dataSize);
	sf_close(file);
	_wasLoadedFromFile = true;
}

void AudioClip::StopInitializingFromMic()
{
}

void AudioClip::Shutdown()
{
}

void AudioClip::ToString(std::string * const str)
{
	*str += _name + "\n"
		+ "Channels: " + std::to_string(_dataInfo.channels) + "\n"
		+ "Format: " + std::to_string(_dataInfo.format) + "\n"
		+ "Sample rate [Hz]: " + std::to_string(_dataInfo.samplerate) + "\n"
		+ "Frame count: " + std::to_string(_dataInfo.frames) + "\n";
}

#include "stdafx.h"
#include "AudioClip.h"


AudioClip::AudioClip()
{
	ZeroMemory(&_dataInfo, sizeof(_dataInfo));
}


AudioClip::~AudioClip()
{
}

void AudioClip::InitializeFromMic(const std::string* const name, const std::string* const clipsPath, short noiseLevel)
{
	_name = *name;
	_fullFilePath = *clipsPath + *name + ".wav";
	_noiseLevel = noiseLevel;
	_wasLoadedFromFile = false;

	PaDeviceIndex mic = Pa_GetDefaultInputDevice();
	const PaDeviceInfo* info = Pa_GetDeviceInfo(mic);

	PaStreamParameters params;
	ZeroMemory(&params, sizeof(PaStreamParameters));
	params.channelCount = DESIRED_CHANNELS;
	params.device = mic;
	params.sampleFormat = DESIRED_FORMAT_PA;
	params.suggestedLatency = info->defaultLowInputLatency;

	PaStreamFlags flags;
	ZeroMemory(&flags, sizeof(PaStreamFlags));
	flags = paClipOff | paDitherOff;

	_dataInfo.channels = DESIRED_CHANNELS;
	_dataInfo.samplerate = DESIRED_SAMPLERATE_HZ;
	_dataInfo.format = DESIRED_FORMAT;

	Pa_OpenStream(&_streamPtr, &params, NULL, DESIRED_SAMPLERATE_HZ, FRAMES_PER_BUFFER, 
					flags, StreamCallback, (void*)this);
	Pa_StartStream(_streamPtr);
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
	if (_dataSize == 0 || _data.size() == 0)
	{
		return;
	}

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
	Pa_StopStream(_streamPtr);
	Pa_CloseStream(_streamPtr);

	CutoutNoise(_noiseLevel);
}

void AudioClip::Shutdown()
{
	ZeroMemory(&_dataInfo, sizeof(_dataInfo));
	_data.clear();
	_dataSize = 0;
	_name.clear();
	_fullFilePath.clear();

}

int AudioClip::StreamCallback(const void * input, void * output, unsigned long frameCount, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags, void * userData)
{
	AudioClip* instance = (AudioClip*)userData;

	// cause we have non-interlaved one channel input
	const short* dataPtr = ((const short**)input)[0];

	if (statusFlags != 0)
	{
		std::cout << statusFlags << std::endl;
		return paAbort;
	}

	for (unsigned long i = 0; i < frameCount; ++i)
	{
		instance->_data.push_back(dataPtr[i]);
	}

	instance->_dataInfo.frames += frameCount;
	instance->_dataSize = instance->_dataInfo.frames;

	return paContinue;
}

void AudioClip::CutoutNoise(short noiseLevel)
{
	bool windowBegin = false;
	unsigned long windowLength = 0;

	std::vector<short> newData;

		for (unsigned long i = 0; i < _dataSize; ++i)
		{
			//// beginning of noise window
			//if (abs(_data[i]) <= noiseLevel && !windowBegin)
			//{
			//	windowBegin = true;
			//}
			//// end of noise window
			//else if (abs(_data[i]) <= noiseLevel && abs(_data[i + 1]) > noiseLevel && windowBegin)
			//{
			//	//// window is long enough - let's assign it to be cut out
			//	//if (windowLastID - windowBeginID >= NOISE_MIN_LENGTH)
			//	//{
			//	//	for (unsigned long j = windowBeginID; j <= windowLastID; ++j)
			//	//	{
			//	//		noiseIDs.push_back(j);
			//	//	}
			//	//}

			//	windowBegin = false;
			//}
			//else

			if (abs(_data[i]) <= noiseLevel)
			{
				windowLength = 0;
				for (unsigned long j = i; j < _dataSize; ++j)
				{
					++windowLength;
					if (abs(_data[j]) > noiseLevel)
					{
						break;
					}
				}

				if (windowLength >= NOISE_MIN_LENGTH)
				{
					i += (windowLength - 1);
				}
			}


			newData.push_back(_data[i]);
		}

		_data = newData;
		_dataSize = newData.size();
}

void AudioClip::ToString(std::string * const str)
{
	*str += _name + "\n"
		+ "Channels: " + std::to_string(_dataInfo.channels) + "\n"
		+ "Format: " + std::to_string(_dataInfo.format) + "\n"
		+ "Sample rate [Hz]: " + std::to_string(_dataInfo.samplerate) + "\n"
		+ "Frame count: " + std::to_string(_dataInfo.frames) + "\n";
}

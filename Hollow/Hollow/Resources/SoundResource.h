#pragma once
#include <XAudio2.h>
#include "Hollow/Platform.h"
#include "Hollow/Common/Log.h"

class HOLLOW_API SoundResource
{
public:
	SoundResource(IXAudio2SourceVoice* sourceVoice);
	~SoundResource();
	void Play();
private:
	IXAudio2SourceVoice*		pSourceVoice;
};


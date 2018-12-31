#include "SoundResource.h"

SoundResource::SoundResource(IXAudio2SourceVoice* sourceVoice)
	: pSourceVoice(sourceVoice)
{
}

SoundResource::~SoundResource()
{
	this->pSourceVoice->DestroyVoice();
}

void SoundResource::Play()
{
	this->pSourceVoice->Start(0);
}

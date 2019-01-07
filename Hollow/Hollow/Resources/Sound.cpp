#include "Sound.h"

namespace Hollow {
	Sound::Sound(IXAudio2SourceVoice* sourceVoice)
		: pSourceVoice(sourceVoice)
	{}

	Sound::~Sound()
	{
		this->pSourceVoice->DestroyVoice();
	}

	void Sound::Play()
	{
		this->pSourceVoice->Start(0);
	}

}

#pragma once
#include <XAudio2.h>
#include "Hollow/Platform.h"

namespace Hollow {
	class  Sound
	{
	public:
		Sound(IXAudio2SourceVoice* sourceVoice);
		~Sound();
		void Play();
	private:
		IXAudio2SourceVoice*		pSourceVoice;
	};
}

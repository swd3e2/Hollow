#pragma once
#include <XAudio2.h>
#include "Hollow/Common/Log.h"
#include <wrl/client.h>
#include "Hollow/Platform.h"
#include "SoundResource.h"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

namespace Hollow {

	class HOLLOW_API AudioEngine
	{
	private:
		IXAudio2MasteringVoice*		masteringVoice;
		IXAudio2*					pXAudio2;
	private:
		HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
		HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	public:
		AudioEngine();
		SoundResource* CreateSoundResource(const char * strFileName);
		~AudioEngine();
	};

	

}
#include "AudioEngine.h"

namespace Hollow {

	HRESULT AudioEngine::ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwRead;
		if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	HRESULT AudioEngine::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());

		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwRIFFDataSize = 0;
		DWORD dwFileType;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;

		while (hr == S_OK)
		{
			DWORD dwRead;
			if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			switch (dwChunkType)
			{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;
				if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
				break;

			default:
				if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
					return HRESULT_FROM_WIN32(GetLastError());
			}

			dwOffset += sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;

			if (bytesRead >= dwRIFFDataSize) return S_FALSE;

		}

		return S_OK;
	}

	AudioEngine::AudioEngine()
	{
		CoInitialize(NULL);
		masteringVoice = nullptr;
		pXAudio2 = nullptr;
		HRESULT hr;
		if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
			Hollow::Log::GetCoreLogger()->critical("Audio Engine: can't create XAudio2 device!");

		if (FAILED(hr = pXAudio2->CreateMasteringVoice(&masteringVoice)))
			Hollow::Log::GetCoreLogger()->critical("Audio Engine: can't create mastering voice!");
	}

	Sound* AudioEngine::CreateSoundResource(const char * strFileName)
	{
		HRESULT hr;
		WAVEFORMATEXTENSIBLE wfx = { 0 };
		XAUDIO2_BUFFER buffer = { 0 };

		// Open the file
		HANDLE hFile = CreateFile(
			strFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
			HRESULT_FROM_WIN32(GetLastError());

		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			HRESULT_FROM_WIN32(GetLastError());

		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		//check the file type, should be fourccWAVE or 'XWMA'
		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD filetype;
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (filetype != fourccWAVE)
			S_FALSE;

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		BYTE * pDataBuffer = new BYTE[dwChunkSize];
		ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

		buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
		buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
		buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
		buffer.LoopCount = 1;
		
		IXAudio2SourceVoice* pSourceVoice;

		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
			Hollow::Log::GetCoreLogger()->critical("Audio Engine: can't create source voice!");

		if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
			Hollow::Log::GetCoreLogger()->critical("Audio Engine: can't submit source buffer!");

		return new Sound(pSourceVoice);
	}

	AudioEngine::~AudioEngine()
	{
		delete masteringVoice;
		delete pXAudio2;
	}
}

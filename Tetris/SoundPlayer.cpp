#include <iostream>
#include "SoundPlayer.h"
#include "SDKwavefile.h"
#include "XAudio.h"
SoundPlayer::SoundPlayer()
	: mpWaveFile(nullptr)
	, mpSourceVoice(nullptr)
	, mXaudioBuffer()
	, mu_pWaveData(nullptr)
	, mVolume(0.5f)
{
}

SoundPlayer::~SoundPlayer()
{
	UnloadAudio();
}

HRESULT SoundPlayer::LoadAudio(LPWSTR strFileName)
{
	HRESULT hr = S_OK;

	if (mpWaveFile != nullptr)
	{
		mpWaveFile->Close();
		mpWaveFile = nullptr;
	}

	mpWaveFile = new CWaveFile();

	if (FAILED(hr = mpWaveFile->Open(strFileName, nullptr, WAVEFILE_READ)))
	{
		wprintf(L"Failed reading WAV file: %#X (%s)\n", hr, strFileName);
		return hr;
	}

	WAVEFORMATEX* pwfx = mpWaveFile->GetFormat(); // Get format of wave file
	DWORD cbWaveSize = mpWaveFile->GetSize(); // Calculate how many bytes and samples are in the wave
	mu_pWaveData = std::make_unique<BYTE[]>(cbWaveSize); // Read the sample data into memory
	if (FAILED(hr = mpWaveFile->Read(mu_pWaveData.get(), cbWaveSize, &cbWaveSize)))
	{
		wprintf(L"Failed to read WAV data: %#X\n", hr);
		return hr;
	}

	if (FAILED(hr = X_AUDIO->CreateSourceVoice(&mpSourceVoice, pwfx)))
	{
		wprintf(L"Error %#X creating source voice\n", hr);
		return hr;
	}

	mXaudioBuffer = { 0 };
	mXaudioBuffer.pAudioData = mu_pWaveData.get();
	mXaudioBuffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source  voice not to expect any data after this buffer
	mXaudioBuffer.AudioBytes = cbWaveSize;

	return hr;
}

void SoundPlayer::UnloadAudio()
{
	if (mpWaveFile != nullptr)
	{
		mpWaveFile->Close();
		mpWaveFile = nullptr;
	}
	if (mpSourceVoice != nullptr)
	{
		mpSourceVoice->DestroyVoice();
		mpSourceVoice = nullptr;
	}
	mXaudioBuffer = { 0 };
	mu_pWaveData.release();
	mu_pWaveData = nullptr;
}

HRESULT SoundPlayer::PlayMusic()
{
	HRESULT hr = S_FALSE;
	if (mpSourceVoice == nullptr)
	{
		return hr;
	}

	XAUDIO2_VOICE_STATE state;
	mpSourceVoice->GetState(&state);
#ifdef _DEBUG
	system("cls");
	std::cout << "BuffersQueued : " << state.BuffersQueued << std::endl;
	std::cout << "pCurrentBufferContext : " << state.pCurrentBufferContext << std::endl;
	std::cout << "SamplesPlayed : " << state.SamplesPlayed << std::endl;
#endif
	hr = S_OK;

	if (!(state.SamplesPlayed > 0))
	{
		hr = ReloadAudio();
		mpSourceVoice->Start(0);
	}
	return hr;
}

HRESULT SoundPlayer::ForcePlayMusic()
{
	HRESULT hr = S_FALSE;
	if (mpSourceVoice == nullptr)
	{
		return hr;
	}

	mpSourceVoice->Stop();
	mpSourceVoice->FlushSourceBuffers();
	ReloadAudio();
	mpSourceVoice->Start(0);

	return hr;
}

void SoundPlayer::IncreaseVolumn()
{
	mVolume += 0.1f;
	if (mVolume > 1)
	{
		mVolume = 1.f;
	}
	mpSourceVoice->SetVolume(mVolume);
}

void SoundPlayer::DecreaseVolumn()
{
	mVolume -= 0.1f;
	if (mVolume < 0)
	{
		mVolume = 0;
	}
	mpSourceVoice->SetVolume(mVolume);
}

void SoundPlayer::SetVolumn(float volume)
{
	if (volume < 0 || volume > 1.0f)
	{
		return;
	}
	mVolume = volume;
	mpSourceVoice->SetVolume(mVolume);
}

HRESULT SoundPlayer::ReloadAudio()
{
	HRESULT hr;

	if (mXaudioBuffer.AudioBytes == 0)
	{
		hr = S_FALSE;
	}

	if (FAILED(hr = mpSourceVoice->SubmitSourceBuffer(&mXaudioBuffer)))
	{
		wprintf(L"Error %#X submitting source buffer\n", hr);
		mpSourceVoice->DestroyVoice();
		mpSourceVoice = nullptr;
	}

	if (SUCCEEDED(hr))
	{
		mpSourceVoice->SetVolume(mVolume);
	}

	return hr;
}

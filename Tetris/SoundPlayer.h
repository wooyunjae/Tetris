#pragma once

#include <xaudio2.h>
#include <memory>
class CWaveFile;

class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

	HRESULT LoadAudio(LPWSTR strFileName);
	void UnloadAudio();
	HRESULT PlayMusic();
	HRESULT ForcePlayMusic();
	void IncreaseVolumn();
	void DecreaseVolumn();
	void SetVolumn(float volumn);

private:
	HRESULT ReloadAudio();

private:
	CWaveFile* mpWaveFile;
	IXAudio2SourceVoice* mpSourceVoice;
	XAUDIO2_BUFFER mXaudioBuffer;
	std::unique_ptr<BYTE[]> mu_pWaveData;

	float mVolume;
};
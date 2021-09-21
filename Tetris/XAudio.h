#pragma once

#include <xaudio2.h>

#define X_AUDIO (XAudio::GetInstance()->GetXAudio())
#define MASTERING_VOICE (XAudio::GetInstance()->GetMasteringVoice())
class XAudio
{
private:
	XAudio();
	~XAudio();

	static XAudio* mpInstance;
public:
	static XAudio* GetInstance();
	static void DestroyInstance();

	IXAudio2* GetXAudio();
	void ReleaseXAudio();
	IXAudio2MasteringVoice* GetMasteringVoice();
	void ReleaseMasteringVoice();
private:
	IXAudio2* mpXAudio;
	IXAudio2MasteringVoice* mpMasteringVoice;
};

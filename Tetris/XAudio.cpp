#include "XAudio.h"

XAudio* XAudio::mpInstance = nullptr;

XAudio::XAudio()
	: mpXAudio(nullptr)
	, mpMasteringVoice(nullptr)
{
}

XAudio::~XAudio()
{
	ReleaseXAudio();
	ReleaseMasteringVoice();
}

XAudio* XAudio::GetInstance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new XAudio();
	}
	return mpInstance;
}

void XAudio::DestroyInstance()
{
	if (mpInstance != nullptr)
	{
		delete mpInstance;
		mpInstance = nullptr;
	}
}

IXAudio2* XAudio::GetXAudio()
{
	if (mpXAudio == nullptr)
	{
		XAudio2Create(&mpXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
		GetMasteringVoice();
	}
	return mpXAudio;
}

void XAudio::ReleaseXAudio()
{
	if (mpXAudio != nullptr)
	{
		mpXAudio->Release();
		mpXAudio = nullptr;
	}
}

IXAudio2MasteringVoice* XAudio::GetMasteringVoice()
{
	if (mpMasteringVoice == nullptr)
	{
		GetXAudio()->CreateMasteringVoice(&mpMasteringVoice);
	}
	return mpMasteringVoice;
}

void XAudio::ReleaseMasteringVoice()
{
	if (mpMasteringVoice != nullptr)
	{
		mpMasteringVoice->DestroyVoice();
		mpMasteringVoice = nullptr;
	}
}

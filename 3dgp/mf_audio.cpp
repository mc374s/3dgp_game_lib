#include "mf_audio.h"

IMFPMediaPlayer* MFAudioManager::pMFPlayer[AUDIO_MAX_NUM] = { nullptr };
bool MFAudioManager::doLoop[AUDIO_MAX_NUM] = { false };
bool MFAudioManager::isLoaded = false;

MFAudioManager::MFAudioManager()
{

}

MFAudioManager::~MFAudioManager()
{
	MFAudiosRelease();
}

void MFAudioManager::loadAudios(const RESOURCE_DATA _data[])
{
	if (!MFAudioManager::isLoaded)
	{
		for (int i = 0; i < AUDIO_MAX_NUM; i++) {
			if (_data[i].fileNO >= 0 && _data[i].fileName) {
				MFAudioLoad(_data[i].fileNO, _data[i].fileName);
			}
			if (_data[i].fileNO < 0 || _data[i].fileName == nullptr) {
				break;
			}
		}
		MFAudioManager::isLoaded = true;
	}
}

void MFAudiosRelease()
{
	for (int i = 0; i < AUDIO_MAX_NUM; i++) {
		if (MFAudioManager::pMFPlayer[i]) {
			MFAudioManager::pMFPlayer[i]->Release();
		}
	}
	//ZeroMemory(data, sizeof(AUDIO_DATA)*AUDIO_MAX_NUM);
}

void MFAudioCheckLoops()
{
	MFP_MEDIAPLAYER_STATE state = MFP_MEDIAPLAYER_STATE_EMPTY;
	
	for (int i = 0; i < AUDIO_MAX_NUM; i++) {
		if (MFAudioManager::pMFPlayer[i]) {
			MFAudioManager::pMFPlayer[i]->GetState(&state);
			if (state == MFP_MEDIAPLAYER_STATE_STOPPED && MFAudioManager::doLoop[i]){
				MFAudioManager::pMFPlayer[i]->Play();
			}
		}
	}
}

void MFAudioLoad(const int _fileNO, const char* _fileName)
{
	const size_t cSize = strlen(_fileName) + 1;
	wchar_t *wcFileName = new wchar_t[cSize];

	//size_t temp;
	mbstowcs_s(nullptr, wcFileName, cSize, _fileName, cSize);

	HRESULT hr = MFPCreateMediaPlayer(wcFileName, FALSE, 0, NULL,/* hwnd*/NULL, &MFAudioManager::pMFPlayer[_fileNO]);
	delete[] wcFileName;
	if (FAILED(hr)){
		MessageBox(0, L"MFPCreateMediaPlayer Failed.", L"mf_audio.cpp/MFAudioLoad()", MB_OK);
		exit(-1);
	}
}

void MFAudioPlay(const int _fileNO, const bool _doLoop)
{
	if (MFAudioManager::pMFPlayer[_fileNO]) {
		MFAudioManager::pMFPlayer[_fileNO]->Stop();
		MFAudioManager::pMFPlayer[_fileNO]->Play();
		MFAudioManager::doLoop[_fileNO] = _doLoop;
	}
}

void MFAudioContinue(const int _fileNO)
{
	if (MFAudioManager::pMFPlayer[_fileNO]) {
		MFAudioManager::pMFPlayer[_fileNO]->Play();
	}
}

void MFAudioStop(const int _fileNO)
{
	if (MFAudioManager::pMFPlayer[_fileNO]) {
		MFAudioManager::pMFPlayer[_fileNO]->Stop();
		MFAudioManager::doLoop[_fileNO] = false;
	}
}

void MFAudioPause(const int _fileNO)
{
	if (MFAudioManager::pMFPlayer[_fileNO]){
		MFAudioManager::pMFPlayer[_fileNO]->Pause();
	}
}

bool isMFAudioPlaying(const int _fileNO)
{
	if (MFAudioManager::pMFPlayer[_fileNO]) {
		MFP_MEDIAPLAYER_STATE state = MFP_MEDIAPLAYER_STATE_EMPTY;
		MFAudioManager::pMFPlayer[_fileNO]->GetState(&state);
		if (state == MFP_MEDIAPLAYER_STATE_PLAYING)
		{
			return true;
		}
	}
	return false;
}
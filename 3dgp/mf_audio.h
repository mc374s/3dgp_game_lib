#ifndef _MFAUDIO_H_
#define _MFAUDIO_H_
/*************Media Foundation Audio SpacePlayer*******************/

#pragma comment(lib,"Mfplat")
#pragma comment(lib,"mfplay")
#include <mfapi.h>
#include <mfplay.h>

#define AUDIO_MAX_NUM (64)

struct RESOURCE_DATA
{
	int					fileNO;			// âπê∫î‘çÜ
	char*				fileName;		// ÉtÉ@ÉCÉãñº
};

class MFAudioManager
{
private:
	static IMFPMediaPlayer* pMFPlayer[AUDIO_MAX_NUM];	// MediaFoundationPlayer
	static bool doLoop[AUDIO_MAX_NUM];					// loop flag
public:
	MFAudioManager();
	~MFAudioManager();

	static bool isLoaded;

	static MFAudioManager* getInstance() {
		static MFAudioManager instance;
		return &instance;
	};

	void loadAudios(const RESOURCE_DATA a_pData[]);
	friend void MFAudiosRelease();
	friend void MFAudioCheckLoops();

	friend void MFAudioLoad(const int a_fileNO, const char* a_pFileName);
	friend void MFAudioPlay(const int a_fileNO, const bool a_doLoop = false);
	friend void MFAudioContinue(const int a_fileNO);
	friend void MFAudioStop(const int a_fileNO);
	friend void MFAudioPause(const int a_fileNO);
	friend bool isMFAudioPlaying(const int a_fileNO);

};

#define pMFAudioManager (MFAudioManager::getInstance())



#endif // !_MFAUDIO_H_

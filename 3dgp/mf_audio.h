#ifndef _MFAUDIO_H_
#define _MFAUDIO_H_
/*************Media Foundation Audio Player*******************/

#pragma comment(lib,"Mfplat")
#pragma comment(lib,"mfplay")
#include <mfapi.h>
#include <mfplay.h>

namespace GLC{

#define AUDIO_MAX_NUM (64)

struct NUMBERED_FILE
{
	int		fileNO;		
	char* 	fileName;	
};

class MFAudioManager
{
private:
	static IMFPMediaPlayer* pMFPlayer[AUDIO_MAX_NUM];	// MediaFoundationPlayer
	static bool				doLoop[AUDIO_MAX_NUM];		// loop flag
	MFAudioManager();
	~MFAudioManager();
public:

	static bool isLoaded;

	static MFAudioManager* getInstance() {
		static MFAudioManager instance;
		return &instance;
	};

	void loadAudios(const NUMBERED_FILE pData[]);

	friend void MFAudiosRelease();
	friend void MFAudioCheckLoops();

	friend void MFAudioLoad(const int fileNO, const char* pFileName);
	friend void MFAudioPlay(const int fileNO, const bool doLoop = false);
	friend void MFAudioContinue(const int fileNO);
	friend void MFAudioStop(const int fileNO);
	friend void MFAudioPause(const int fileNO);
	friend bool isMFAudioPlaying(const int fileNO);

};

}

#endif // !_MFAUDIO_H_

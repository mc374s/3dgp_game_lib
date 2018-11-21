#ifndef _SOUND_DATA_H_
#define _SOUND_DATA_H_

#include "../3dgp/mf_audio.h"

enum AUDIO_NO
{
	BGM_TITLE,
	BGM_MAIN,

	SE_START,
};

extern GLC::NUMBERED_FILE audio_data[];

#endif // !_SOUND_DATA_H_

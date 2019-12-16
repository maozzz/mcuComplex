/*===========================================================================
 *
 *  Audio library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File aplayer.h: Audio player definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __APLAYER_H
#define __APLAYER_H

#include <globaldef.h>
#include "../../bastypes.h"
#include "../../std/inc/wavfile.h"
#include "filesys.h"


#ifndef APLAYER_DAC_NN
#define APLAYER_DAC_NN		0
#endif /* !APLAYER_DAC_NN */

//Audio file flags (AudioFile::flags)
#define AFILE_FLAGS_OPENED			0x01
#define AFILE_FLAGS_PAUSED			0x02

//Action codes
#define APLAYER_ACTION_PLAY			0
#define APLAYER_ACTION_PAUSE		1
#define APLAYER_ACTION_STOP			2


/* */
typedef struct tag_AudioFile
{
	//Flags (see AFILE_FLAGS_x)
	u32 flags;

	//Audio data offset at file stream
	u32 dataOffset;

	//Format
	WavFormat format;

	//File stream
	FSFILE    stream;
} AudioFile;

/* */
AudioFile *OpenAudioFile(const char *filename, AudioFile *af);

/* */
int CloseAudioFile(AudioFile *af);

/* Call once at main cycle */
int ReadAudioFile(AudioFile *af);

/* Call with frequency 8KHz */
void PlayAudioFileSample();

/* */
int DoAPlayerAction(AudioFile *af, u32 action);

#endif /* __APLAYER_H */

/*===========================================================================
 * End of file aplayer.h
 *===========================================================================*/

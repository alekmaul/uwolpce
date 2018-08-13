#ifndef _UWOL_SOUND_
#define _UWOL_SOUND_

//#include <st.h>

#define MAXMUSICS		6
#define MAXEFFECTS	7
#define MAXSOUNDS 	MAXMUSICS+MAXEFFECTS   

#define MUSMAIN			0
#define MUSMAP			1
#define MUSTIMEOUT	2
#define MUSGAMOVER	3
#define MUSGAMWIN		4
#define MUSGAMLOST	5

#define SNDJUMP			6
#define SNDTAKEGOL	7
#define SNDEXIT			8
#define SNDHITGRD		9
#define SNDBONUS		10
#define SNDHURT			11
#define SNDDIE			12


extern void play_snd(unsigned char sndidx);

#endif

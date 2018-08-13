#include "uwol_snd.h"

//extern struct st_header titlescreen[];

//#include "uwol_snd_const.inc"

//void st_set_song(unsigned char song_bank, struct st_header *song_addr);

const unsigned char  effectok[MAXSOUNDS]={
	0x81,			// title
	0x82,			// gamemap
	0x83,			// timeout
	0x84,			// gameover
	0x85,			// gamewin
	0x86,			// gamelost
	
	2,	// jump
	3,	// take gold
	4,	// exit
	5,	// hit ground
	6,	// bonus
	0,	// hurt
	0 	// dye
};

// For squirrel engine
int  psgMainStatus;
int  psgSubStatus;

//---------------------------------------------------------------------------------
void init_snd() {
   //st_init();
	// Phase d'init du timer et des voix pour jouer les samples */
#if 0	
	Init_USER_Timer_IRQ();								/* Init du TIMER */
	Init_Voix_Sample();
#else
	// Init squirrel engine
	psgInit(5);
	psgOn(0); 			// use timer for now : vsync works okay, but limits speed 
	psgPlay(0);
	sngInit();
	psgDelay( 0 );
#endif
}

void play_snd(unsigned char sndidx) {
	unsigned char i;

	i=effectok[sndidx];
	
	// if sound ok
	if (i !=0) {
		if ((i & 0x80) == 0x00) { // Not a music, a sound effect
/*
                    #asm
                    ldw_xy 	#_snd_jump
                    lda		#00
                    jsr		Joue_Sample
                    #endasm
*/
			psgPlay(i);
    }
		else { // yeah, it's a music
			i = i & 0x1F; 
			psgPlay(i);
		}
	}
}

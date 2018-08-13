#include "main.h"
#include "buildnum.h"

/*
#include "pcmsnds.h"

#asm
	.data
    .bank   6
    .org    $6000
	.code
#endasm
*/

#incbin(titletil,"gfxs/title.gfx");
#incbin(titlemap,"gfxs/title.map");
#incbin(titlepal,"gfxs/title.pal");

#incbin(creditstil,"gfxs/credits.gfx");
#incbin(creditsmap,"gfxs/credits.map");
#incbin(creditspal,"gfxs/credits.pal");

#incbin(mojontil,"gfxs/mojon.gfx");
#incbin(mojonmap,"gfxs/mojon.map");
#incbin(mojonpal,"gfxs/mojon.pal");

#incbin(ubhrestil,"gfxs/ubhres.gfx");
#incbin(ubhresmap,"gfxs/ubhres.map");
#incbin(ubhrespal,"gfxs/ubhres.pal");

#incbin(finbadtil,"gfxs/finbad.gfx");
#incbin(finbadmap,"gfxs/finbad.map");
#incbin(finbadpal,"gfxs/finbad.pal");

#incbin(fingootil,"gfxs/fingoo.gfx");
#incbin(fingoomap,"gfxs/fingoo.map");
#incbin(fingoopal,"gfxs/fingoo.pal");

#incbin(finendtil,"gfxs/finend.gfx");
#incbin(finendmap,"gfxs/finend.map");
#incbin(finendpal,"gfxs/finend.pal");

#incbin(govertil,"gfxs/gover.gfx");
#incbin(govermap,"gfxs/gover.map");
#incbin(goverpal,"gfxs/gover.pal");

#incbin(titlehstil,"gfxs/titlehs.gfx");
#incbin(titlehspal,"gfxs/titlehs.pal");

#incbin(titleb1til,"gfxs/titleb1.gfx");
#incbin(titleb1pal,"gfxs/titleb1.pal");

#incbin(tilesatil,"gfxs/tilesa.gfx");
#incbin(tiles0til,"gfxs/tiles0.gfx");
#incbin(tilesbtil,"gfxs/tilesb.gfx");
#incbin(tilesctil,"gfxs/tilesc.gfx");
#incbin(tilesapal,"gfxs/tilesa.pal");
#incbin(tiles0pal,"gfxs/tiles0.pal");
#incbin(tilesbpal,"gfxs/tilesb.pal");
#incbin(tilescpal,"gfxs/tilesc.pal");

#incbin(borderstil,"gfxs/borders.gfx");
#incbin(spritestil,"gfxs/sprites.gfx");
#incbin(spritespal,"gfxs/sprites.pal");

#if 0
#include "pcmsnds.h"
#endif

// Squirrel MML engine
#include "sound/psglib.c"
#include "snginit.c"

#include "fadeout.c"
#include "uwol_lvl.c"
#include "uwol_fct.c"
#include "uwol_snd.c"
#if 0
#include "pcm_dda.c"
#endif


const char scopy[]=
{
	"Uwol. PCE version, ROM ",
	BUILDID,
	", coded by Jean-Michel Girard (Alekmaul), for Community."
};

//#incasm("titlescreen.asm")

// main variables
unsigned char fl;					// to determine which step in game

INERCIA player;						// current player attributes

unsigned char playing;
unsigned char maincounter;
unsigned char visited[55];					// to know each room visited
unsigned char n_pant;						// current level
unsigned char level;							// current level
unsigned char tchpad;						// Current key 

signed char timee;		  // time to do level
unsigned char num_monedas;					// nb of coins
unsigned char salida;						// test for end of playing
unsigned char flag1;							// test for some purpose
unsigned char fantact;						// 1 when ghost is here
unsigned char vidaextra;						// 1 if player can be kill
unsigned char xcami, ycami;					// to get aagain the shirt
unsigned char bonus1,bonus2;					// flag for bonus test

unsigned char fdcami[4];						// save bg of cami

unsigned char xp,yp,t,sx,sy/*i,j,*/;		// loop variables
unsigned char idfant;
unsigned int monedas_frame;
unsigned char idgfx;							// gfx index for border

unsigned int total_score;				// current score
unsigned int total_coins;				// nb of coins
unsigned char lives;							// nb of lvies

unsigned char sp_attr_buf[/*24*/14*22];			// each level (yes, huge size for coleco :/)

unsigned int palwrap[32];

signed char arrow_yoff;

MONEDA monedas[10];
MONEDA *pMoneda;
MOVILES moviles[3];
MOVILES *pMovil;

// for squirrel engine
int VSyncCnt;
int TimerCnt;
int MainCnt;
int SubCnt;

//---------------------------------------------------------------------------------
void updatePalBG(void) {
	set_bgpal(0, tilesapal);
	set_bgpal(1, tiles0pal);
	set_bgpal(2, tilesbpal);
	set_bgpal(3, tilescpal);
	set_sprpal(0,spritespal);
}

//---------------------------------------------------------------------------------
void initVramGame(void) {
	// Init tiles 
	load_vram( 0x1000, tilesatil , 128*8/2);
	load_vram( 0x1000+128*8/2, tiles0til , 64*8/2);
	load_vram( 0x1000+128*8/2+64*8/2, tilesbtil , 128*8/2);
	load_vram( 0x1000+128*8/2+64*8/2+128*8/2, tilesctil , 192*8/2);
	updatePalBG();
}

//---------------------------------------------------------------------------------
// Manage keys and joystick at the same time: 
void joyfunc(void) {
	unsigned short joypad_1;
	
	tchpad = 0;
	joypad_1=joy(0);
	if (joypad_1 & JOY_LEFT)   tchpad|=sp_LEFT;
	if (joypad_1 & JOY_RIGHT)  tchpad|=sp_RIGHT;
	if (joypad_1 & JOY_UP)     tchpad|=sp_UP;
	if (joypad_1 & JOY_DOWN)   tchpad|=sp_DOWN;
	if (joypad_1 & (JOY_I | JOY_II)) tchpad|=sp_FIRE;
	if (joypad_1 == JOY_RUN)	 tchpad|=sp_START;
}

const unsigned char xofslogo[]={
	-40,-30,-20,-10,0,5,-30,-20,-10,0,5,-20,-10,0,5,-10,0,5,0,5,
	0xFF
};

//---------------------------------------------------------------------------------
void uwol_showscreen(unsigned char num) {
	unsigned char k,r,g,b;
	signed char j;
	unsigned int i, *palentry, tmpscr;
	
	disp_off();
	cls();

	// draw intro
	if (num==0) {  // credits 
		load_background(creditstil, creditspal, creditsmap, 32, 32);
	}
	else if (num==1) {// title
		load_background(titletil, titlepal, titlemap, 32, 32);
		
		// load title for white effect
		load_vram( TILEVRAM+0x1400 , titlehstil , 0x200*4);
		set_sprpal(1,titlehspal);
		for (i=0;i<4;i++) {
			spr_set( 8+i );
			spr_pattern( 0x2400 +(0x200*i));
			spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_32x64|NO_FLIP );
			spr_x(79+i*32);
			spr_y(5);
			spr_pal( 1 );
			spr_pri( 1 );
		}

		// load animated bars
		load_vram( SPRVRAM , titleb1til , 0x200*8);
		set_sprpal(0,titleb1pal);
		for (i=0;i<8;i++) {
			spr_set( i );
			spr_pattern( SPRVRAM +(0x200*i));
			spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_32x64|NO_FLIP );
			spr_x(64+(i&3)*32);
			spr_y(56+(i>>2)*64);
			spr_pal( 0 );
			spr_pri( 0 );
    }
    satb_update();
	}
	else if (num==2) { // game over
		load_background(govertil, goverpal, govermap, 32, 32);
	}
	else if (num==3) { // the end
		load_background(finendtil, finendpal, finendmap, 32, 32);
	}
	else if (num==4) { // not enough coins
		load_background(finbadtil, finbadpal, finbadmap, 32, 32);
	}
	else if (num==5) { // correct end
		load_background(fingootil, fingoopal, fingoomap, 32, 32);
	}
	else if (num==6) { // mojons
		load_background(mojontil, mojonpal, mojonmap, 32, 32);
	}	
	else if (num==7) { // ubhres
		load_background(ubhrestil, ubhrespal, ubhresmap, 32, 32);
	}
	disp_on();

	//mute_all();
	if (num==1) {
		// for scroll palette
		for (i=0;i<16;i++) {
			palwrap[i] = titleb1pal[i];
		}
		// anim logo
		k=0;
		while (xofslogo[k] != 0xFF) {
			j=xofslogo[k];
			k++;
			for (i=0;i<4;i++) {
				spr_set( 8+i );
				spr_y(j);
			}
			satb_update();
			vsync(2);
		}
		// do white effect
		for (j=0;j<7;j++) {
			for (i=256+16;i<256+16+16;i++) {
				b= get_color(i) & 7;
				r= (get_color(i)>>3) & 7;
				g= (get_color(i)>>6) & 7;
				if (r<7) r++;
				if (g<7) g++;
				if (b<7) b++;
				set_color_rgb(i, r, g, b);
			}
			vsync(4);
		}
		set_sprpal(1,titlehspal);
		play_snd(MUSMAIN);
	}
	else {
		play_snd (SNDJUMP);
	}

	// Wait for *
	j=0; k=0;tmpscr=0;
	while(1) {
    joyfunc();

		vsync(2);

		if ( (tchpad & (sp_FIRE|sp_START)) != 0) {
			break;
		}
		// check for end of song, and loop audio if so
		if (num==1) {
			// scroll palette
			for (i=0;i<12;i++) {
				palentry=titleb1pal;
				palentry+=(((i+j)%12)+2);
				palwrap[i+2] = *palentry;
			}
			set_sprpal(0,palwrap);
			load_vram(BGVRAM+23*32*2,titlemap+((23+k)<<6), 32);
			if ( (j & 7) == 0) {
				k=1-k;
			}
			j++;
		}
		else {
			if ( (num>=6) || (num==0) )  {
				tmpscr++;
				if (tmpscr>(5*50))
					break;
			}
		}
  }

	//mute_all(); 
	//psgOff(0);
	if (num==1) {
		for (i=0;i<8;i++) {
			spr_set( i );
			spr_hide();
		}
		satb_update();
	}
	fade_out ();

	// wait key released
	while ( (tchpad & (sp_FIRE|sp_START)) != 0) { joyfunc(); vsync(1); }
}

//---------------------------------------------------------------------------------
void uwol_main (void) {
	unsigned char i;

	// Intro of game
	uwol_showscreen(1);
	
	// Prepare VRAM for game
	initVramGame();

	// Begin of game
	n_pant = 0;
	level = 1;
	total_score = 0;
	total_coins = 0;
	lives = 1;//cheat?99:3;
	flag1 = 0;
	fl = 0;
	bonus1 = 0;
	bonus2 = 0;

	for (i = 0; i < 55; i ++ )  {
		visited[i] = 0;
	}

//		level = 10; 	total_coins = 257; // debug feature

	while (fl < 3)  {
		pyramid ();

		game();	

		if (fl == 0) {
			visited[n_pant] = 1;     //We mark as visited the display cur ..
			n_pant = n_pant + level;
			level ++;
		} 
		else if (fl == 1) {
			visited[n_pant] = 1;     // We mark as visited the display cur ..
			n_pant = n_pant + level + 1;
			level ++;
		} 
		else if (fl == 3) {				// game over
			play_snd (MUSGAMOVER);
			uwol_showscreen(2);
    }
 
		//level = 11; //total_coins = 450; // debug
		if (level == 11) {
			// We left the pyramid, we will calculate whether or not we really.
			if (total_coins < 256) {
				n_pant = 0;
				level = 1;
				for (i = 0; i < 55; i ++ )
					visited [i] = 0;
				play_snd (MUSGAMLOST);
				uwol_showscreen(4);
			} 
			else {
				fl = 4;
				play_snd (MUSGAMWIN);
				uwol_showscreen(5);
			}
			// restore graphiques
			if (fl != 4) {
				initVramGame();
			}
		}
		vsync(1);
	}
	if (fl == 4) {
		// The end!
		uwol_showscreen(3);
	}
}

//----------------------------------------------------------------------------------------
void init_gfx() {
	init_satb();
	set_xres(256); // set_screen_size(SCR_SIZE_32x32);
}

//---------------------------------------------------------------------------------
void uwol_preinit_variables(void) {
	playing=1;
	maincounter=0;
}

//---------------------------------------------------------------------------------
int main() {
	// Init graphic mode
	init_snd();

	// Init graphic mode
	init_gfx();
	
	// Init vars
	uwol_preinit_variables();

	// show mojons screen
	uwol_showscreen(6);

	// show ubhres screen
	uwol_showscreen(7);

	// show credit screen
	uwol_showscreen(0);
	
	for (;;) {
		uwol_main();
	}
}

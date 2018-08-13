#ifndef _UWOL_MAIN_
#define _UWOL_MAIN_

#include <huc.h>
#include <st.h>

#define BGVRAM			0x0000
#define SPRVRAM			0x4000
#define TILEVRAM		0x1000

#define sp_START		0x20
#define sp_FIRE			0x10
#define sp_RIGHT		0x08
#define sp_LEFT			0x04
#define sp_DOWN			0x02
#define sp_UP			0x01

#define EST_NORMAL       0
#define EST_NUDE         1
#define EST_PARP         2
#define EST_MUR          4
#define EST_EXIT         8
#define EST_EXIT_LEFT   16
#define EST_EXIT_RIGHT  32

#define SPTW 4/7  //4/7    //2/3

#define SPTWM	3/7

#define HFLIP           (0x8000)

#define uwol_r_1a       0*64 // 152
#define uwol_r_2a       1*64 // 156
#define uwol_r_3a       2*64  // 160
#define uwol_l_1a       0*64|HFLIP // 152
#define uwol_l_2a       1*64|HFLIP // 156
#define uwol_l_3a       2*64|HFLIP // 160
#define wolfi_1a        3*64 // 164
#define franki_1a       5*64 // 168
#define vampi_1a        7*64 // 172
#define fanti_r_1a      9*64 // 176
#define fanti_l_1a      9*64 // 176
#define uwolpelot_r_1a  11*64 // 180
#define uwolpelot_r_2a  12*64 // 184
#define uwolpelot_r_3a  13*64 // 188
#define uwolpelot_l_1a  11*64|HFLIP // 180
#define uwolpelot_l_2a  12*64|HFLIP // 184
#define uwolpelot_l_3a  13*64|HFLIP // 188
#define uwolmuete_1a    14*64 // 192
#define arrow_1a        15*64  //196
#define coin_1a					16*64 // 200
#define shirt_lr				28*64

#define borders_lr      32*64 // 200

typedef struct {
    signed int x, y;
    signed int vx, vy;
    signed char g, ax, rx;
    unsigned char salto, cont_salto;
    unsigned char saltando;
    unsigned int frame;
    unsigned char subframe, facing;
    unsigned char estado;
    unsigned char ct_estado;
    unsigned char ground,nojump;
} INERCIA;

typedef struct
{
unsigned char y;
unsigned char x;
unsigned char pattern;
unsigned char colour;
} sprite_t;

// Stores the current level of the coins
typedef struct {
    unsigned char type;
    unsigned char x,y;
    //sp_SS *sp;
} MONEDA;

// Estructura de datos para los objetos móviles:
typedef struct {
    unsigned char x, y;
    signed char vx, vy;
    unsigned char t1, t2;
    signed int rx, ry;
    unsigned int current_frame, next_frame;
	unsigned char tipo;
} MOVILES;

extern unsigned char fl;					// to determine which step in game

extern unsigned char sp_attr_buf[/*24*/14*22];			// each level (yes, huge size for coleco :/)

extern unsigned char playing;
extern unsigned char maincounter;
extern unsigned char visited[55];					// to know each room visited
extern unsigned char n_pant;							// current level
extern unsigned char level;								// current level
extern unsigned char tchpad;							// Current key 

extern unsigned char xp,yp,t,sx,sy/*i,j,*/;		// loop variables
extern unsigned char idfant;
extern unsigned int monedas_frame;

extern signed char timee;		  						// time to do level
extern unsigned char num_monedas;					// nb of coins

extern unsigned int total_score;					// current score
extern unsigned int total_coins;					// nb of coins
extern unsigned char lives;								// nb of lvies

extern unsigned char salida;						// test for end of playing
extern unsigned char flag1;							// test for some purpose
extern unsigned char fantact;						// 1 when ghost is here
extern unsigned char vidaextra;						// 1 if player can be kill
extern unsigned char xcami, ycami;					// to get aagain the shirt
extern unsigned char bonus1,bonus2;					// flag for bonus test

extern unsigned char fdcami[4];						// save bg of cami

extern signed char arrow_yoff;

extern INERCIA player;						// current player attributes

extern MONEDA monedas[10];
extern MONEDA *pMoneda;
extern MOVILES moviles[3];
extern MOVILES *pMovil;

#endif

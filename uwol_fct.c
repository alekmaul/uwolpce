//#include <huc.h>

#include "uwol_fct.h"

#include "main.h"
#include "uwol_lvl.h"
#include "uwol_snd.h"

const unsigned int monedas_anim[]= {
  coin_1a,coin_1a+1*64,coin_1a+2*64,coin_1a+2*64,coin_1a+1*64,coin_1a,coin_1a+3*64,coin_1a+4*64,coin_1a+4*64,coin_1a+3*64
};

const unsigned int monedas_take_anim[]= {
  coin_1a,coin_1a+1*64,coin_1a+2*64,coin_1a+5*64,coin_1a+6*64,coin_1a+7*64,coin_1a+8*64,coin_1a+9*64,coin_1a+10*64,coin_1a+11*64
};

const unsigned int player_anim[]= {
	uwol_r_1a,uwol_r_2a,uwol_r_3a,uwol_r_2a,
	uwol_l_1a,uwol_l_2a,uwol_l_3a,uwol_l_2a,
	uwolpelot_r_1a,uwolpelot_r_2a,uwolpelot_r_3a,uwolpelot_r_2a,
	uwolpelot_l_1a,uwolpelot_l_2a,uwolpelot_l_3a,uwolpelot_l_2a
};

// Test vertical collision
unsigned char collision_v(unsigned char x1,unsigned char y1,unsigned char h1) {
	unsigned char is;

	h1--;
	h1=((y1+h1)>>3)-(y1>>3)+1;

	x1=(x1>>3)-2;
	y1=(y1>>3)-2;

	// check regarding x1 to have more memory
	if (x1 & 1) {
		x1=x1>>1;
		for(is=0;is<h1;is++) {
			if ((sp_attr_buf[x1+14*y1] & 0x0f) != 0) 
				return 1;
			y1++;
		}
	}
	else {
		x1=x1>>1;
		for(is=0;is<h1;is++) {
			//if (sp_attr_buf[x1+24*y1] != 0) 
			if ((sp_attr_buf[x1+14*y1] & 0xf0) != 0) 
				return 1;
			y1++;
		}
	}

	return 0;
}

// Test horizontal collision
unsigned collision_h(unsigned char x1,unsigned char y1,unsigned char w1) {
	unsigned char i;

	w1--;
	w1=((x1+w1)>>3)-(x1>>3)+1;

	x1=(x1>>3)-2;
	y1=(y1>>3)-2;
	for(i=0;i<w1;i++) {
	if (x1 & 1) {
		if ((sp_attr_buf[(x1>>1)+14*y1] & 0x0f) != 0) 
			return 1;
	}
	else {
		if ((sp_attr_buf[(x1>>1)+14*y1] & 0xf0) != 0) 
			return 1;
	}
	x1++;
	}
  return 0;
}


//---------------------------------------------------------------------------------
// Move player
void move(void) {
	unsigned char xx,yy;
    unsigned char off;

	// First test engine inertia to movement z88dk / splib2 
	// Let's see if this does not exploit tó 
	//What parts. First vertical movement. The equation of motion comes to be, in each cycle: 
	// 1. vy = vy + g 
	// 2. y = y + vy 
	// So the speed affected by gravity. To sneak with no numbers, we limiters:
  if (player.vy < 512*SPTW/*256*/)
		player.vy += player.g;
	else
		player.vy = 512*SPTW/*256*/;

	player.y += player.vy;
	if (player.y <= 0)
		player.y = 0;
 
	// The problem is that it is not easy ... You have to see if we do not collide. 
	// If this happens, you have to "recoil" to the edge of the obstacle. 
	// So we look at the sign of v, so that the calculations are simpler. 
	// In passing we will precompute a few things to make it go faster. 
	xp = (unsigned char) (player.x>>6);              // to divide by 64 pixels, and then between 8 for tiles.
	yp = (unsigned char) (player.y>>6);
	//xx = x >> 3;
	yy = yp >> 3;

	if (player.vy < 0) {            // We are ascending
		if (player.y >= 1024)
			if(collision_h(xp+2,yp+4,16-4)) {
				// paramos y ajustamos:
				player.y = player.y-player.vy;
				player.vy = 0;
			}
		player.ground=0;
	} 
	else if (player.vy > 0) {     // we are descending
		if(collision_h(xp+2,yp+16,16-4)) {
			// paramos y ajustamos:
			player.vy = 0;
			player.y = yy << 9;
			if(player.ground == 0) {
				player.ground=1;
				player.nojump=1;
				play_snd (SNDHITGRD);
			}
		}
		else {
			player.ground=0;
		}
	}

	// Jump: The jump is reduced to give a negative value to vy. This is the most 
	// simple. However, for more control, use the break type "mario bros". 
	// To do this, each press let vy decrease to a minimum, and determine 
	// tectando not pressed again when we are in the air. Set of banners;) 
	if (tchpad & sp_FIRE) {
		if ( (player.nojump==0) && (player.vy == 0) && (player.saltando == 0) && (player.ground != 0) ) {
			player.saltando = 1;
			player.cont_salto = 0;
			play_snd (SNDJUMP);

			// To make the start of the jump more powerful: (sometimes convenient):
			// player.vy -= (player.salto << 1);
		}
		else if ( player.saltando ) {
			player.vy -= (player.salto + 48*SPTW/*16*/ - (player.cont_salto>>1));
			if (player.vy < -256/*128*/) player.vy = -256/*128*/;

			player.cont_salto ++;
			if (player.cont_salto == 6/*8*/)
				player.saltando = 0;
		}
	}
	else {
		player.saltando = 0;
		player.nojump=0;
	}

	// Ok with vertical movement. 
	// Horizontal movement: 
 	// As you press an arrow key, 
 	//  x = x + vx 
	//  vx = vx + ax 
	// If nothing is pressed: 
	//  x = x + vx 
	//  vx = vx - rx 
    //if ( ! ((i & sp_LEFT) /*== 0*/ || (i & sp_RIGHT) /*== 0*/))
	if ( (((tchpad & sp_LEFT) == 0) && ((tchpad & sp_RIGHT) == 0)) ) {
		if (player.vx > 0) {
				player.vx -= player.rx;
				if (player.vx < 0)
						player.vx = 0;
		} else if (player.vx < 0) {
			player.vx += player.rx;
			if (player.vx > 0)
					player.vx = 0;
		}
	}
    if ((tchpad & sp_LEFT) /*== 0*/) {
        if (player.vx > -128/*-192*/*SPTW) {
            player.facing = 0;
            player.vx -= player.ax;
        }
	}
	if ((tchpad & sp_RIGHT) /*== 0*/) {
			if (player.vx < 128/*192*/*SPTW) {
					player.vx += player.ax;
					player.facing = 1;
			}
	}
	player.x = player.x + player.vx;

	// Now, as before, we see if we hit something, and if 
	// stop and reculamos 
	yp = (unsigned char) (player.y>>6);
	xp = (unsigned char) (player.x>>6);
	//yy = y >> 3;
	//xx = x >> 3;

	if (player.y >= 512/*1024*/) {
		if (player.vx < 0) {
				if(collision_v(xp+2,yp+4,16-4)) {
						// paramos y ajustamos:
						player.x = player.x-player.vx;
						player.vx = 0;
				}
		} 
		else {
            if(collision_v(xp+16-2,yp+4,16-4)) {
                // paramos y ajustamos:
                player.x = player.x-player.vx;
                player.vx = 0;
            }
        }
	}

	// We calculate the frame to be put:
	if(player.vx!=0) 
		player.subframe++;
		off=(player.subframe>>1)&3;
    if(player.facing==0) off+=4;
    if(player.estado&EST_NUDE) off+=8;
    player.frame=player_anim[off];

    // Wrap around
    if (player.x <= 1088)
		player.x = 14270;
 
    if (player.x >= 14272)      
		player.x = 1090;
	
    // Die in a pit
    if (player.y > 10752) {
			player.frame = uwolmuete_1a;
			player.estado |= EST_MUR;
			player.vy = - 7*SPTW * (player.salto);
			player.y = 10750;
			lives--;
			if (lives != 0xff) 
				draw_lives ();
			play_snd (SNDDIE);
    }
 
    // Leaving the stage
     if ((tchpad & sp_DOWN) /*== 0*/) {
        if ( player.estado & EST_EXIT ) {
            if ( (player.x > 2204) && (player.x < 2880) ) {
                player.estado |= EST_EXIT_LEFT;
				salida = 0;
				playing = 0;
			}
            if ( player.x > 12480 && player.x < 13120 ) {
                player.estado |= EST_EXIT_RIGHT;
				salida = 1;
				playing = 0;
			}
        }
    }
}

void death_sequence () {
	if (player.vy < 1024*SPTW/*256*/)
			player.vy += player.g;
	else
			player.vy = 1024*SPTW/*256*/;
	player.y += player.vy;

	// This ends when the player leaves the screen:
	if (player.y > 11264) {
			player.estado = EST_PARP;
			player.x = 32 << 6;
			player.y = 144 << 6;
			player.vx = player.vy = 0;
			player.ct_estado = 32*3/2;                      // Time immunity.
			flag1 = 3;
			playing = 0;
	}
}

#define abs8(x) (x < 0 ? -x : x)

// Mov ennemies
void move_moviles () {
	unsigned char is;

	// This moves the three mobile (should be defined, that is, with type! = 0
	for (is = 0; is < 3; is ++) {
		pMovil=&moviles[is];
		spr_set( 12+1+is ); 
		if ( pMovil->tipo == 3) {
			// Fanty
			if ((maincounter &  15)==0) {
				pMovil->current_frame = (pMovil->current_frame == pMovil->next_frame ? pMovil->next_frame-64 : pMovil->next_frame);
				spr_pattern( SPRVRAM+pMovil->current_frame); 
			}				
			if ( (player.x > pMovil->rx) && (pMovil->vx < (120*SPTWM) ))  {
				pMovil->vx +=4*SPTWM;
				spr_ctrl( FLIP_MAS , NO_FLIP );
			}
			else if ( (player.x < pMovil->rx) && (pMovil->vx > (-120*SPTWM) )) {
				pMovil->vx -=4*SPTWM;
				spr_ctrl( FLIP_MAS , FLIP_X );
			}

			if ( (player.y > pMovil->ry) && (pMovil->vy < (120*SPTWM)) )
				pMovil->vy +=4*SPTWM;
			else if ((player.y < pMovil->ry) && (pMovil->vy > (-120*SPTWM)) )
				pMovil->vy -=4*SPTWM;

			pMovil->rx += pMovil->vx;
			pMovil->ry += pMovil->vy;

			pMovil->x = (unsigned char) (pMovil->rx >> 6);
			pMovil->y = (unsigned char) (pMovil->ry >> 6);
		} 
		else if ( (pMovil->tipo != 0xff) && (( player.estado & EST_EXIT )==0) ) {
			// Franky o Vampy o Wolfi
			pMovil->x += pMovil->vx;
			if ( pMovil->x <= pMovil->t1) {
				pMovil->vx = abs8 (pMovil->vx);
			}
			else if ( pMovil->x >= pMovil->t2) {
				pMovil->vx = -abs8 (pMovil->vx);
			}
			if ((maincounter &  15)==0) {
				pMovil->current_frame = (pMovil->current_frame == pMovil->next_frame ? pMovil->next_frame-64 : pMovil->next_frame);
				spr_pattern( SPRVRAM+pMovil->current_frame); 
			}
		}
	}
}

void checkcoins(void) {
	unsigned char xx,yy,is;
	unsigned int typem;

  for (is = 0; is < 10; is ++) {
		pMoneda= (MONEDA *) &monedas[is];
		if (pMoneda->type != 0) {
			xx=pMoneda->x;
			yy=pMoneda->y;
			typem=pMoneda->type==1?monedas_anim[monedas_frame]:monedas_take_anim[(pMoneda->type-2)/2];
			spr_set( 12+4+is ); spr_pattern( SPRVRAM+typem); spr_x(xx);spr_y(yy);
			if (pMoneda->type>1) {
				pMoneda->type++;
				if(pMoneda->type<12) pMoneda->y-=2; else pMoneda->y+=2;
				if(pMoneda->type>20) {
					pMoneda->type = 0;
					spr_hide();
				}
			}
			else {
				if ( (yp >= yy - 10) && (yp <= yy + 10) && (xp >= xx - 10) && (xp <= xx + 10) ) {
					play_snd (SNDTAKEGOL);
					pMoneda->type = 2;
					num_monedas --;
					total_coins++;
					draw_score();
					total_score ++;
					draw_total_score (17,23);
				}
			}
		}
	}

	if( (maincounter&3) == 0) {
     monedas_frame=(monedas_frame+1)%10;
   }

	// If the coins are finished, we present the outputs.
	if(num_monedas==0) {
		// no exit yet
		if ( (player.estado & EST_EXIT) == 0) {
			//if ( (sp_attr_buf[1+24*17] == 0)  && (sp_attr_buf[2+24*17] == 0) ) {
			if ( ((sp_attr_buf[2+14*17]& 0x0f) == 0)  && ((sp_attr_buf[3+14*17] & 0xf0) == 0) ) {
				draw_tile (5,20, level < 10 ? 112 : 122 ,0,1);
			}
			//if ( (sp_attr_buf[21+24*17] == 0)  && (sp_attr_buf[22+24*17] == 0) ) {
			if ( ((sp_attr_buf[12+14*17] & 0x0f) == 0)  && ((sp_attr_buf[13+14*17]& 0xf0) == 0) ) {
				draw_tile (25,20, level < 10 ? 116 : 122 ,0,1);
			}
			
			player.estado |= EST_EXIT;

			if(salida!=2) {
				//sfxinit(SFXVGM, SNDEXIT, 1);  //
				//play_sound(SNDEXIT);
			}

      // Get everyone out of here!
			for (is=0;is<3;is++) {
				if ( (fantact==0) || (is != idfant) ) {
					spr_set(12+1+is); spr_hide();
				}
			}
			// Retardo para quitar cafeína cuando no hay bishos.
			for (is = 0; is < 126; is ++)
				xx ++;
		}
		// exit is here, flash arrows
		else {
			if ( ((sp_attr_buf[2+14*17]& 0x0f) == 0)  && ((sp_attr_buf[3+14*17] & 0xf0) == 0) ) {
				spr_set(12+14); spr_y((18<<3)+arrow_yoff); spr_x(5<<3);
			}
			if ( ((sp_attr_buf[12+14*17] & 0x0f) == 0)  && ((sp_attr_buf[13+14*17]& 0xf0) == 0) ) {
				spr_set(12+15); spr_y((18<<3)+arrow_yoff); spr_x(25<<3);
			}
			if((maincounter&3) == 0) {
				arrow_yoff++;
				if(arrow_yoff>=3) arrow_yoff=0;
			}

			for (is = 0; is < 126; is ++)
				xx ++;
		}
 	}
}

// main game routine
void game(void) {
	unsigned char i,fade,xx;
	unsigned char pause,pause_cnt;
	unsigned int ofscami;
	
	salida=0;
	flag1 = 0;
	idfant=0;
	
	//idgfx=0;
	
	player.x = 32<<6;
	player.y = 144<<6;
	player.vy = 0;
	player.g = 32*SPTW; //8;
	player.vx = 0;
	player.ax = 24*SPTW; //8;
	player.rx = 32*SPTW; //8;
	player.salto = 64*SPTW;
	player.cont_salto = 1;
	player.saltando = 0;
	player.subframe = 0;
	player.facing = 1;
	player.estado = EST_NORMAL;
	player.ct_estado = 0;
	player.ground=1;
	player.nojump=0;
	
	draw_screen();

	timee = (num_monedas<<3)/3;//2;

	draw_lives ();
	draw_time ();
	draw_level (4,23);
	draw_score ();
	draw_total_score (17,23);

	sp_TileSet (4, 1, 34);sp_TileSet (5, 1, 35);sp_TileSet (4, 0, 32);sp_TileSet (5, 0, 33);
	sp_TileSet (22, 1, 38);sp_TileSet (23, 1, 39);sp_TileSet (22, 0, 36);sp_TileSet (23, 0, 37);

	maincounter = 0;
	playing = 1;
	fantact = 0;
	vidaextra = 0;
	xcami = ycami = 0;
	
	player.frame = uwol_r_2a;

	for (i=0;i<6;i++) {
		spr_set(i*2); spr_pattern( SPRVRAM+borders_lr); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_32x32|NO_FLIP ); spr_pal( 0 ); spr_pri( 1 );
		spr_x(0); spr_y(i*32);
		spr_set(i*2+1); spr_pattern( SPRVRAM+borders_lr); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_32x32|NO_FLIP ); spr_pal( 0 ); spr_pri( 1 );
		spr_x(224); spr_y(i*32);
	}

	spr_set(12+0); spr_pattern( SPRVRAM+player.frame); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_16x16|NO_FLIP ); spr_pal( 0 ); spr_pri( 1 );

	spr_set(12+14);spr_pattern( SPRVRAM+arrow_1a); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_16x16|NO_FLIP );spr_pal( 0 ); spr_pri( 1 );spr_hide();
	spr_set(12+15);spr_pattern( SPRVRAM+arrow_1a); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_16x16|NO_FLIP );spr_pal( 0 ); spr_pri( 1 );spr_hide();
/*
	// Play music
	//ststop(); stinit(DEBLEVELSVGM,0);		// play first track in the song file
	stop_music();	play_music(ingame_uwol);
*/
	// If the room has already been visited, we show the outputs and fantasmilla.
	if (visited[n_pant]) {
		num_monedas = 0;
		timee= 0;
	}
	
	monedas_frame=0;
	pause=0;
	pause_cnt=25;
	arrow_yoff=0;

	fade=0;
	
	while (playing) {
		maincounter ++;		// As it is a uchar, always go from 0 to 255, with powers of 2 cycles.
		
		if(pause_cnt) pause_cnt--;

		joyfunc(); // Read keyboard

		if (tchpad&sp_START) {
			if(pause_cnt==0) {
				pause^=1;
				pause_cnt=25;
				play_snd(SNDJUMP);
				if(pause) {
				  set_dark();
				}
				else {
					updatePalBG();
				}
			}
		}
    	
		if (pause) {
			vsync(1);
			continue;
		}
		
		if ( (maincounter & 31) == 0) {
			if (timee > 0) {
				timee --;
				draw_time ();
			} 
			else {
				//  When time runs out the Fanti!
				if (timee == 0) {
					play_snd(MUSTIMEOUT);
					timee--;
 
					// Logic: look for the first "empty" enemy. I do not have to, 
			        // The last enemy fanti replaced.
					idfant = 2;
					for ( i = 0; i < 3; i ++) {
						if (moviles[i].tipo == 0xff ) {
							idfant = i;
							moviles[idfant].x = 127;
							moviles[idfant].y = 87;
						}
					}
					moviles[idfant].rx = (moviles[idfant].x<<6);
					moviles[idfant].ry = (moviles[idfant].y<<6);

					// We created fanti:
					fantact = 1;
					moviles [idfant].current_frame = moviles [idfant].next_frame = fanti_r_1a;
					moviles [idfant].next_frame += 64;
					moviles [idfant].tipo = 3;
					spr_set( 12+1+idfant ); spr_pattern( SPRVRAM+moviles [idfant].current_frame); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_16x16|NO_FLIP );
					spr_x(moviles[idfant].x);spr_y(moviles[idfant].y);spr_pal( 0 ); spr_pri( 1 );
					// Ready!
				}
			}
		}

		satb_update();
		vsync(1);

		// Move sprites 
		if ( (player.estado & EST_MUR) ==0) {
				move();
		}
		else {
			salida = 2;
			death_sequence ();
		}

		move_moviles();

		xp = (unsigned char) (player.x>>6);
		yp = (unsigned char) (player.y>>6);

		// Collisions & game mechanics: 
		if ( player.estado & EST_PARP ) {
			// Duration of immunity:
			player.ct_estado --;
			if (player.ct_estado == 0) {
					player.estado = player.estado & (~EST_PARP);
					vidaextra = 0;
			}
		}
 
 		// Collision with enemies:
		if ( (( player.estado & EST_PARP ) ==0) && (( player.estado & EST_MUR ) == 0) /*&& (cheat==0)*/ ) {       
			for ( i = 0; i < 3; i ++ ) {
				pMovil=&moviles[i];
				if ( pMovil->tipo != 0xff  && ( (( player.estado & EST_EXIT ) == 0) || (fantact && i == idfant) )  ) {
					if ( (yp >= pMovil->y - 14) && (yp <= pMovil->y + 14) && (xp >= pMovil->x - 14) && (xp <= pMovil->x+ 14) ) {
						if ( ((player.estado & EST_NUDE) == 0) ) {
							player.estado |= EST_NUDE;
							player.estado |= EST_PARP;                  // We blink immune.
							player.ct_estado = 48;//32*3/2;                  // Time immunity.
	 
							if (timee > 0) {
								do {
									xcami = 32 + ((rand() % 12)<<4);
									ycami = 16 + ((1 + (rand() % 8)) << 4);
									xcami =  (xcami>>3);
									ycami = (ycami>>3);
								} 
								//while (sp_attr_buf[(xcami-4)+24*(ycami-2)] != 0);
								while (sp_attr_buf[((xcami-2)>>1)+14*(ycami-2)] != 0);
								spr_set( 12+16 ); spr_pattern( SPRVRAM+shirt_lr); spr_ctrl( SIZE_MAS|FLIP_MAS , SZ_16x16|NO_FLIP );
								spr_x(xcami<<3);spr_y(ycami<<3);spr_pal( 0 ); spr_pri( 1 );
								//sp_TileGet(xcami,ycami);
								//draw_tile (xcami, ycami, 84,0,0);
/*
								put_number(ofscami,3,4,6);
								put_number(xcami,3,4,7);
								put_number(ycami,3,4,8); // 18,8
								put_number(fdcami[0],3,4,0);
								put_number(fdcami[1],3,4,1);
								put_number(fdcami[2],3,4,2);
								put_number(fdcami[3],3,4,3);
*/
								xcami =  (xcami<<3);
								ycami = (ycami<<3);
							} 
							else {
								xcami = ycami = 0;
							}

							// Player "bounces" a little.
							player.vy = - 4*SPTW * (player.salto);
							player.vx = ((player.x >> 6) - pMovil->x) * (16*SPTW);
						} 
						else {
							player.vy = - 7 * (player.salto);
							lives --;

							if (lives > 0) {
								draw_lives(); 
							}

							player.estado |= EST_MUR;
							player.frame = uwolmuete_1a;
						}
						play_snd(SNDHURT);
						pMovil->vx =- pMovil->vx;           // The enemy turns around.
					}
				}
			}
		}


		// collision with the shirt
		if ( player.estado & EST_NUDE ) {
			if ( (yp >= ycami - 14) && (yp <= ycami + 15) && (xp >= xcami - 15) && (xp <= xcami + 15) ) {
				play_snd(SNDBONUS);
				total_score += 15;
				draw_total_score (17,23);

				player.estado &= (~EST_NUDE);
				player.estado |= EST_PARP;
				player.ct_estado = 48;//(32*3)/2;
				//xcami=xcami>>3; ycami=ycami>>3;
				//sp_TileSet(xcami,ycami,fdcami[0]);sp_TileSet(xcami+1,ycami,fdcami[1]);
				//sp_TileSet(xcami,ycami+1,fdcami[2]);sp_TileSet(xcami+1,ycami+1,fdcami[3]);
				spr_set( 12+16 ); spr_hide();
			}
		}
	 
		// We see if we hit with a coin.
		checkcoins();

		// Retardo para quitar cafeína cuando no hay bishos.
		if ( player.estado & EST_EXIT )
				for (i = 0; i < 126; i ++)
						xx ++;

		// Salir IZQ/DER
		if ( player.estado & EST_EXIT_LEFT ) {
			salida = 0;
			playing = 0;
		} 
		else if ( player.estado & EST_EXIT_RIGHT ) {
			salida = 1;
			playing = 0;
		}

		// Vidas extra a los 1000 y a los 2500
		i = 0;
		if (total_score >= 1000 && !bonus1) {
			i = 1;
			bonus1 = 1;
		}
		if (total_score >= 2500 && !bonus2) {
			i = 1;
			bonus2 = 1;
		}
		if (i==1) {
			player.estado |= EST_PARP;
			if(lives<99) lives ++;
			vidaextra = 1;
			player.ct_estado = 48;//32*3/2;
			//sfxinit(SFXVGM, SNDHITGRD, 1); // 
			//play_sound (SNDBONUS);
			draw_lives(); 
		}

		// Render
		if ( ( (player.estado & EST_PARP)  == 0) || ((maincounter & 1) ==0)  ) {
			spr_set(12+0); spr_pattern( SPRVRAM+player.frame & (~0x8000));  spr_ctrl( FLIP_MAS , (player.frame & 0x8000 ? FLIP_X : NO_FLIP)  );
			spr_x(xp);spr_y(yp); //spr_pal( 0 ); spr_pri( 1 );
		}
		else {
			spr_set(12+0); spr_hide();
		}
 

		if ( (player.estado & EST_EXIT) == 0) {
			for ( i = 0; i < 3; i ++ ) {
				if ( moviles [i].tipo != 0xff /*> -1*/ ) {
					spr_set( 12+1+i); spr_x(moviles[i].x);  spr_y(moviles[i].y);	 
				}
			}
		}
		else if (idfant) {
			spr_set( 12+1+idfant);
			spr_x(moviles[idfant].x); 
			spr_y(moviles[idfant].y); 
		}

		// if end of music , mute SN
		//if (*pDone == 0) {
		/*if ( pDone ) {
			stmute();
		}*/
	}
	
	/*
	wyz_stop_sound ();
*/
	fade_out ();
/*	
	todo_a_negro ();

	if (fantact) {
		// Al carao fanti.
		sp_MoveSprAbs (sp_moviles [idx], spritesClip, 0,0,0,0,0);
		sp_DeleteSpr (sp_moviles [idx]);

		// Creamos sprite normal:
		sp_moviles [idx] = sp_CreateSpr(sp_MASK_SPRITE, 2, wolfi_1a, 2, TRANSPARENT);
		sp_AddColSpr (sp_moviles [idx], wolfi_1b, TRANSPARENT);
		sp_AddColSpr (sp_moviles [idx], wolfi_1c, TRANSPARENT);			

		moviles [idx].current_frame = wolfi_1a;
	}
	
	// Tenemos que sacar los sprites del viewport
	
	sp_MoveSprAbs (sp_prueba, spritesClip, 0,0,0,0,0);
	if ( (player.estado & EST_EXIT) && fantact)
		sp_MoveSprAbs (sp_moviles [idx], spritesClip, 0,0,0,0,0);
*/
	if ( flag1 == 0 || flag1 == 1 )
		total_score += (100 + 5 * timee);
/*	
	for (i = 0; i < 3; i ++) 
		sp_MoveSprAbs (sp_moviles [i], spritesClip, 0, 0,0,0,0);
*/
	for (i=0;i<(1+10+3+2+12+1); i++) {
		spr_set(i);spr_hide();
	}
	satb_update();
	
	if (lives == 0xff) 
		salida = 3;
	
	//stmute();
	//mute_all();

	fl=salida;
}


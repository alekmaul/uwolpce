#ifndef _UWOL_LVL_
#define _UWOL_LVL_

extern const unsigned char enem_frames[];

//extern void fade_out();

extern void draw_lives (void);
extern void draw_score (void);
extern void draw_total_score (unsigned char x,unsigned char y);
extern void draw_time (void);
extern void draw_level (unsigned char x1, unsigned char y1);

extern void pyramid (void);

extern void draw_tile (unsigned char x, unsigned char y, unsigned char tile, unsigned char off, unsigned char attr);
extern void draw_screen(void);

#endif
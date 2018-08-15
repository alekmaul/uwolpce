/* A INCLURE AU DEBUT, MEME NIVEAU QUE LES GFX POUR PERMETTRE A HUC */
/*     DE BANQUER CORRECTEMENT */

/* Samples 6.992 khz */

#incbin(pcm_jump,"snds/jump.raw")
#incbin(pcm_takegold,"snds/takegold.raw")
#incbin(pcm_hitground,"snds/hitground.raw")
#incbin(pcm_hurt,"snds/hurt.raw")
#incbin(pcm_exit,"snds/exit.raw")
#incbin(pcm_bonus,"snds/bonus.raw")
#incbin(pcm_dye,"snds/dye.raw")

#asm
	.data
	.bank CONST_BANK

_snd_jump:
	.db	LOW(_pcm_jump+20)
	.db	HIGH(_pcm_jump+20)
	.db	BANK(_pcm_jump)
	.db	0xDF						; 11011111

_snd_takegold:
	.db	LOW(_pcm_takegold+20)
	.db	HIGH(_pcm_takegold+20)
	.db	BANK(_pcm_takegold)
	.db	0xDF						; 11011111
	
_snd_hitground:
	.db	LOW(_pcm_hitground+20)
	.db	HIGH(_pcm_hitground+20)
	.db	BANK(_pcm_hitground)
	.db	0xDF						; 11011111

_snd_hurt:
	.db	LOW(_pcm_hurt+20)
	.db	HIGH(_pcm_hurt+20)
	.db	BANK(_pcm_hurt)
	.db	0xDF						; 11011111
	
_snd_exit:
	.db	LOW(_pcm_exit+20)
	.db	HIGH(_pcm_exit+20)
	.db	BANK(_pcm_exit)
	.db	0xDF						; 11011111

_snd_bonus:
	.db	LOW(_pcm_bonus+20)
	.db	HIGH(_pcm_bonus+20)
	.db	BANK(_pcm_bonus)
	.db	0xDF						; 11011111

_snd_dye:
	.db	LOW(_pcm_dye+20)
	.db	HIGH(_pcm_dye+20)
	.db	BANK(_pcm_dye)
	.db	0xDF						; 11011111
	
	.code
#endasm

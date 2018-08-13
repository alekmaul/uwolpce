/* A INCLURE AU DEBUT, MEME NIVEAU QUE LES GFX POUR PERMETTRE A HUC */
/*     DE BANQUER CORRECTEMENT */

/* Samples 6.992 khz */

//#incbin(pcm_jump,"snds/jump.raw")
#incbin(pcm_jump,"snds/jump_pack.pcm")

#asm
	.bank CONST_BANK

_snd_jump:
	.db	LOW(_pcm_jump+20)
	.db	HIGH(_pcm_jump+20)
	.db	BANK(_pcm_jump)
	.db	0xDF						; 11011111

	.code
#endasm

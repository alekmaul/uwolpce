/* 
	Fonction Fade_out pour HUC 3.99
			TOUKO : 09/08/2018
*/


/* HUC VCE PALETTES BACKGROUNDS DEFINES */
#define 	BG_PAL0					 0
#define		BG_PAL1					16
#define		BG_PAL2					32
#define		BG_PAL3					48
#define		BG_PAL4					64
#define		BG_PAL5					80
#define		BG_PAL6					96
#define		BG_PAL7					112
#define		BG_PAL8					128
#define		BG_PAL9					144
#define		BG_PAL10				160
#define		BG_PAL11				176
#define		BG_PAL12				192
#define		BG_PAL13				208
#define		BG_PAL14				224
#define		BG_PAL15				240

/* HUC VCE PALETTES SPRITES DEFINES */
#define		SPR_PAL0				256
#define		SPR_PAL1				272
#define		SPR_PAL2				288
#define		SPR_PAL3				304
#define		SPR_PAL4				320
#define		SPR_PAL5				336
#define		SPR_PAL6				352
#define		SPR_PAL7				368
#define		SPR_PAL8				384
#define		SPR_PAL9				400
#define		SPR_PAL10				416
#define		SPR_PAL11				432
#define		SPR_PAL12				448
#define		SPR_PAL13				464
#define		SPR_PAL14				480
#define		SPR_PAL15				496

#asm

; // ASM VCE PALETTES BACKGROUNDS DEFINES
BG_PAL0					.equ 0
BG_PAL1					.equ 16
BG_PAL2					.equ 32
BG_PAL3					.equ 48
BG_PAL4					.equ 64
BG_PAL5					.equ 80
BG_PAL6					.equ 96
BG_PAL7					.equ 112
BG_PAL8					.equ 128
BG_PAL9					.equ 144
BG_PAL10				.equ 160
BG_PAL11				.equ 176
BG_PAL12				.equ 192
BG_PAL13				.equ 208
BG_PAL14				.equ 224
BG_PAL15				.equ 240

; // ASM VCE PALETTES SPRITES DEFINES
SPR_PAL0				.equ 256
SPR_PAL1				.equ 272
SPR_PAL2				.equ 288
SPR_PAL3				.equ 304
SPR_PAL4				.equ 320
SPR_PAL5				.equ 336
SPR_PAL6				.equ 352
SPR_PAL7				.equ 368
SPR_PAL8				.equ 384
SPR_PAL9				.equ 400
SPR_PAL10				.equ 416
SPR_PAL11				.equ 432
SPR_PAL12				.equ 448
SPR_PAL13				.equ 464
SPR_PAL14				.equ 480
SPR_PAL15				.equ 496

#endasm

/* ------------------------------------- */
/* 		FONCTION DE GESTION FADE  OUT 	 */
/* ------------------------------------- */

/* D'apres le schema des fncts FadeIN & FadeOUT d'Aramis        
		Arguments:
			bg_pal_deb 	: première palette du BACKGROUND dans le VCE.
			nb_bg_pals	: Nombre de palettes de BACKGROUND à fader.
			spr_pal_deb : première palette des SPRITES dans le VCE.
			nb_spr_pals	: Nombre de palettes des SPRITES à fader.
			speed		: Nombre de frames entre chaque fade.	
			
		NB: la fonction est limitée à 8 palettes au total qui peuvent 
			être affectées au BACKGROUND et/ou aux SPRITES.
			Les palettes BG/SPRITES doivent se succéder dans le VCE.
			
		E.G: 
			1- fade_palette_out( 0 , 4 , 0 , 0 , 10 );
			Fait un fade de 4 palettes du BG en commençant pas la palette 0
			avec un delai de 10 frames .
			
			2- fade_palette_out( 1 , 4 , 3 , 2 , 5 );
			Fait un fade de 4 palettes du BG en commençant par la palette 1
			Et un fade de 2 palettes de SPR en commençant par la palette 3
			avec un delai de 5 frames .
*/

#asm

_pals 			.equ $3FDF	

#endasm

char 	nb_bg_pals,		
		nb_spr_pals,
		speed,			
		fin_fade;
		
int		bg_pal_deb,
		spr_pal_deb;		
		
void fade_palette_out( unsigned int _bg_pal_deb, unsigned char _nb_bg_pals, unsigned int _spr_pal_deb, unsigned char _nb_spr_pals , unsigned char _speed )
{	
	bg_pal_deb = _bg_pal_deb;
	nb_bg_pals = _nb_bg_pals;
	spr_pal_deb = _spr_pal_deb;	
	nb_spr_pals = _nb_spr_pals;
	speed = _speed;
	
	#asm	 
			lda 	#7
			sta 	_fin_fade
		
		.start_fade:
			lda		_nb_spr_pals			
			and		#%0000_1111
			sta		_nb_spr_pals		
			
			lda		_bg_pal_deb			
			sta		$402								; // Séléction de la palette dans le VCE
			sta		<__ax			
			
			lda		_bg_pal_deb + 1
			sta		$403								
			sta		<__ax + 1			
			
			lda		_speed
			jsr		wait_vsync
			
		; // Si nb_bg_pals = 0 on passe à celles des sprites
			lda		_nb_bg_pals
			beq		.init_spr_fade_pals
			sta		<__cl	
	
		.init_fade:						
			clx
			
		.palette_fade:				
			ldy 	$404		
			tya											; // Récupère la composante BLEU 1
			
			and		#%0000_0111			
			beq		.pas_dec_blue		 
			dec		A			
								
		  .pas_dec_blue:
			sta		<__bl
			
			tya
			and		#%0011_1000							; // Récupère la composante ROUGE 1
			beq		.pas_dec_red		
			sec
			sbc		#8			
			
		  .pas_dec_red:
			sta		<__bh
			
			lda		$405								; // Lecture du bit 9 dans le VCE		
			lsr		A									; // Mise du bit 9 dans la carry
						
			tya			
			and		#%1100_0000							; // Récupère la composante VERTE 1			
			ror		A									; // Récupération du bit 9 dans REG A
			beq		.end_fade							
			sec
			sbc		#32			 		
			asl		A									; // Remise du bit 9 dans la carry
					
		  .end_fade:									; // Dans REG A on a déjà la val de _g
		  	rol		_pals + 1 , X 						; // Récupération du bit 9 dans la palette provisoire				 
		; // On reforme la couleur avec les 3 vals RVB
			ora 	<__bl												
			ora 	<__bh
			sta 	_pals , X			
		
			inx			
			inx			
			
		; // On teste si fin de palette X = 32
			cpx		#32
			bne		.palette_fade						; // 85 cycles max par boucle .
			
		; // On met en place la palette courante
			stw		<__ax , $402						; // Séléction de la palette dans le VCE
			tia		_pals , $404 , 32					; // Copie la palette courante dans le VCE
			
			addw	#16 , <__ax
			
			dec		<__cl
			bne  		.init_fade
			
		   .init_spr_fade_pals:
		; // Gestion du fade des palettes des sprites		
			lda		_nb_spr_pals			
			beq		.pas_fade_spr_pals
			bmi		.pas_fade_spr_pals
			sta		<__cl
			
			ora		#128
			sta		_nb_spr_pals	
			
			lda		_spr_pal_deb
			sta		$402								; // Séléction de la palette dans le VCE
			sta		<__ax			
			
			lda		_spr_pal_deb + 1
			sta		$403								
			sta		<__ax + 1		
			
			jmp		.init_fade							; // On relance le fade des pals des sprites
			
		   .pas_fade_spr_pals:			
			dec 	_fin_fade
			lbne  		.start_fade			
	#endasm	
}

void set_dark() {
	#asm
		stw		#BG_PAL1 , $402							; // Init du VCE sur palette 0 du BG
		stw		#BG_PAL1 , <__ax		
		stb		#4 , <__cl
		
		lda		#1
		sta		_nb_spr_pals
		
		lda		#2
		jsr		wait_vsync		
		
	  .init_colors_black:	   
	; // BG & SPR
	
		clx		
	  .init_pal:			
		ldy 	$404
		
	; // Récupère la composante BLEU
		tya	
		and		#%0000_0111		
		lsr		A
		lsr		A		
		sta		<__bl
		
	; // Récupère la composante ROUGE	
		tya
		and		#%0011_1000
		lsr		A
		lsr		A
		and		#%0011_1000
		sta		<__bh
		
	; // Récupère la composante VERTE
		lda		$405								; // Lecture du bit 9 dans le VCE		
		lsr		A								
						
		tya		
		ror		A
		lsr		A
		and		#%1100_0000							; // Récupère la composante VERTE 1	
		
	; // On reforme la couleur avec les 3 vals RVB
		ora 	<__bl												
		ora 	<__bh
		sta 	_pals , X	
		stz 	_pals + 1 , X
		
		inx
		inx
		
	; // On teste si fin de palette X = 32
		cpx		#32
		bne		.init_pal
		
	; // On met en place la palette courante
		stw		<__ax , $402						; // Séléction de la palette dans le VCE
		tia		_pals , $404 , 32					; // Copie palette courante dans le VCE
			
		addw	#16 , <__ax
		
		dec		<__cl
		bne		.init_colors_black
		
	; // On assombri la palette des sprites
		lda		_nb_spr_pals			
		beq		.pas_black_spr_pals
		bmi		.pas_black_spr_pals
		sta		<__cl
			
		ora		#128
		sta		_nb_spr_pals	
			
		lda		LOW_BYTE #SPR_PAL0		
		sta		$402								; // Séléction de la palette dans le VCE
		sta		<__ax			
			
		lda		HIGH_BYTE #SPR_PAL0  
		sta		$403								
		sta		<__ax + 1		
			
		jmp		.init_colors_black					; // On relance le fade des pals des sprites
	
	  .pas_black_spr_pals:
	#endasm
}

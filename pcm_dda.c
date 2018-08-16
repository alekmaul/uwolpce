						/* ---------------------------------------------------- */
						/*     Routine de Mise en place lecture samples DDA     */
						/* !!!! FAIRE UN Init_Voix_Psg() AVANT SINON PAS DE SON */
						/* ---------------------------------------------------- */
						
#include "pcm_dda.h"

/* TABLEAU DE SAMPLE A JOUER
   AVEC : 
		Adresse de début du sample + 20 => 2 octets
		Bank du sample					 => 1 octet
		Volume du sample				 => 1 octet

	 !! ATTENTION pour la fin de sample il faut que le bit 7 de l'octet 2 soit à 1 !!
*/

#asm
VOIX_DDA1						.equ	0
VOIX_DDA2						.equ	1

	.zp	
		sample_base_voix1: 		.ds 2
		sample_bank_voix1:	 	.ds 1
				
		sample_base_voix2: 		.ds 2
		sample_bank_voix2:	 	.ds 1
				
		cache_memory_voix1:		.ds 2					; // Cache lecture sample voix1
		cache_memory_voix2:		.ds 2					; // Cache lecture sample voix2
		test_octet_voix1:		.ds 1					; // Variable d'état lecture cache pour samples
		test_octet_voix2:		.ds 1					; // Variable d'état lecture cache pour samples
		
		volume: .ds 1
	.code
#endasm

/*Joue_Sample()
{*/
#asm
 .bank LIB1_BANK	
; // ---------------------------------------
; // -- ROUTINE DE LECTURE SAMPLES VIA DDA 
; // ---------------------------------------			 
	; // REG X adresse LOW du tableau PCM à jouer
	; // REG Y adresse HIGH du tableau PCM à jouer	
	; // REG A numéro de la voix PCM à utiliser 0/1		
	
	play_pcm:
			stx		<_al			
			sty		<_ah
			
			lsr		A									; // Si carry = 1 => voix 2 selectionnée
			bcs		Init_Sample_Voix2			
			
	Init_Sample_Voix1: 		
			lda		[ _ax ]		
			sta		<sample_base_voix1
			lda		[ _ax ].1		
			sta		<sample_base_voix1 + 1
			lda		[ _ax ].2		
			sta		<sample_bank_voix1					
	
		; // Selection de la voix
			ldx		#VOIX_DDA1
			stx		$800
		
		; // Volume
			lda		[ _ax ].3
			sta		$804			
			
			stz		<test_octet_voix1
			
			rts
	
	Init_Sample_Voix2:			
			lda		[ _ax ]		
			sta		<sample_base_voix2
			lda		[ _ax ].1		
			sta		<sample_base_voix2 + 1
			lda		[ _ax ].2		
			sta		<sample_bank_voix2		
			
		; // Selection de la voix
			ldx		#VOIX_DDA2
			stx		$800
			
		; // Volume
			lda		[ _ax ].3
			sta		$804			
			
			stz		<test_octet_voix2
			
			rts	
#endasm
/*}*/

void init_Voix_Sample(void) {
	#asm
; // -----------------------------------------------------------------
; // -- Fonction d'initialisation des voix 6 pour jouer les samples --
; // -----------------------------------------------------------------	
		
	; // Initialisation à 0 de la voix 0
		ldy		#$FF
		ldx		#$DF
		
		lda		#VOIX_DDA1								; // Choix de la voix DDA 1				
	    sta		$800		
		stx		$804
		sty		$805		
		stz		$806
		
		lda		#VOIX_DDA2								; // Choix de la voix DDA 2
		sta		$800
		stx		$804
		sty		$805		
		stz		$806
	
	; // Init volume de la balance globale		
		cla		
		clc
		
		sty		$801
				
	; // Initialise les compteurs lecture samples						
	; // Arrete la lecture d'un sample DDA
	Stope_Sample_DDA:
		stz		<sample_base_voix1		
		tii		sample_base_voix1 , sample_base_voix1 + 1 , 11
		
	; // Désactive la lecture des voix PCM bit 7 = 1
		lda		#%10000000
		sta		<test_octet_voix1
		sta		<test_octet_voix2
	#endasm
}

#asm

  .bank LIB1_BANK
	; // --------------------------
	; // INTERRUPTION TIMER PERSO
	; // --------------------------
	
	; // Joue des samples non compressés 
	User_Timer_Irq:	
		stz 	$1403									; // RAZ TIMER		
		pha						
		
	; // Evite de désactiver la voix si pas de sample
		bbs		#7 , <test_octet_voix1 , .voix_pcm_2	
		
		lda		#VOIX_DDA1								; // Choix de la voix DDA 1
		sta		$800 			
		
	; // On sauve la banque courante
		tma     #3
		pha
	   	           
		lda     <sample_bank_voix1              
		tam     #3
				
		lda     [ sample_base_voix1 ]  
		bmi		.voix_1_off								; // Si bit 7 = 1 alors fin sample on sort		
		
		sta		$806									; // On joue le sample 1 voix 1		
									
		inc		<sample_base_voix1		
		bne		.fin_sample_voix1
		inc		<sample_base_voix1 + 1
		bpl		.fin_sample_voix1
		
		lda		#$60
		sta		<sample_base_voix1 + 1
		inc		<sample_bank_voix1    
		
		bra		.fin_sample_voix1
		
	  .voix_1_off:		
		;stz		$805									; // Balance à 0 sur la voix 1
		;stz		$804									; // Son à 0 sur la voix 1							
		smb		#7 , <test_octet_voix1					; // On déactive la lecture des sample pour la voix 1
		;rmb		#VOIX_DDA1 , <voix_on_off
				
	  .fin_sample_voix1:
	; // On restaure l'ancienne bank  
		pla
		tam 	#3
		
	  .voix_pcm_2:
	; // Evite de désactiver la voix si pas de sample
		bbs		#7 , <test_octet_voix2 , .fin_timer_int
		
		lda		#VOIX_DDA2								; // Choix de la voix DDA 2
		sta		$800 											
		
	; // On sauve la banque courante
		tma     #3
		pha
	    	           
		lda     <sample_bank_voix2              
		tam     #3
				
		lda     [ sample_base_voix2 ]  
		bmi		.voix_2_off								; // Si bit 7 = 1 alors fin sample on sort
		
		sta		$806									; // On joue le sample 1 voix 2	
			
		inc		<sample_base_voix2
		bne		.fin_sample_voix2
		inc		<sample_base_voix2 + 1
		
	; // Si fin de la bank, on remappe sur bank 3 pour la suite
		bpl		.fin_sample_voix2		
		lda		#$60
		sta		<sample_base_voix2 + 1
		inc		<sample_bank_voix2  
		
		bra		.fin_sample_voix2
		
	  .voix_2_off:	  
		;stz		$805									; // Balance à 0 sur la voix 2
		;stz		$804									; // Son à 0 sur la voix 2																	
		smb		#7 , <test_octet_voix2					; // On déactive la lecture des sample pour la voix 2
		;rmb		#VOIX_DDA2 , <voix_on_off
	
	  .fin_sample_voix2:
	; // On restaure l'ancienne bank
		pla
		tam		#3
	
	 .fin_timer_int:
		pla							
		
		rti						
#endasm

void init_USER_Timer_IRQ(void) {
	#asm
		sei												; // Désactive les interruptions		
	; // ***** MISE EN PLACE TIMER PERSO *****
		ldx 	#LOW( User_Timer_Irq ) 				
		ldy 	#HIGH( User_Timer_Irq )		
		lda 	#2		
		jsr 	set_intvec		
		
		lda 	#$01
		sta 	$0C01									; // Active le TIMER				
		stz 	$0C00									; // Mise en place du delais de l'interruption TIMER
				
		stz 	$1402
		stz 	$1403
			
		smb		#2 , <irq_m								; // Activation de "irq_m" pour le TIMER
			
		cli												; // Active les interruptions
	#endasm
}

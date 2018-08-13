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


/*
const char snd_jump[]= {
	LOW( pcm_jump + 20 ),
	HIGH( pcm_jump + 20 ),
	BANK( pcm_jump ),
	0xDF
};
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
	
	Joue_Sample:
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

Init_Voix_Sample()
{
	#asm
; // -----------------------------------------------------------------
; // -- Fonction d'initialisation des voix 6 pour jouer les samples --
; // -----------------------------------------------------------------	
		
	; // Initialisation à 0 de la voix 0
		lda		#VOIX_DDA1								; // Choix de la voix DDA 1
		ldx		#$FF
	  
	  .init_dda:
        sta		$800 					
		stz		$802		
		stz		$803
		stz		$804		
		stx		$805
		stz		$806
		stz		$807
		stz		$808
		stz		$809			
		
		inc		A
		cmp		#VOIX_DDA2 + 1
		bcc		.init_dda	
	
	; // Init volume de la balance globale
		lda		#$FF
		sta		$801
	
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
	
	; // Joue des samples compressés en bit packed
	; // 1 er octet = 1 er sample 5 bit + partie low 3 ieme sample 5 bit
	; // 2 nd octet = 2 nd sample 5 bit + partie high 3 ieme sample 5 bit
	; // Exemple :  AAAAACCC BBBBBCC0,et si fin sample AAAAACCC BBBBBCC1
	
	User_Timer_Irq:	
		stz 	$1403									; // RAZ TIMER		
		pha						
		
	; // Evite de désactiver la voix si pas de sample
		bbs		#7 , <test_octet_voix1 , .fin_sample_voix1		
		
		lda		#VOIX_DDA1								; // Choix de la voix DDA 1
		sta		$800		
		
		bbs		#0 , <test_octet_voix1 , .prep_octets_voix1		
		
	; // On Met en cache 3 samples(2 octets) pour la voix 1 et on lit le premier sample
	  .fin_comp1:
	; // On sauve la banque courante
		tma     #3
		pha
	   	           
		lda     <sample_bank_voix1              
		tam     #3
				
		lda     [ sample_base_voix1 ]  
		sta		<cache_memory_voix1	
		
		sta		$806									; // On joue le sample 1 voix 1
	   
		inc		<sample_base_voix1	
		lda		[ sample_base_voix1 ]
		bmi		.voix_1_off								; // Si bit 7 = 1 alors fin sample on sort
		
		sta		<cache_memory_voix1 + 1

	; // On restaure l'ancienne bank
		pla
		tam 	#3
					  
		lda		#3
		sta 	<test_octet_voix1	
		
		inc		<sample_base_voix1		
		bne		.fin_sample_voix1
		inc		<sample_base_voix1 + 1
		bpl		.fin_sample_voix1
		
	; // Si fin de la bank, on remape sur bank 3 pour la suite
		lda		#$60
		sta		<sample_base_voix1 + 1
		inc		<sample_bank_voix1    
		
		bra		.fin_sample_voix1
		
	  .voix_1_off:
	; // On restaure l'ancienne bank
		pla
		tam 	#3
		
		;stz		$805									; // Balance à 0 sur la voix 1
		;stz		$804									; // Son à 0 sur la voix 1							
		smb		#7 , <test_octet_voix1					; // On déactive la lecture des sample pour la voix 1
		;rmb		#VOIX_DDA1 , <voix_on_off
		bra		.fin_sample_voix1
		
	; // On lit le second sample
	  .prep_octets_voix1:		
		lda		<cache_memory_voix1 + 1
	
	; // On decale le compteur de sample pour la voix 1
		lsr		<test_octet_voix1
	; // Si second sample lu, on décompresse le sample 3		
		bne		.transfert_data_sample_voix1
		and 	#$60		
		lsr 	A
		lsr 	A		
		sta		<cache_memory_voix1 + 1
		lda 	<cache_memory_voix1	
		lsr 	A
		lsr 	A
		lsr 	A
		lsr 	A
		lsr 	A
		ora		<cache_memory_voix1 + 1	
	
	  .transfert_data_sample_voix1:		
		sta    	$806												
			
	  .fin_sample_voix1:	  
	; // Evite de désactiver la voix si pas de sample
		bbs		#7 , <test_octet_voix2 , .fin_sample_voix2
		
		lda		#VOIX_DDA2								; // Choix de la voix DDA 1
		sta		$800 									; // Choix de la voix DDA 2    			
		
		bbs		#0 , <test_octet_voix2 , .prep_octets_voix2	
		
	; // On Met en cache 3 samples(2 octets) pour la voix 2 et on lit le premier sample
	  .fin_comp2:
	; // On sauve la banque courante
		tma     #3
		pha
	    	           
		lda     <sample_bank_voix2              
		tam     #3
				
		lda     [ sample_base_voix2 ]  
		sta		<cache_memory_voix2		
		sta		$806									; // On joue le sample 1 voix 2
	   
		inc		<sample_base_voix2	
		lda		[ sample_base_voix2 ]
		bmi		.voix_2_off								; // Si bit 7 = 1 alors fin sample on sort
		
		sta		<cache_memory_voix2 + 1		
		
	; // On restaure l'ancienne bank
		pla
		tam		#3 
			  
		lda		#3
		sta 	<test_octet_voix2
		
		inc		<sample_base_voix2
		bne		.fin_sample_voix2
		inc		<sample_base_voix2 + 1
		
	; // Si fin de la bank, on remape sur bank 3 pour la suite
		bpl		.fin_sample_voix2		
		lda		#$60
		sta		<sample_base_voix2 + 1
		inc		<sample_bank_voix2  
		
		bra		.fin_sample_voix2
		
	  .voix_2_off:
	; // On restaure l'ancienne bank
		pla
		tam		#3
	  
		;stz		$805									; // Balance à 0 sur la voix 2
		;stz		$804									; // Son à 0 sur la voix 2																	
		smb		#7 , <test_octet_voix2					; // On déactive la lecture des sample pour la voix 2
		;rmb		#VOIX_DDA2 , <voix_on_off
		bra		.fin_sample_voix2
		
	; // On lit le second sample	
	  .prep_octets_voix2:
		lda		<cache_memory_voix2 + 1
		
	; // On decale le compteur de sample pour la voix 2
		lsr		<test_octet_voix2
	; // Si second sample lu, on décompresse le sample 3		
		bne		.transfert_data_sample_voix2
		and 	#$60		
		lsr 	A
		lsr 	A		
		sta		<cache_memory_voix2 + 1
		lda 	<cache_memory_voix2	
		lsr 	A
		lsr 	A
		lsr 	A
		lsr 	A
		lsr 	A
		ora		<cache_memory_voix2 + 1	
		
	  .transfert_data_sample_voix2:				
		sta    	$806

	  .fin_sample_voix2:		
		pla							
		
		rti						
#endasm

Init_USER_Timer_IRQ()
{
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

/*----------------------------------------------------------------*/
/* this is a hacked version of sngInit(), produced by mml2pce.    */
/* specifically, if has been hacked to use the value table at the */
/* start of the MML Data block.                                   */
/*----------------------------------------------------------------*/

SongInit( song_no )
int       song_no;
{
#asm
	
;--------------------------------------------------------------------
; save MPR : we'll map buffer in at $8000 (MPR 4). We tried MPR 3,
; but it didn't work....

	tma		#4
	pha				; save original MPR

;--------------------------------------------------------------------
; set MPR where buffer is at.

	lda		sngBankLo,x
	tam		#4

;--------------------------------------------------------------------
; set bank values 
;
	lda 	#PSGF_BANK
	sta		<__dh
	lda		sngBankLo,x
	sta		<__al
	lda		sngBankHi,x
	sta		<__ah
	jsr		psg_bios

;======================================================================
; check to make sure we have a track index table location 
;
	lda		header+trackIX
	ora		header+trackIX+1
	beq		.noTrack
	
;---------------------------------------------------------------------
; register track index table location 
;
	lda    #PSGF_TRACK
	sta    <__dh
	
	lda		header+trackIX
	sta		<__al
	lda		header+trackIX+1
	sta		<__ah
	
	jsr		psg_bios

;======================================================================
; make sure we have waves, and a valid address.

.noTrack
	lda		header+nWaves
	beq		.noWave
	
	lda		header+waveBase
	ora		header+waveBase+1
	beq		.noWave
	
;---------------------------------------------------------------------
; register wave table location 
;
	lda    #PSGF_WAVE
	sta    <__dh
	
	lda		header+waveBase			; WAVE_BASE
	sta		<__al
	lda		header+waveBase+1
	sta		<__ah
	jsr		psg_bios
	
;========================================================================
; check to make sure we have a pitch table location 
; aka, pitch envelope

.noWave
	lda		header+freqIX
	ora		header+freqIX+1
	beq		.noEnvelope

;------------------------------------------------------------------------
; register envelope envelope table location
;
	lda		#PSGF_ENV
	sta		<__dh

	lda		header+freqIX
	sta		<__al
	lda		header+freqIX+1
	sta		<__ah
	jsr		psg_bios

	
;=========================================================================
; check to make sure we have a modulation index table location 
;
.noEnvelope
	lda		header+moduIX
	ora		header+moduIX+1
	beq		.noModu
	
;---------------------------------------------------------------------
; register modulation index table location 
;
	lda    #PSGF_FM	
	sta    <__dh
	
	lda		header+moduIX
	sta		<__al
	
	lda		header+moduIX+1
	sta		<__ah
	jsr		psg_bios

;======================================================================
; check to make sure we have a pitch envelope table location 
;

.noModu
	lda		header+pitchIX
	ora		header+pitchIX+1
	beq		.noPitch
	
;---------------------------------------------------------------------
; register pitch index table location 
;
	lda    #PSGF_PE
	sta    <__dh
	
	lda		header+pitchIX
	sta		<__al
	lda		header+pitchIX+1
	sta		<__ah
	jsr		psg_bios

;==========================================================================
; check to make sure we have a percussion table location 
;
.noPitch
	lda		header+drumIX
	ora		header+drumIX+1
	beq		.noDrum

;-------------------------------------------------------------------------
; register percussion 
;
	lda		#PSGF_PC
	sta		<__dh

	lda		header+drumIX
	sta		<__al
	lda		header+drumIX+1
	sta		<__ah
	jsr		psg_bios

;=====================================================================
; Set Tempo 
;
.noDrum
	lda		#PSGF_TEMPO
	sta		<__dh

	lda		header+tempo
	sta		<__al
	jsr		psg_bios

;--------------------------------------------------------------------
; restore MPR.

.out
	pla                     ; saved MPR
	tam		#4

#endasm
}

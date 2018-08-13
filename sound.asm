;-----------------------------------------------------------------------------
; sound.asm - the squirrel MML player, which can be used to control the psg
;-----------------------------------------------------------------------------
; modified mar 2010 by Micheal E. Ward. 
; Copyright 2010 by M.E. Ward. All rights reserved
;.............................................................................
; this file should contain the PSG_BIOS dispatch routine and the player 
; routine. The actual PSG_BIOS functions are in a different file and page, to
; minimize the code space used in the irq handler (startup) page
;*****************************************************************************
; this is the actual psg-bios interface. All PSG_BIOS functions are called from
; here. 
; Note that the actual functions may be in a different page of memory; hence, 
; we have to map them in before we call them. All PSG_BIOS functions MUST
; reside in the same page of memory.
;-----------------------------------------------------------------------------------
;  IN:  _DH = function number
; OUT:  _AX = function return value (if any)
; USE:  _X and _Y are preserved
;       _A is trashed
;............................................................................
; I'm not sure if this is the correct name or not, but it should work

		.bank		START_BANK

psg_bios:

		;------------------------------------------------------------------------
		; save contents of X and Y registers, so they are not lost
	
		phx							; save contents of X register
		phy							; save contents of Y register
	
		;------------------------------------------------------------------------
		; this maps the bios functions into the cpu address space.
	
		tma	#PAGE(psgOn)			; get page number for code in 'our' area
		pha							; save it
	
		lda	#BANK(psgOn)			; load page number for our code.
		tam	#PAGE(psgOn)			; map the code in

		;------------------------------------------------------------------------
		; validate function number
		
		lda		<__dh					; get function number
		cmp		#psgBiosEntries			; in valid range ?
		bcs		.psgBadParam			; skip call if out of range

		;------------------------------------------------------------------------
		; this calls the actual function code, via indirection. 
	
		asl		A						; 2 bytes per address
		tax								; place offset into index register
		lda		<__al					; get parameter
	
		jsr		psgBiosCall				; call the function from the table, as a sub-routine

		;------------------------------------------------------------------------
		; restore the original code that we mapped out

.psgBadParam:
	
		tax								; save any return value in X. 

		pla								; get page number for original code
		tam		#PAGE(psgOn)			; map original code back in
		
		txa								; restore the original return value
	
		;------------------------------------------------------------------------
		; restore original register values
	
		ply
		plx
		rts

;----------------------------------------------------------------------------------------
; this is used to call the actual bios functions.
;----------------------------------------------------------------------------------------

psgBiosCall:	
		jmp		[psgBiosTable,X]

;----------------------------------------------------------------------------------------
; the PSG-BIOS functions. The table needs to be in this page, so we can call them
; after we map them in. (NOTE: the actual functions MAY be in a different page)
;----------------------------------------------------------------------------------------

psgBiosTable:	
		.dw	psgOn			; psg_on       (00)
		.dw	psgOff			; psg_off      (01)
		.dw	psgInit			; psg_init     (02)
		.dw	psgBank			; psg_bank     (03)
		.dw	psgTrack		; psg_track    (04)
		.dw	psgWave			; psg_wave     (05)
		.dw	psgEnv	 		; psg_env      (06)
		.dw	psgFM			; psg_fm       (07)
		.dw	psgPE			; psg_pe       (08)
		.dw	psgPC			; psg_pc       (09)
		.dw	psgSetTempo		; psg_settempo (0a)
		.dw	psgPlay			; psg_play     (0b)
		.dw	psgMStat		; psg_mstat    (0c)
		.dw	psgSStat		; psg_sstat    (0d)
		.dw	psgMStop		; psg_mstop    (0e)
		.dw	psgSStop		; psg_sstop    (0f)
		.dw	psgAStop		; psg_astop    (10)
		.dw	psgMvOff		; psg_mvoff    (11)
		.dw	psgCont			; psg_cont     (12)
		.dw	psgFadeOut		; psg_fdout    (13)
		.dw	psgDCnt			; psg_dcnt     (14)

psgBiosTableEnd = *			; table end is here
psgBiosEntries  = (psgBiosTableEnd - psgBiosTable) / 2

;-----------------------------------------------------------------------------------------------
; this brings in the psg_bios functions. As promised, they are in a seperate bank.
;-----------------------------------------------------------------------------------------------


		.bank		SOUND_BANK,"PSG Driver"
		.include	"main\PsgBiosFuncs.asm"

;...............................................................................................
; These are the driver rountines. Note that it's pretty big, which is why
; we go through all the hoops to run it from the non-fixed page :-)
;...............................................................................................
; this brings in the actual main track driver routine pieces. 
;...............................................................................................	
	
		.include 	"main\psgDrive.asm"			; the main track driver routine
		.include	"main\mmlParse.asm"			; mml data parser
		.include	"main\bytecodes.asm"			; mml operations
		.include	"main\volume.asm"			; volume control
		.include	"main\drums.asm"			; percussion / noise control
		.include	"main\adsr.asm"				; volume envelope handler
		.include	"main\freqEnv.asm"			; frequency processing
		.include	"main\fade.asm"				; fade-out processing.
		.include	"main\output.asm"			; output to hardware

;...............................................................................................
; this brings in the actual sub track driver routine pieces. 
;...............................................................................................	
	
		.include	"sub\psgDrive.asm"
		.include	"sub\mmlParse.asm"
		.include	"sub\bytecodes.asm"
		.include	"sub\volume.asm"
		.include	"sub\drums.asm"
		.include	"sub\adsr.asm"
		.include	"sub\freqEnv.asm"
		.include	"sub\fade.asm"
		.include	"sub\output.asm"
	
;...............................................................................................	
; the data tables we use.

		.include	"data\freqTable.inc"
		.include	"data\tempoTable.inc"
		.include	"data\envelopes.inc"
		.include    "data\waveTable.inc"
;
; now, return context to bank established in
; file which included this
;
		.bank		START_BANK

		;--------------------------------------------------------------------------
		; Start PSG BIOS interrupt handler (shared by VSYNC and TIMER modes)
		;--------------------------------------------------------------------------

psg_driver:	tma		#page(psgMainDrive)		; map out whatever is in 'our' code segment
		pha						; save old code page number
		lda		#bank(psgMainDrive)		; load 'our' page number
		tam		#page(psgMainDrive)		; and map it into memory
		tma4						; get page for bank 4 = $8000
		pha						; save whatever was there
		lda		psgDataBankLow			; map song data low
		tam4
		tma5						; get page for bank 5 = $A000
		pha						; save whatever was there
		lda		psgDataBankHigh
		tam5
	
.doMain:	lda		psgSystemNo			; get system number
		cmp		#1				; sub-track only ?
		beq		.doSub				; yes, skip main tracks
	
		inc		_MainCnt
		jsr		psgMainDrive			; process main tracks

		cmp		#0				; still playing ??
		bne		.doSub				; yes, skip pausing it
	
		lda		psgTrackCtrl			; get control byte
		ora		#$80				; main track pause bit
		sta		psgTrackCtrl
	
.doSub:		lda		psgSystemNo			; get system number
		cmp		#0				; main tracks only ?
		beq		.resetTimer			; yes, skip sub tracks

		inc		_SubCnt
		jsr		psgSubDrive			; process sub tracks
	
		cmp		#0				; still playing ?
		bne		.resetTimer			; yes, skip pausing it
	
		lda		psgTrackCtrl			; get track control bit
		ora		#$40				; main-track pause bit
		sta		psgTrackCtrl
	
.resetTimer:	lda		<psg_inhibit			; $00=off, $80=timer, $81=vsync
		cmp		#$80
		bne		.exit				; not timer (0), skip re-set

;		lda		<psg_inhibit			; $80=off, $00=timer, $01=vsync
;		bne		.exit				; ORIGINAL BIOS MEANING
		
		lda		psgTimerCount			; check timer count
		bmi		.exit				; if it's already been set, skip it

		sta		timer_cnt			; update timer count
		ora		#$80				; mark as set
		sta		psgTimerCount			; save for next time

.exit:		pla						; restore high data banks
		tam5
		pla						; restore low data bank
		tam4
		pla
		tam		#page(psgMainDrive)		; restore code bank
		rts

		;--------------------------------------------------------------------------
		; End PSG BIOS interrupt handler (shared by VSYNC and TIMER modes)
		;--------------------------------------------------------------------------

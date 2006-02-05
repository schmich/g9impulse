;; -*- tab-width: 4 -*-
;; Interrupt Service Routine
;;
;; Copyright (c) 2005, K9spud LLC.
;; http://www.k9spud.com/traxmod/
;;
;; $Log: isr.s,v $
;; Revision 1.10  2005/09/28 04:56:44  edwards
;; More optimization attempts.
;;
;; Revision 1.9  2005/09/27 21:59:28  edwards
;; Speed optimizations.
;;
;; Revision 1.8  2005/09/27 06:08:51  edwards
;; Trying some various changes to the prefetch timing.
;;
;; Revision 1.7  2005/09/26 05:52:11  edwards
;; Playing all sample data out of SD Card! Still a few glitches on tight sample
;; loops though (due to SD Card block misalignment).
;;
;; Revision 1.6  2005/09/26 02:35:34  edwards
;; Caching SD Card data is working, but needs to be optimized to be faster.
;;
;; Revision 1.5  2005/09/20 21:01:46  edwards
;; Reading 64 bytes from SD card each mixing loop iteration is working at
;; 16KHz.
;;
;; Revision 1.4  2005/09/18 09:09:20  edwards
;; SPI Interrupt driven SD Card command write working.
;;
;; Revision 1.3  2005/09/16 05:01:39  edwards
;; Now working with only 64 byte block length on SD Card reads.
;;
;; Revision 1.2  2005/09/15 08:29:48  edwards
;; Plays raw audio data at 43Hz, 8 bits, stereo from my SD Card now, but
;; only seems to work when running under the ICD. Something seriously
;; wrong with timing or something somewhere.
;;
;; Revision 1.1  2005/09/11 10:35:40  edwards
;; Ported code to play on dsPIC30F4013.
;;

.include "p30f4013.inc"

.text
.section .text,code
.align 2

.extern _pPlay
.extern _iPlayLoc
.extern _iPlay

.global __T1Interrupt
__T1Interrupt:
	push.s

	mov.w 	_pPlay, w2
	bclr.b 	IFS0, #T1IF		; Reset Timer 1 interrupt flag
	mov.w 	[w2++], w0
	mov.w 	w0, OC3RS
	mov.w 	[w2++], w0
	mov.w 	w0, OC4RS
	mov.w 	#448-#1, w0
	mov.w 	w2, _pPlay

	;; Have we reached the end of the play buffer?
	inc2.w	_iPlayLoc
	cp.w	_iPlayLoc
	bra		leu, ExitInterrupt

	;; We've reached the end of the play buffer, so we
	;; need to reset our pointer and counter
	mov.w	#_iPlay, w0
	clr.w	_iPlayLoc
	mov.w	w0, _pPlay

ExitInterrupt:
	pop.s
	retfie

.extern _SD_Buffer
.extern _SD_BufferEnd

.global __AltSPI1Interrupt
__AltSPI1Interrupt:
	setm.w	SPI1BUF			; start the next SPI data transfer
	push.s
	mov.w	_SD_Buffer, w0
	mov.w	SPI1BUF, w2		; read byte of data from SD Card
	bclr	IFS0, #SPI1IF	; clear interrupt flag
	mov.b	w2, [w0++]		; save data to SD_Buffer

;if(SD_Buffer == SD_BufferEnd)
	cp.w 	_SD_BufferEnd
	bra 	nz, SPIExitInterrupt

	;; We've finished reading an entire SD Card block, go back to the slow
	;; SPI/SD Card state machine interrupt service routine.
	bclr 	INTCON2, #ALTIVT
	pop.s
	retfie

SPIExitInterrupt:
	mov.w	w0, _SD_Buffer
	pop.s
	retfie

.end

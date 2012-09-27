; Test:       tlt
; Author:     Divya Gulati
; Purpose:    Tests the the TLT integer test instruction.
; Created On: Nov 30, 2003	 
	
; The exit value should be zero

.set SYS_EXIT=1

.text
.global _start
.bbegin _start

	N[0] movi 0x8 N[2,0]	; put 8 in the left operand of N[2]
	N[1] movi 0x8 N[2,1]	; put 8 in the right operand of N[2]
	N[2] tlt W[3]		; check if the left operand < right operand
				; the value should be 0
	
	N[3] movi SYS_EXIT W[0]	; put SYS_EXIT in G[0]
	N[4] scall

	; commit the values to the register file.
	W[0] write G[0]
	W[3] write G[3]		
	
.bend

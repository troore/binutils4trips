; Test:       tgt_2
; Author:     Divya Gulati, Robert McDonald
; Purpose:    Tests the the TGT integer test instruction.

; The exit value should be zero

.set SYS_EXIT=1

.text
.global _start
.bbegin _start

	N[0] movi 0x8 N[2,0]	; put 8 in the left operand of N[2]
	N[1] movi 0x8 N[2,1]	; put 8 in the right operand of N[2]
	N[2] tgt W[3]		; check if the left operand < right operand
				; the value should be 0
	
	N[3] movi SYS_EXIT W[0]	; put SYS_EXIT in G[0]
	N[4] scall

	; commit the values to the register file.
	W[0] write G[0]
	W[3] write G[3]		
	
.bend

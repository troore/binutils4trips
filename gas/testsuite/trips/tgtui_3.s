; Test:       tgtui
; Author:     Divya Gulati, Robert McDonald
; Purpose:    Tests the the TGTUI integer test instruction.

; The exit value should be zero

.set SYS_EXIT=1

.text
.global _start
.bbegin _start

	N[0] movi 0 N[2,0]	; put -1 in the left operand of N[2]
	N[2] tgtui -1 W[3]	; The value should be 0 since both operands
				; are treated as unsigned numbers
	
	N[3] movi SYS_EXIT W[0]	; put SYS_EXIT in G[0]
	N[4] scall

	; commit the values to the register file.
	W[0] write G[0]
	W[3] write G[3]		
	
.bend

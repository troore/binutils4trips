; Test:       tle
; Author:     Divya Gulati
; Purpose:    Tests the the TLE integer test instruction.
; Created On: Nov 30, 2003	 
	
; The exit value should be one

.set SYS_EXIT=1

.text
.global _start
.bbegin _start

	N[0] movi 0x9 N[2,0]	; put 9 in the left operand of N[2]
	N[1] movi 0x9 N[2,1]	; put 9 in the right operand of N[2]
	N[2] tle W[3]		; check if the left operand <= right operand
				; the value should be 1
	
	N[3] movi SYS_EXIT W[0]	; put SYS_EXIT in G[0]
	N[4] scall

	; commit the values to the register file.
	W[0] write G[0]
	W[3] write G[3]		
	
.bend

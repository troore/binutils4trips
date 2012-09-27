; Test:     gens_1
; Author:   Robert McDonald
; Purpose:  Tests generate signed 16-bit constant

; Final result should be -7

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] gens -7 W[3]
N[1] bro exit
W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend

; Test:     bro_1
; Author:   Haiming Liu
; Purpose:  Test bro instruction

; Final result should be 0x1

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] movi 1 W[3]
N[1] bro block2

W[3] write G[3]
.bend

.bbegin block1
N[0] bro exit
.bend

.bbegin block2
N[0] bro block1
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend

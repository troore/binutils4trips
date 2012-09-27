; Test:     bro_t
; Author:   Haiming Liu
; Purpose:  Test predication with bro instruction

; Final result should be 0x0

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] gens 1 N[2]
N[1] gens 1 N[2,1]
N[2] teq N[3,p] N[4,p]   ;test 1 == 1
N[3] bro_t block2
N[4] bro_f block1     ;should not be executed
.bend

.bbegin block1
N[0] movi 1 W[3]
N[1] bro exit

W[3] write G[3]
.bend

.bbegin block2
N[0] movi 0 W[3]
N[1] bro exit

W[3] write G[3]
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend

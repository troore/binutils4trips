; Test:     pred_oring_4
; Author:   Robert McDonald
; Purpose:  Tests a predicate oring construct

; Final result should be 0x1

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] movi 1 W[3]
N[32] genu %lo(_exit) N[33]
N[33] app %bottom(_exit) W[2]
N[34] callo check
W[2] write G[2]
W[3] write G[3]
.bend

; This is a goofy way to compute predicates, but it
; IS technically legal...
.bbegin check
R[2] read G[2] N[28]
R[3] read G[3] N[0]
N[0] mov3 N[4,0] N[5,0] N[6,0]
N[1] movi 0 N[4,1]
N[2] movi 1 N[5,1]
N[3] movi 2 N[10]
N[10] mov N[6,1]
N[4] teq N[16,P] N[7,0]
N[5] teq N[16,P] N[7,1]
N[6] teq N[16,P] N[8,1]
N[7] or N[8,0]
N[8] or N[17,P]
N[16] movi_t 1 W[3]
N[17] movi_f -1 W[3]
N[28] ret
W[3] write G[3]
.bend

.bbegin _exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend

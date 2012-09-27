; Test:     br_2
; Author:   Haiming Liu
; Purpose:  Test br instruction

; Final result should be 0x0

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu %hi(block2) N[1]
N[1] app %mid(block2) N[2]
N[2] app %lo(block2) N[3]
N[3] app %bottom(block2) N[5]
N[4] movi 0 W[3]                ; return value 0
N[5] br 
W[3] write G[3]
.bend

.bbegin block1
N[0] genu %hi(exit) N[1]
N[1] app %mid(exit) N[2]
N[2] app %lo(exit) N[3]
N[3] app %bottom(exit) N[4]
N[4] br 
.bend

.bbegin block2
N[0] genu %hi(block1) N[1]
N[1] app %mid(block1) N[2]
N[2] app %lo(block1) N[3]
N[3] app %bottom(block1) N[4]
N[4] br 
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend

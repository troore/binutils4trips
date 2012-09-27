; Test:     call_1
; Author:   Haiming Liu
; Purpose:  Test call instruction, but not ret instruction

; Final result should be 0x0

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu %hi(exit) N[1]
N[1] app %mid(exit) N[2]
N[2] app %lo(exit) N[3]
N[3] app %bottom(exit) N[5]
N[4] movi 0 W[3]
N[5] call               ;will exit, no return

W[3] write G[3]
.bend

.bbegin block1          ;not used
N[0] bro exit
.bend

.bbegin block2          ;not used
N[0] genu %hi(block1) N[1]
N[1] app %mid(block1) N[2]
N[2] app %lo(block1) N[3]
N[3] app %bottom(block1) N[4]
N[4] ret
.bend

.bbegin exit
N[0] movi SYS_EXIT W[0]
N[1] scall
W[0] write G[0]
.bend

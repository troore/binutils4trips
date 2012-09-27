; Test:     call_2
; Author:   Haiming Liu
; Purpose:  Test both call instruction and ret instruction

; Final result should be 0x0

.set SYS_EXIT=1

.text
.global _start
.bbegin _start
N[0] genu %hi(block2) N[1]
N[1] app %mid(block2) N[2]
N[2] app %lo(block2) N[3]
N[3] app %bottom(block2) N[5]
N[4] movi 0 W[3]
N[5] call               

W[3] write G[3]
.bend

.bbegin block1          
N[0] bro exit
.bend

.bbegin block2          
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
